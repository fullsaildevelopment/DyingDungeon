#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "Buffer.h"
#include "RenderState.h"
#include <random>
#include "ReadWriteLock.h"
#include "Transform.h"

namespace Odyssey
{
	// Forward declaration
	class Material;
	class Shader;
	class Texture;
	class DepthState;
	class BlendState;

	enum class EmissionType
	{
		None,
		Looping,
		Burst
	};

	class ParticleSystemShape
	{
	public:
		DirectX::XMFLOAT3 center;
		DirectX::XMFLOAT3 mRadius;
		float minXDirection, maxXDirection;
		float minYDirection, maxYDirection;
		float minZDirection, maxZDirection;
		bool isCircle, isSphere;
	public:
		void setRadius(DirectX::XMFLOAT3 radius)
		{
			mRadius = radius;
		}
	};

	class BoxPS : public ParticleSystemShape
	{
	public:
		BoxPS()
		{
			center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			mRadius = { 1.0f, 1.0f, 1.0f };

			minXDirection = -0.25f;
			maxXDirection = 0.25f;
			minYDirection = 1.0f;
			maxYDirection = 1.0f;
			minZDirection = -0.25f;
			maxZDirection = 0.25f;
			isCircle = isSphere = false;
		}

		BoxPS(float posX, float posY, float posZ, float width, float height, float depth)
		{
			center = DirectX::XMFLOAT3(posX, posY, posZ);
			mRadius = { width / 2.0f, height / 2.0f, depth / 2.0f };

			minXDirection = -0.1f;
			maxXDirection = 0.1f;
			minYDirection = 0.5f;
			maxYDirection = 1.0f;
			minZDirection = -0.1f;
			maxZDirection = 0.1f;
			isCircle = isSphere = false;
		}
	};

	class CirclePS : public ParticleSystemShape
	{
	public:
		CirclePS(float posX, float posY, float posZ, float xRadius, float zRadius, bool outline)
		{
			center = DirectX::XMFLOAT3(posX, posY, posZ);
			mRadius = { xRadius, 0.0f, zRadius };

			minXDirection = 0.0f;
			maxXDirection = 0.0f;
			minYDirection = 1.0f;
			maxYDirection = 1.0f;
			minZDirection = 0.0f;
			maxZDirection = 0.0f;
			isCircle = true;
			isSphere = false;
		}
	};

	class ConePS : public ParticleSystemShape
	{
	public:
		ConePS()
		{
			center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			mRadius = { 1.0f, 1.0f, 1.0f };

			minXDirection = -0.25f;
			maxXDirection = 0.25f;
			minYDirection = 1.0f;
			maxYDirection = 1.0f;
			minZDirection = -0.25f;
			maxZDirection = 0.25f;
			isCircle = isSphere = false;
		}

		ConePS(float posX, float posY, float posZ, float rad, float xAxisAngle, float zAxisAngle)
		{
			center = DirectX::XMFLOAT3(posX, posY, posZ);
			mRadius = { rad, rad, rad };

			minXDirection = -xAxisAngle / 180.0f;
			maxXDirection = xAxisAngle / 180.0f;
			minYDirection = 1.0f;
			maxYDirection = 1.0f;
			minZDirection = -zAxisAngle / 180.0f;
			maxZDirection = zAxisAngle / 180.0f;
			isCircle = isSphere = false;
		}
	};

	class SpherePS : public ParticleSystemShape
	{
	public:
		SpherePS()
		{
			center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			mRadius = { 1.0f, 1.0f, 1.0f };

			minXDirection = -1.0f;
			maxXDirection = 1.0f;
			minYDirection = -1.0f;
			maxYDirection = 1.0f;
			minZDirection = -1.0f;
			maxZDirection = 1.0f;
			isCircle = false;
			isSphere = true;
		}

		SpherePS(float x, float y, float z, float rad)
		{
			center = DirectX::XMFLOAT3(x, y, z);
			mRadius = { rad, rad, rad };

			minXDirection = -1.0f;
			maxXDirection = 1.0f;
			minYDirection = -1.0f;
			maxYDirection = 1.0f;
			minZDirection = -1.0f;
			maxZDirection = 1.0f;
			isCircle = false;
			isSphere = true;
		}
	};

	class ParticleSystem : public Component
	{
		CLASS_DECLARATION(ParticleSystem)
	public:
		ParticleSystem(const ParticleSystem& copy);
		virtual std::shared_ptr<Component> clone() const;
	public: // Rule of 3
		ParticleSystem();
	public: // Interface
		virtual void initialize();
		virtual void update(double deltaTime);
		virtual void onDestroy();
	public: // Helpers
		void run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void setShape(ParticleSystemShape shape);
		void setTexture(TextureType textureType, const char* filename);
		void setColor(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor);
		void setLifetime(float min, float max);
		void setParticleCount(int startingCount, int maxCount);
		void setEmissionOverLifetime(int emissionRate);
		void setSpeed(float min, float max);
		void setSize(float min, float max);
		void setSizeOverLifetime(float min, float max);
		DirectX::XMFLOAT3 getStartColor();
		DirectX::XMFLOAT3 getEndColor();
		void setLooping(bool looping);
		void setDuration(double duration);
		void setGravity(float gravity);
		ParticleSystemShape* getShape();
		void play();
		void pause();
		void stop();
	private:
		void setDefaultEmitterData();
		void setInitialData();
		void createParticleBuffer();
		void createParticle(Particle& particle);
		DirectX::XMFLOAT3 generateParticlePosition(std::mt19937& generator, bool isCircle);
		DirectX::XMFLOAT3 generateParticleVelocity(std::mt19937& generator, DirectX::XMFLOAT3 particlePosition, float speed, bool isSphere);
		int runEmission(double deltaTime);
		DirectX::XMFLOAT4 colorLerp(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor, float ratio);
		float lerp(float start, float end, float ratio);
	private: // Members
		ParticleSystemShape mShape;
		DirectX::XMFLOAT3 mStartColor;
		DirectX::XMFLOAT3 mEndColor;
		DirectX::XMFLOAT3 mFadeStart;
		DirectX::XMFLOAT3 mFadeEnd;
		float mMinLife, mMaxLife;
		float mMinSpeed, mMaxSpeed;
		float mMinSize, mMaxSize;
		float mMinSizeLifetime, mMaxSizeLifetime;
		float mGravity, mCurrentGravity;
		int mStartCount, mMaxCount;
		int mEmissionRate;
		bool mIsLooping;
		bool mIsPlaying;
		double mDuration;
		double mCurrentTime;
		bool mGravityEnabled;
		double time;
		int numSpawn;
		double totalTime;
		int mCurrentEmission;
	private: // Rendering
		int mVertexShader;
		int mGeometryShader;
		int mPixelShader;
		int mRenderState;
		int mDepthState;
		//int mBlendState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		int mTexture;
		int mParticleBuffer;
		std::vector<Particle> mParticleData;
		Transform* mTransform;
	private:
		std::random_device rnd;
		ReadWriteLock mLock;
	};
}