#include "ParticleSystem.h"
#include "Buffer.h"
#include "Material.h"
#include "RenderState.h"
#include "Shader.h"
#include "Texture.h"
#include "Entity.h"
#include "DepthState.h"
#include "BlendState.h"
#include "RenderManager.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, ParticleSystem)

	ParticleSystem::ParticleSystem(const ParticleSystem& copy)
	{
		mShape = copy.mShape;
		mStartColor = copy.mStartColor;
		mEndColor = copy.mEndColor;
		mMinLife = copy.mMinLife;
		mMaxLife = copy.mMaxLife;
		mMinSpeed = copy.mMinSpeed;
		mMaxSpeed = copy.mMaxSpeed;
		mMinSize = copy.mMinSize;
		mMaxSize = copy.mMaxSize;
		mGravity = copy.mGravity;
		mCurrentGravity = copy.mCurrentGravity;
		mStartCount = copy.mStartCount;
		mMaxCount = copy.mMaxCount;
		mEmissionRate = copy.mEmissionRate;
		mIsLooping = copy.mIsLooping;
		mIsPlaying = copy.mIsPlaying;
		mDuration = copy.mDuration;
		mCurrentTime = copy.mCurrentTime;
		mGravityEnabled = copy.mGravityEnabled;
		time = copy.time;
		numSpawn = copy.numSpawn;
		totalTime = copy.totalTime;
		mCurrentEmission = copy.mCurrentEmission;
		mVertexShader = copy.mVertexShader;
		mGeometryShader = copy.mGeometryShader;
		mPixelShader = copy.mPixelShader;
		mRenderState = copy.mRenderState;
		mDepthState = copy.mDepthState;
		mBlendState = copy.mBlendState;
		mTexture = copy.mTexture;
		mParticleBuffer = copy.mParticleBuffer;
		mParticleData = copy.mParticleData;
		mEntity = copy.mEntity;
		mIsActive = copy.mIsActive;
	}

	std::shared_ptr<Component> ParticleSystem::clone() const
	{
		return std::make_shared<ParticleSystem>(*this);
	}

	ParticleSystem::ParticleSystem()
	{
		// Create the shaders
		mVertexShader = RenderManager::getInstance().createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/ParticleVertexShader.cso", nullptr, 0);
		mGeometryShader = RenderManager::getInstance().createShader(ShaderType::GeometryShader, "../OdysseyEngine/shaders/ParticleGeometryShader.cso", nullptr);
		mPixelShader = RenderManager::getInstance().createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/ParticlePixelShader.cso", nullptr);

		// Create the render state
		mRenderState = RenderManager::getInstance().createRenderState(Topology::PointList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, false, false);

		// Create the depth
		mDepthState = RenderManager::getInstance().createDepthState(false, true, ComparisonFunc::COMPARISON_LESS);

		// Create the blend state
		float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		BlendDesc blendDesc;
		blendDesc.mSrcBlend = Blend::BLEND_SRC_ALPHA;
		blendDesc.mDestBlend = Blend::BLEND_ONE;
		blendDesc.mBlendOp = BlendOperation::BLEND_OP_ADD;
		blendDesc.mSrcAlphaBlend = Blend::BLEND_ZERO;
		blendDesc.mDestAlphaBlend = Blend::BLEND_ONE;
		blendDesc.mAlphaBlendOp = BlendOperation::BLEND_OP_ADD;
		blendDesc.mAlphaToCoverage = true;
		blendDesc.mIndependentBlendEnable = false;
		mBlendState = RenderManager::getInstance().createBlendState(blendDesc, blendFactor);

		// Default to the fire texture
		mTexture = RenderManager::getInstance().createTexture(TextureType::Diffuse, "Particle.png");

		// Create the particle data buffer
		mStartCount = 10;
		mMaxCount = 100;
		mEmissionRate = 10;
		mShape = SpherePS();
		mStartColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mEndColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mMinSpeed = 0.0f;
		mMaxSpeed = 1.0f;
		mMinSize = 0.25f;
		mMaxSize = 1.0f;
		mMinLife = 0.0f;
		mMaxLife = 1.0f;
		mGravity = 0.0f;
		mGravityEnabled = false;
		mIsPlaying = true;
		time = 0.0;
		totalTime = 1.0;
		numSpawn = mEmissionRate;
		mCurrentEmission = 0;
	}

	void ParticleSystem::run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mLock.lock(LockState::Read);

		// Unbind any vertex buffers from the pipeline
		ID3D11Buffer* nBuffs[] = { nullptr };
		UINT stride = 0;
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, nBuffs, &stride, &offset);

		RenderManager& renderManager = RenderManager::getInstance();

		// Bind the depth state
		renderManager.getDepthState(mDepthState)->bind(context);

		// Update the particle buffer
		renderManager.getBuffer(mParticleBuffer)->updateData(context, mParticleData.data());

		// Bind the geometry shader
		renderManager.getShader(mGeometryShader)->bind(context);

		// Bind the particle buffer
		renderManager.getBuffer(mParticleBuffer)->bind(context, 0, ShaderType::GeometryShader);

		// Bind the render state
		renderManager.getRenderState(mRenderState)->bind(context);

		// Bind the vertex shader and material properties
		renderManager.getShader(mVertexShader)->bind(context);
		renderManager.getShader(mPixelShader)->bind(context);

		// Bind the particle texture
		renderManager.getTexture(mTexture)->bind(context);

		// Bind the blend state
		renderManager.getBlendState(mBlendState)->bind(context);

		// Draw the particles
		context->Draw(mMaxCount, 0);

		// Unbind the depth and blend state
		renderManager.getDepthState(mDepthState)->unbind(context);
		renderManager.getBlendState(mBlendState)->unbind(context);

		// Unbind the shaders and buffers
		renderManager.getBuffer(mParticleBuffer)->unbind(context, 0, ShaderType::GeometryShader);
		renderManager.getShader(mGeometryShader)->unbind(context);
		renderManager.getShader(mPixelShader)->unbind(context);
		renderManager.getShader(mVertexShader)->unbind(context);

		mLock.unlock(LockState::Read);
	}

	void ParticleSystem::setShape(ParticleSystemShape shape)
	{
		mShape = shape;
	}

	void ParticleSystem::setTexture(TextureType textureType, const char* filename)
	{
		mTexture = RenderManager::getInstance().createTexture(textureType, filename);
	}

	void ParticleSystem::setColor(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor)
	{
		mStartColor = startColor;
		mEndColor = endColor;
	}

	void ParticleSystem::setLifetime(float min, float max)
	{
		mMinLife = min;
		mMaxLife = max;
	}

	void ParticleSystem::setParticleCount(int startingCount, int maxCount)
	{
		mStartCount = startingCount;
		mMaxCount = maxCount;
		mParticleData.resize(mMaxCount);
	}

	void ParticleSystem::setRateOverTime(int emissionRate)
	{
		mEmissionRate = emissionRate;
		numSpawn = mEmissionRate;
	}

	void ParticleSystem::setSpeed(float min, float max)
	{
		mMinSpeed = min;
		mMaxSpeed = max;
	}

	void ParticleSystem::setSize(float min, float max)
	{
		mMinSize = min;
		mMaxSize = max;
	}

	DirectX::XMFLOAT3 ParticleSystem::getStartColor()
	{
		return mStartColor;
	}

	DirectX::XMFLOAT3 ParticleSystem::getEndColor()
	{
		return mEndColor;
	}

	void ParticleSystem::setLooping(bool looping)
	{
		mIsLooping = looping;
	}

	void ParticleSystem::setDuration(double duration)
	{
		mDuration = duration;
	}

	void ParticleSystem::setGravity(float gravity)
	{
		mGravity = gravity;
		mGravityEnabled = true;
	}

	void ParticleSystem::play()
	{
		mIsActive = true;
		mIsPlaying = true;
		mCurrentTime = 0.0f;

		setInitialData();
	}

	void ParticleSystem::pause()
	{
		mIsPlaying = false;
	}

	void ParticleSystem::stop()
	{
		mIsActive = false;
		mIsPlaying = false;
		mParticleData.clear();
		mParticleData.resize(mMaxCount);
		mCurrentTime = 0.0f;
	}

	void ParticleSystem::setInitialData()
	{
		for (int i = 0; i < mStartCount; i++)
		{
			Particle p;
			createParticle(p);
			mParticleData[i] = p;
		}
	}

	void ParticleSystem::createParticleBuffer()
	{
		if (mParticleData.size() != mMaxCount)
		{
			mParticleData.resize(mMaxCount);
		}

		mParticleBuffer = RenderManager::getInstance().createBuffer(BufferBindFlag::StructuredBuffer, size_t(mMaxCount),
			static_cast<UINT>(sizeof(Particle)), mParticleData.data());
	}

	void ParticleSystem::createParticle(Particle& particle)
	{
		std::mt19937 generator(rnd());
		std::uniform_real_distribution<float> xPosition(-1.0f * mShape.radius.x + mShape.center.x, mShape.center.x + mShape.radius.x);
		std::uniform_real_distribution<float> yPosition(-1.0f * mShape.radius.y + mShape.center.y, mShape.center.y + mShape.radius.y);
		std::uniform_real_distribution<float> zPosition(-1.0f * mShape.radius.z + mShape.center.z, mShape.center.z + mShape.radius.z);
		std::uniform_real_distribution<float> xDirection(mShape.minXDirection, mShape.maxXDirection);
		std::uniform_real_distribution<float> yDirection(mShape.minYDirection, mShape.maxYDirection);
		std::uniform_real_distribution<float> zDirection(mShape.minZDirection, mShape.maxZDirection);
		std::uniform_real_distribution<float> lf(mMinLife, mMaxLife);
		std::uniform_real_distribution<float> spd(mMinSpeed, mMaxSpeed);
		std::uniform_real_distribution<float> size(mMinSize, mMaxSize);
		float speed = spd(generator);

		particle.color = colorLerp(mStartColor, mEndColor, 0.0f);
		particle.position = DirectX::XMFLOAT3(xPosition(generator), yPosition(generator), zPosition(generator));
		particle.velocity = DirectX::XMFLOAT3(xDirection(generator), yDirection(generator), zDirection(generator));
		DirectX::XMStoreFloat3(&particle.velocity, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&particle.velocity)));
		particle.velocity = DirectX::XMFLOAT3(particle.velocity.x * speed, particle.velocity.y * speed, particle.velocity.z * speed);
		particle.size = size(generator);
		particle.active = true;
		particle.lifeTime = lf(generator);
		particle.startLifetime = particle.lifeTime;
	}

	int ParticleSystem::runEmission(double deltaTime)
	{
		int spawn = 0;

		double emissionInterval = totalTime / (double)numSpawn;

		time += deltaTime;

		if (time > emissionInterval)
		{
			spawn = static_cast<int>(floor(time / emissionInterval));
			spawn = max(0, min(spawn, mEmissionRate));
			totalTime -= time;
			numSpawn -= spawn;
			time = 0.0;
		}

		if (totalTime <= 0.0)
		{
			totalTime = 1.0;
			numSpawn = mEmissionRate;
		}
		return spawn;
	}

	DirectX::XMFLOAT4 ParticleSystem::colorLerp(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor, float ratio)
	{
		DirectX::XMFLOAT4 finalColor;
		finalColor.x = (1.0f - ratio) * startColor.x + (ratio * endColor.x);
		finalColor.y = (1.0f - ratio) * startColor.y + (ratio * endColor.y);
		finalColor.z = (1.0f - ratio) * startColor.z + (ratio * endColor.z);
		finalColor.w = 1.0f - ratio;

		// Return the resulting lerped color
		return finalColor;
	}

	float ParticleSystem::lerp(float start, float end, float ratio)
	{
		return (1.0f - ratio) * start + (ratio * end);
	}

	void ParticleSystem::initialize()
	{
		if (mIsPlaying)
		{
			setInitialData();
		}

		createParticleBuffer();
	}

	void ParticleSystem::update(double deltaTime)
	{
		mLock.lock(LockState::Write);

		if (mCurrentTime >= mDuration && mIsPlaying && !mIsLooping)
		{
			mCurrentGravity = 0.0f;
			stop();
			mLock.unlock(LockState::Write);
			return;
		}

		if (mIsPlaying == false)
		{
			mLock.unlock(LockState::Write);
			return;
		}

		mCurrentTime += deltaTime;

		bool allowEmit = (mIsLooping || mCurrentTime < mDuration);

		mCurrentEmission = runEmission(deltaTime);

		for (int i = 0; i < mParticleData.size(); i++)
		{
			mParticleData[i].lifeTime -= static_cast<float>(deltaTime);

			mCurrentGravity = lerp(0.0f, mGravity, (1.0f - mParticleData[i].lifeTime / mParticleData[i].startLifetime));

			if (mParticleData[i].lifeTime <= 0.0f && mParticleData[i].active)
			{
				mParticleData[i].active = false;
				mParticleData[i].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			}
			else if (mParticleData[i].active)
			{
				mParticleData[i].position.x += mParticleData[i].velocity.x * static_cast<float>(deltaTime);
				mParticleData[i].position.y += mParticleData[i].velocity.y * static_cast<float>(deltaTime);
				mParticleData[i].position.z += mParticleData[i].velocity.z * static_cast<float>(deltaTime);

				if (mGravityEnabled)
					mParticleData[i].position.y -= mCurrentGravity * static_cast<float>(deltaTime);

				float ratio = 1.0f - mParticleData[i].lifeTime / mParticleData[i].startLifetime;

				mParticleData[i].color = colorLerp(mStartColor, mEndColor, ratio);
			}

			if (mParticleData[i].active == false && mCurrentEmission != 0 && allowEmit)
			{
				createParticle(mParticleData[i]);
				mCurrentEmission--;
			}
		}

		mLock.unlock(LockState::Write);
	}

	void ParticleSystem::onDestroy()
	{
		RenderManager::getInstance().destroyBuffer(mParticleBuffer);
	}
}