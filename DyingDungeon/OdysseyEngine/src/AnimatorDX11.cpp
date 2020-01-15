#include "AnimatorDX11.h"
#include "Buffer.h"
#include "DebugManager.h"
#include "RenderDevice.h"

namespace Odyssey
{
	CLASS_DEFINITION(Animator, AnimatorDX11)

	AnimatorDX11::AnimatorDX11(RenderDevice& renderDevice)
	{
		// Set default values
		mCurrentClip.nextFrame = 0;
		mCurrentClip.prevFrame = 0;
		mCurrentClip.maxFrames = 0;
		mCurrentClip.currentTime = 0.0f;
		mCurrentClip.progress = 0.0f;
		mIsActive = false;
		mIsPlaying = false;
		mDebugEnabled = false;

		// Create the constant buffer for skinned animation
		mAnimationBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(AnimationBuffer)), nullptr);

		// No animation data to process for skinned rendering
		mAnimationData.hasAnimationData = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	void AnimatorDX11::initialize()
	{
		setActive(true);
	}

	void AnimatorDX11::update(double deltaTime)
	{
		// Don't update while paused or inactive or if there are no keyframes in the clip
		if (mIsPlaying == false || mIsActive == false || mCurrentClip.keyframes.size() == 0)
		{
			return;
		}

		// If there is a sequence, update it
		if (false && mSequence.isBlend)
		{
			updateSequence(deltaTime);
			return;
		}

		// Convert the delta time from seconds to milliseconds to match the animation units
		deltaTime *= 1000.0;

		// Process the current animation clip and update it to the current frame
		processAnimationClip(mCurrentClip, deltaTime, mCurrentClip.currentFrame);

		// Send the current keyframe to the shader for skinning
		updateAnimationBuffer(mCurrentClip.currentFrame);
	}

	void AnimatorDX11::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Update the constant buffer and bind it to slot 1 of the vertex shader
		mAnimationBuffer->updateData(context , &mAnimationData);
		mAnimationBuffer->bind(context, 2, ShaderType::VertexShader);
	}

	void AnimatorDX11::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Ubbind the constant buffer from slot 1 of the vertex shader
		mAnimationBuffer->unbind(context, 2, ShaderType::VertexShader);
	}

	void AnimatorDX11::debugDraw(DirectX::XMFLOAT4X4& globalTransform, DirectX::XMFLOAT3 color)
	{
		// Don't debug inactive animators
		if (mIsActive == false) { return; }
		if (mDebugEnabled == false) { return; }

		// Iterate over each joint
		for (int i = 0; i < mSkeleton.size(); i++)
		{
			// Get the joint's position in the current frame and transform it into world space
			DirectX::XMFLOAT3 jointPos = { mCurrentClip.currentFrame.jointTransforms[i].m[3][0], mCurrentClip.currentFrame.jointTransforms[i].m[3][1], mCurrentClip.currentFrame.jointTransforms[i].m[3][2] };
			DirectX::XMStoreFloat3(&jointPos, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&jointPos), DirectX::XMLoadFloat4x4(&globalTransform)));

			// Get the index of the parent joint
			int pIndex = mSkeleton[i].parentIndex;

			// Check if the joint has a parent
			if (pIndex != -1)
			{
				// Get the joint's parent's position in the current frame and transform it into world space
				DirectX::XMFLOAT3 parentPos = { mCurrentClip.currentFrame.jointTransforms[pIndex].m[3][0], mCurrentClip.currentFrame.jointTransforms[pIndex].m[3][1], mCurrentClip.currentFrame.jointTransforms[pIndex].m[3][2] };
				DirectX::XMStoreFloat3(&parentPos, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&parentPos), DirectX::XMLoadFloat4x4(&globalTransform)));

				// Draw a line from the joint to it's parent
				DebugManager::getInstance().addLine(jointPos, parentPos, color, color);
			}

			// Draw a debug sphere at the joint
			DebugManager::getInstance().addSphere(jointPos, 0.025f, { 0,1,0 });
		}
	}

	void AnimatorDX11::setSkeleton(std::vector<Joint> skeleton)
	{
		// Set the skeleton associated with this animator
		mSkeleton = skeleton;
		mCurrentClip.currentFrame.jointTransforms.resize(mSkeleton.size());

		// If there are not frames inside of the clip, set the animator to inactive
		if (mAnimationMap.size() == 0)
		{
			mIsActive = false;
		}
		else
		{
			mIsActive = true;
		}
	}

	void AnimatorDX11::loopAnimationClip(AnimationClip& clip)
	{
		if (clip.loop == false)
		{
			clip.prevFrame = static_cast<unsigned int>(clip.keyframes.size()) - 2;
			clip.nextFrame = static_cast<unsigned int>(clip.keyframes.size()) - 1;
			pause();
		}
		else
		{
			clip = mAnimationMap["Idle"];
			clip.nextFrame = 1;
			clip.prevFrame = 0;
			clip.progress = 0.0f;
			clip.currentTime = 0.0f;
		}
	}

	void AnimatorDX11::updateSequence(double deltaTime)
	{
		// Convert the delta time from seconds to milliseconds to match the animation units
		deltaTime *= 1000.0;

		// Process the sequence's two animation clips and update them to the current frame
		processAnimationClip(mSequence.blendA, deltaTime, mSequence.blendA.currentFrame);
		processAnimationClip(mSequence.blendB, deltaTime, mSequence.blendB.currentFrame);

		Keyframe outFrame;
		// Blend together the two current keyframes from the animation clips based on the sequence's blend factor
		blendKeyframes(mSequence.blendA.currentFrame, mSequence.blendB.currentFrame, mSequence.blendFactor, outFrame);

		// Send the blended current frame to the shader for skinning
		updateAnimationBuffer(outFrame);
	}

	void AnimatorDX11::updateAnimationTime(AnimationClip& clip, double deltaTime, Keyframe& outPrevKeyframe, Keyframe& outNextKeyframe)
	{
		// Add deltat time to the animation clip time
		clip.currentTime += deltaTime;

		// Find the target frame time
		// If the animation has looped, we need to target the duration so the animation finishes playing
		double frameTime = (clip.nextFrame == 0) ? clip.duration : clip.keyframes[clip.nextFrame].time;

		// Check if the frame time exceeds the current frame's time
		if (clip.currentTime >= frameTime)
		{
			// Change to the next frame
			clip.nextFrame++;

			// Clamp the frame from going above max frames
			clip.prevFrame = clip.nextFrame - 1;
			clip.nextFrame = clip.nextFrame % clip.maxFrames;

			// Set the time to the passed frame's time
			clip.currentTime = clip.keyframes[clip.prevFrame].time;

			if (clip.prevFrame == clip.maxFrames - 1)
			{
				loopAnimationClip(clip);
			}
		}

		// Assign the appropriate out keyframes
		outPrevKeyframe = clip.keyframes[clip.prevFrame];
		outNextKeyframe = clip.keyframes[clip.nextFrame];
	}

	void AnimatorDX11::processAnimationClip(AnimationClip& clip, double deltaTime, Keyframe& processedKeyframe)
	{
		// Create the containers for the previous and next frames of the current clip
		Keyframe prevFrame, nextFrame;

		// Advance the animation forward in time and get the appropriate previous and next frames
		updateAnimationTime(clip, deltaTime, prevFrame, nextFrame);

		// Calculate the interpolation ratio
		float totalTime = (clip.nextFrame == 0) ? static_cast<float>(clip.duration) : static_cast<float>(nextFrame.time);
		float ratio = static_cast<float>(clip.currentTime - prevFrame.time) / (totalTime - static_cast<float>(prevFrame.time));
		clip.progress = static_cast<float>(clip.currentTime / clip.duration);

		// Blend the two keyframes and store it as the "current" keyframe
		blendKeyframes(prevFrame, nextFrame, ratio, processedKeyframe);
	}

	void AnimatorDX11::blendKeyframes(Keyframe& keyframeA, Keyframe& keyframeB, float blendFactor, Keyframe& outKeyframe)
	{
		// Only clips with the same amount of joints can be blended
		assert(keyframeA.jointTransforms.size() == keyframeB.jointTransforms.size());

		// Get the number of joints in the keyframe
		unsigned int numJoints = static_cast<unsigned int>(keyframeA.jointTransforms.size());

		// Resize the out keyframe to match the number of joints
		outKeyframe.jointTransforms.resize(numJoints);

		// Iterate over each joint of the skeleton
		for (unsigned int i = 0; i < numJoints; i++)
		{
			// Interpolate between the previous frame and the next frame's joint transforms and store it in the out keyframe
			interpolateJointMatrix(keyframeA.jointTransforms[i], keyframeB.jointTransforms[i], blendFactor, outKeyframe.jointTransforms[i]);
		}
	}

	void AnimatorDX11::updateAnimationBuffer(Keyframe& animationKeyframe)
	{
		// Iterate over the animation keyframe joints
		for (int i = 0; i < animationKeyframe.jointTransforms.size(); i++)
		{
			// Calculate the final transform for the joint
			DirectX::XMMATRIX finalMatrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&mSkeleton[i].invBindposeTransform), DirectX::XMLoadFloat4x4(&animationKeyframe.jointTransforms[i]));

			// Store the transpose of this final matrix in the constant buffer for the skinning shader
			DirectX::XMStoreFloat4x4(&mAnimationData.finalPose[i], DirectX::XMMatrixTranspose(finalMatrix));
		}

		// Flag the shader that there is animation data to be processed
		mAnimationData.hasAnimationData = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	void AnimatorDX11::interpolateJointMatrix(DirectX::XMFLOAT4X4& prevTransform, DirectX::XMFLOAT4X4& nextTransform, float ratio, DirectX::XMFLOAT4X4& outTransform)
	{
		// Get the positions from the prev and next frame transforms
		DirectX::XMVECTOR prevPosition = DirectX::XMVectorSet(prevTransform.m[3][0], prevTransform.m[3][1], prevTransform.m[3][2], 1.0f);
		DirectX::XMVECTOR nextPosition = DirectX::XMVectorSet(nextTransform.m[3][0], nextTransform.m[3][1], nextTransform.m[3][2], 1.0f);

		// Get the quaternion rotation from the prev and next frame transforms
		DirectX::XMVECTOR prevRotation = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&prevTransform));
		DirectX::XMVECTOR nextRotation = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&nextTransform));

		// Calculate the slerp matrix from the prev and next frame using the parameter ratio
		DirectX::XMMATRIX slerpMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionSlerp(prevRotation, nextRotation, ratio));

		// Lerp the prev and next positions
		DirectX::XMVECTOR lerpPosition = DirectX::XMVectorLerp(prevPosition, nextPosition, ratio);

		// Multiply the slerp matrix into the lerp position for the final interpolated matrix
		slerpMatrix = DirectX::XMMatrixMultiply(slerpMatrix, DirectX::XMMatrixTranslationFromVector(lerpPosition));

		// Store the slerp matrix in an XMFloat4x4 and return
		DirectX::XMStoreFloat4x4(&outTransform, slerpMatrix);
	}
}