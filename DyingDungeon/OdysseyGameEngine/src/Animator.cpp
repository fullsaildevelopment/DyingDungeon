#include <fstream>
#include "Animator.h"
#include "BufferManager.h"
#include "RenderManager.h"
#include "Buffer.h"
#include "DebugManager.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Animator)

	Animator::Animator()
	{
		mCurrentClip.nextFrame = 0;
		mCurrentClip.prevFrame = 0;
		mCurrentClip.maxFrames = 0;
		mCurrentClip.currentTime = 0.0f;
		mIsActive = false;
		mIsPlaying = false;
		mDebugEnabled = false;

		// Create the constant buffer for skinned animation
		mAnimationBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(AnimationBuffer), nullptr);

		// No animation data to process for skinned rendering
		mAnimationData.hasAnimationData = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	void Animator::initialize(SceneObject* parent)
	{
		setEnabled(true);
		mParent = parent;
	}

	void Animator::importAnimation(const char* animationName, const char* filename)
	{
		// Open/Create the mesh file
		std::fstream animFile(filename, std::ios_base::in | std::ios_base::binary);

		// Ensure the file was opened
		assert(animFile.is_open());

		// Create a new animation clip for this import
		AnimationClip clip;

		// Read the clip duration
		animFile.read((char*)&clip.duration, sizeof(clip.duration));

		// Read the number of frames
		uint64_t numFrames;
		animFile.read((char*)&numFrames, sizeof(numFrames));

		// Assign the max number of frames
		clip.maxFrames = static_cast<int>(numFrames);
		clip.prevFrame = 0;
		clip.nextFrame = 1;

		// Read in the frames per second
		animFile.read((char*)&clip.framesPerSecond, sizeof(clip.framesPerSecond));

		// Iterate over each frames
		for (int i = 0; i < numFrames; i++)
		{
			Keyframe frame;
			// Read the keyframe number
			animFile.read((char*)&frame.frameNumber, sizeof(frame.frameNumber));
			// Read the keyframe time
			animFile.read((char*)&frame.time, sizeof(frame.time));
			// Read the number of joints
			uint64_t numJoints;
			animFile.read((char*)&numJoints, sizeof(numJoints));
			// Resize the joint transforms to match
			frame.jointTransforms.resize(numJoints);

			// Read in the joint data
			for (int j = 0; j < numJoints; j++)
			{
				// Read the joint transform
				animFile.read((char*)&frame.jointTransforms[j], sizeof(DirectX::XMFLOAT4X4));
			}

			// Push the keyframe into the list
			clip.keyframes.push_back(frame);
		}

		// Close the file
		animFile.close();

		// Add the animation to the map
		clip.currentTime = clip.keyframes[0].time;
		mAnimationMap[animationName] = clip;

		// The imported clip is the new current clip
		mCurrentClip = clip;
		mCurrentClip.currentFrame.jointTransforms.resize(mCurrentClip.keyframes[0].jointTransforms.size());

		// Set the animator to active and playing
		mIsActive = true;
		mIsPlaying = true;

		// Reset the frame
		reset();
	}

	void Animator::setSkeleton(std::vector<Joint> skeleton)
	{
		// Set the skeleton associated with this animator
		mSkeleton = skeleton;
		mCurrentClip.currentFrame.jointTransforms.resize(mSkeleton.size());

		// If there are not frames inside of the clip, set the animator to inactive
		if (mAnimationMap.size() == 0)
		{
			mIsActive = false;
		}
		mIsActive = true;
	}

	void Animator::setWorldMatrix(DirectX::XMFLOAT4X4 worldMatrix)
	{
		mWorldMatrix = worldMatrix;
	}

	void Animator::update(double deltaTime)
	{
		// Don't update while paused or inactive or if there are no keyframes in the clip
		if (mIsPlaying == false || mIsActive == false || mCurrentClip.keyframes.size() == 0)
		{
			return;
		}

		if (mSequence.isBlend)
		{
			updateSequence(deltaTime);
			return;
		}
		// Convert the delta time from seconds to milliseconds to match the animation units
		deltaTime *= 1000.0;

		// Create the containers for the previous and next frames of the current clip
		Keyframe prevFrame, nextFrame;

		// Advance the animation forward in time and get the appropriate previous and next frames
		updateAnimationTime(mCurrentClip, deltaTime, prevFrame, nextFrame);

		// Calculate the interpolation ratio
		float totalTime = (mCurrentClip.nextFrame == 0) ? static_cast<float>(mCurrentClip.duration) : static_cast<float>(nextFrame.time);
		float ratio = static_cast<float>(mCurrentClip.currentTime - prevFrame.time) / (totalTime - static_cast<float>(prevFrame.time));

		blendKeyframes(prevFrame, nextFrame, ratio, mCurrentClip.currentFrame);

		updateAnimationBuffer(mCurrentClip.currentFrame);
	}

	void Animator::onEnable()
	{
	}

	void Animator::onDisable()
	{
	}

	void Animator::updateSequence(double deltaTime)
	{
		// Convert the delta time from seconds to milliseconds to match the animation units
		deltaTime *= 1000.0;

		// Create the containers for the previous and next frames of the current clip
		Keyframe prevFrameA, nextFrameA;

		// Advance the animation forward in time and get the appropriate previous and next frames
		updateAnimationTime(mSequence.blendA, deltaTime, prevFrameA, nextFrameA);

		// Calculate the interpolation ratio
		float totalTime = (mSequence.blendA.nextFrame == 0) ? static_cast<float>(mSequence.blendA.duration) : static_cast<float>(nextFrameA.time);
		float ratio = static_cast<float>(mSequence.blendA.currentTime - prevFrameA.time) / (totalTime - static_cast<float>(prevFrameA.time));

		blendKeyframes(prevFrameA, nextFrameA, ratio, mSequence.blendA.currentFrame);

		Keyframe prevFrameB, nextFrameB;
		updateAnimationTime(mSequence.blendB, deltaTime, prevFrameB, nextFrameB);

		// Calculate the interpolation ratio
		totalTime = (mSequence.blendB.nextFrame == 0) ? static_cast<float>(mSequence.blendB.duration) : static_cast<float>(nextFrameB.time);
		ratio = static_cast<float>(mSequence.blendB.currentTime - prevFrameB.time) / (totalTime - static_cast<float>(prevFrameB.time));

		blendKeyframes(prevFrameB, nextFrameB, ratio, mSequence.blendB.currentFrame);
		Keyframe outFrame;
		blendKeyframes(mSequence.blendA.currentFrame, mSequence.blendB.currentFrame, mSequence.blendFactor, outFrame);

		updateAnimationBuffer(outFrame);
	}

	void Animator::play()
	{
		mIsPlaying = true;
	}

	void Animator::pause()
	{
		// Invert the current paused state
		mIsPlaying = false;
	}

	void Animator::reset()
	{
		// Set the animation clip state to return to the start
		mCurrentClip.nextFrame = 1;
		mCurrentClip.prevFrame = 0;
		mCurrentClip.currentTime = mCurrentClip.keyframes[mCurrentClip.prevFrame].time;
		mCurrentClip.maxFrames = static_cast<int>(mCurrentClip.keyframes.size());
	}

	void Animator::bind()
	{
		// Update the constant buffer and bind it to slot 1 of the vertex shader
		mAnimationBuffer->updateData(&mAnimationData);
		mAnimationBuffer->bind(1, ShaderType::VertexShader);
	}

	void Animator::unbind()
	{
		// Ubbind the constant buffer from slot 1 of the vertex shader
		mAnimationBuffer->unbind(1, ShaderType::VertexShader);
	}

	void Animator::setAnimationClip(const char* clipToPlay)
	{
		// Add the animation clip to the map and set the clip to the beginning
		mCurrentClip = mAnimationMap[clipToPlay];
		reset();
	}

	void Animator::setAnimationSequence(const char* clipA, const char* clipB, float blendFactor)
	{
		mSequence.isBlend = true;
		mSequence.blendA = mAnimationMap[clipA];
		mSequence.blendB = mAnimationMap[clipB];
		mSequence.blendFactor = blendFactor;
	}

	void Animator::addAnimationBlend(float addition)
	{
		mSequence.blendFactor += addition;
		mSequence.blendFactor = max(0.0f, min(mSequence.blendFactor, 1.0f));
	}

	bool Animator::getActive()
	{
		return mIsActive;
	}

	void Animator::debugDraw(DirectX::XMFLOAT3 color)
	{
		// Don't debug inactive animators
		if (mIsActive == false) { return; }
		if (mDebugEnabled == false) { return; }

		// Iterate over each joint
		for (int i = 0; i < mSkeleton.size(); i++)
		{
			// Get the joint's position in the current frame and transform it into world space
			DirectX::XMFLOAT3 jointPos = { mCurrentClip.currentFrame.jointTransforms[i].m[3][0], mCurrentClip.currentFrame.jointTransforms[i].m[3][1], mCurrentClip.currentFrame.jointTransforms[i].m[3][2] };
			DirectX::XMStoreFloat3(&jointPos, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&jointPos), DirectX::XMLoadFloat4x4(&mWorldMatrix)));

			// Get the index of the parent joint
			int pIndex = mSkeleton[i].parentIndex;

			// Check if the joint has a parent
			if (pIndex != -1)
			{
				// Get the joint's parent's position in the current frame and transform it into world space
				DirectX::XMFLOAT3 parentPos = { mCurrentClip.currentFrame.jointTransforms[pIndex].m[3][0], mCurrentClip.currentFrame.jointTransforms[pIndex].m[3][1], mCurrentClip.currentFrame.jointTransforms[pIndex].m[3][2] };
				DirectX::XMStoreFloat3(&parentPos, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&parentPos), DirectX::XMLoadFloat4x4(&mWorldMatrix)));

				// Draw a line from the joint to it's parent
				//DebugManager::getInstance().addLine(jointPos, parentPos, color, color);
			}

			// Draw a debug sphere at the joint
			//DebugManager::getInstance().addSphere(jointPos, 0.025f, { 0,1,0 });
		}
	}

	void Animator::updateAnimationTime(AnimationClip& clip, double deltaTime, Keyframe& outPrevKeyframe, Keyframe& outNextKeyframe)
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
		}

		// Assign the appropriate out keyframes
		outPrevKeyframe = clip.keyframes[clip.prevFrame];
		outNextKeyframe = clip.keyframes[clip.nextFrame];
	}

	void Animator::blendKeyframes(Keyframe& keyframeA, Keyframe& keyframeB, float blendFactor, Keyframe& outKeyframe)
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
			// Interpolate between the previous frame and the next frame's joint transforms
			DirectX::XMFLOAT4X4 slerpMatrix = interpolateJointMatrix(keyframeA.jointTransforms[i], keyframeB.jointTransforms[i], blendFactor);

			// Assign this slerp matrix to the out keyframe's joint transform
			outKeyframe.jointTransforms[i] = slerpMatrix;
		}
	}

	void Animator::updateAnimationBuffer(Keyframe& animationKeyframe)
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

	void Animator::setDebugEnabled(bool enabled)
	{
		mDebugEnabled = enabled;
	}

	bool Animator::getDebugEnabled()
	{
		return mDebugEnabled;
	}

	DirectX::XMFLOAT4X4 Animator::interpolateJointMatrix(DirectX::XMFLOAT4X4 prevTransform, DirectX::XMFLOAT4X4 nextTransform, float ratio)
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
		DirectX::XMFLOAT4X4 outTransform;
		DirectX::XMStoreFloat4x4(&outTransform, slerpMatrix);
		return outTransform;
	}
}