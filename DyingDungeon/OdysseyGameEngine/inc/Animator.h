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
	public: // Constructors
		Animator(RenderDevice& renderDevice);
	public: // Component Inherited Functions
		virtual void initialize();
		virtual void update(double deltaTime);
		virtual void onEnable();
		virtual void onDisable();
	public: // Import Functions
		void importAnimation(const char* animationName, const char* filename);
	public: // Mutators
		void setSkeleton(std::vector<Joint> skeleton);
		void setWorldMatrix(DirectX::XMFLOAT4X4 worldMatrix);
		void setAnimationClip(const char* clipToPlay);
		void setAnimationSequence(const char* clipA, const char* clipB, float blendFactor);
	public: // Interactions

	public: // Update Functions
		void updateSequence(double deltaTime);
		void play();
		void pause();
		void reset();
	public: // Rendering
		void bind();
		void unbind();
	public: // Mutators
		void addAnimationBlend(float addition);
	public: // Accessors
		bool getActive();
	public: // Debug Functions
		void setDebugEnabled(bool enabled);
		bool getDebugEnabled();
		void debugDraw(DirectX::XMFLOAT3 color);
	private: // Helper Functions
		void updateAnimationTime(AnimationClip& clip, double deltaTime, Keyframe& outPrevKeyframe, Keyframe& outNextKeyframe);
		void blendKeyframes(Keyframe& keyframeA, Keyframe& keyframeB, float blendFactor, Keyframe& outKeyframe);
		void updateAnimationBuffer(Keyframe& animationKeyframe);
		DirectX::XMFLOAT4X4 interpolateJointMatrix(DirectX::XMFLOAT4X4 before, DirectX::XMFLOAT4X4 after, float ratio);
	private: // Framework variables
		std::shared_ptr<Buffer> mAnimationBuffer;
		AnimationBuffer mAnimationData;
		std::vector<Joint> mSkeleton;
	private: // Animation clips
		AnimationSequence mSequence;
		AnimationClip mCurrentClip;
		bool mIsActive;
		bool mIsPlaying;
		bool mDebugEnabled;
		DirectX::XMFLOAT4X4 mWorldMatrix;
		std::map<const char*, AnimationClip> mAnimationMap;
	};
}