#include <fstream>
#include "Animator.h"
#include "Buffer.h"
#include "DebugManager.h"
#include "RenderDevice.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, Animator)

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

	bool Animator::getActive()
	{
		return mIsActive;
	}

	void Animator::setDebugEnabled(bool enabled)
	{
		mDebugEnabled = enabled;
	}

	bool Animator::getDebugEnabled()
	{
		return mDebugEnabled;
	}
}