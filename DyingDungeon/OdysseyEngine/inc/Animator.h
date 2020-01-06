#pragma once
#include "EngineIncludes.h"
#include "RenderTypes.h"
#include "Component.h"

namespace Odyssey
{
	// Forward declarations
	class Buffer;
	class RenderDevice;

	class Animator : public Component
	{
		CLASS_DECLARATION(Animator)

	public: // Interface
		/**
		 *	Import a custom binary file containing an animation clip into the animator component.
		 *	@param[in] animationName The custom string name of the animation clip.
		 *	@param[in] filename The file path and name of the animation clip file.
		 *	@return void
		 */
		void importAnimation(std::string animationName, const char* filename, bool isLooping = true);

		/**
		 *	Play the animation clip associated with the parameter clip name.
		 *	@param[in] animation The name of the animation clip to play.
		 *	@return void
		 */
		void playClip(std::string animation);

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
		 *	Get the current animation clip's progress.
		 *	@param[in] void
		 *	@return float The progress of the current animation clip.
		 */
		float getProgress();

		/**
		 *	Set the animator's debug state.
		 *	@param[in] enabled The debug state of the animator.
		 *	@return void
		 */
		void setDebugEnabled(bool enabled);

		/**
		 *	Get the animator's debug state
		 *	@param[in] void
		 *	@return bool The debug state of the Animator.
		 */
		bool getDebugEnabled();	

	protected: // Members
		AnimationSequence mSequence;
		AnimationClip mCurrentClip;
		bool mIsPlaying;
		bool mDebugEnabled;
		std::map<std::string, AnimationClip> mAnimationMap;
	};
}