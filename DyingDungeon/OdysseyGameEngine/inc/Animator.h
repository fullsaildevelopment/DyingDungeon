#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	struct Joint
	{
		std::string name;
		DirectX::XMFLOAT4X4 invBindposeTransform;
		DirectX::XMFLOAT4X4 bindposeTransform;
		int parentIndex;
	};

	struct Keyframe
	{
		int frameNumber;
		double time;
		std::vector<DirectX::XMFLOAT4X4> jointTransforms;
	};

	struct AnimationClip
	{
		unsigned int maxFrames;
		unsigned int framesPerSecond;
		std::vector<Keyframe> keyframes;
		Keyframe currentFrame;
		double duration;
		double currentTime;
		unsigned int prevFrame;
		unsigned int nextFrame;
	};

	struct AnimationBuffer
	{
		DirectX::XMFLOAT4 hasAnimationData;
		DirectX::XMFLOAT4X4 finalPose[128];
	};

	struct AnimationSequence
	{
		bool isBlend;
		AnimationClip blendA;
		AnimationClip blendB;
		float blendFactor;
	};

	class Buffer;
	class RenderDevice;

	class Animator : public Component
	{
		CLASS_DECLARATION(Animator)
	public:
		/**
		 *	Import an animation clip into the Animator component.
		 *	@param[in] animationName The custom string name of the animation clip.
		 *	@param[in] filename The file path and name of the animation clip file.
		 *	@return void
		 */
		void importAnimation(const char* animationName, const char* filename);

		/**
		 *	Set the skeleton attached to the Animator component
		 *	@param[in] skeleton The container of joints associated with the skeelton.
		 *	@return void
		 */
		void setAnimationClip(const char* clipToPlay);

		/**
		 *	Set current animation sequence.
		 *	@param[in] clipA The string ID of the first animation clip.
		 *	@param[in] clipB The string ID of the second animation clip.
		 *	@param[in] blendFactor The amount of blending to apply to the animation sequence.
		 *	@return void
		 */
		void setAnimationSequence(const char* clipA, const char* clipB, float blendFactor);

		/**
		 *	Play the current animation clip, if one is set.
		 *	@param[in] void
		 *	@return void
		 */
		void play();

		/**
		 *	Pause the current animation clip, if one is set.
		 *	@param[in] void
		 *	@return void
		 */
		void pause();

		/**
		 *	Reset the current animation clip to the first frame, if one is set.
		 *	@param[in] void
		 *	@return void
		 */
		void reset();

		/**
		 *	Get the active state of the Animator component.
		 *	@param[in] void
		 *	@return bool The active state of the Animator component.
		 */
		bool getActive();

		/**
		 *	Set the debug state of the Animator component.
		 *	@param[in] enabled The new debug state.
		 *	@return void
		 */
		void setDebugEnabled(bool enabled);

		/**
		 *	Get the debug state of the Animator component.
		 *	@param[in] void
		 *	@return bool The debug state of the Animator component.
		 */
		bool getDebugEnabled();	
	protected: // Animation clips
		AnimationSequence mSequence;
		AnimationClip mCurrentClip;
		bool mIsActive;
		bool mIsPlaying;
		bool mDebugEnabled;
		std::map<const char*, AnimationClip> mAnimationMap;
	};
}