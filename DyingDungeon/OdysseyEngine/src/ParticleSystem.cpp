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
#include "Transform.h"
#include "Entity.h"

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
		mMinSizeLifetime = copy.mMinSizeLifetime;
		mMaxSizeLifetime = copy.mMaxSizeLifetime;
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
		setDefaultEmitterData();
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
		// Set the start and end color
		mStartColor = startColor;
		mEndColor = endColor;

		// Set the fading start color and end color
		DirectX::XMFLOAT4 color = colorLerp(mStartColor, mEndColor, 0.8f);
		mFadeStart = { color.x, color.y, color.z };
		mFadeEnd = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	void ParticleSystem::setLifetime(float min, float max)
	{
		// Set the min and max lifetime
		mMinLife = min;
		mMaxLife = max;
	}

	void ParticleSystem::setParticleCount(int startingCount, int maxCount)
	{
		// Set the particle counts and resize buffer to the max count
		mStartCount = startingCount;
		mMaxCount = maxCount;
		mParticleData.resize(mMaxCount);
	}

	void ParticleSystem::setEmissionOverLifetime(int emissionRate)
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

	void ParticleSystem::setSizeOverLifetime(float min, float max)
	{
		mMinSizeLifetime = min;
		mMaxSizeLifetime = max;
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

	ParticleSystemShape* ParticleSystem::getShape()
	{
		return &mShape;
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
		EventManager::getInstance().publish(new DestroyEntityEvent(mEntity));
	}

	void ParticleSystem::setDefaultEmitterData()
	{
		mStartCount = 10;
		mMaxCount = 100;
		mEmissionRate = 10;
		mShape = SpherePS();
		mStartColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mEndColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mStartColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mEndColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mMinSpeed = 0.0f;
		mMaxSpeed = 1.0f;
		mMinSize = 0.25f;
		mMaxSize = 1.0f;
		mMinSizeLifetime = 0.0f;
		mMaxSizeLifetime = 0.0f;
		mMinLife = 0.0f;
		mMaxLife = 1.0f;
		mGravity = 0.0f;
		mGravityEnabled = false;
		mIsPlaying = true;
		time = 0.0;
		totalTime = 1.0;
		numSpawn = mEmissionRate;
		mCurrentEmission = 0;
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
		// Seed the generator
		std::mt19937 generator(rnd());

		// Generate a random lifetime and speed
		std::uniform_real_distribution<float> lf(mMinLife, mMaxLife);
		std::uniform_real_distribution<float> spd(mMinSpeed, mMaxSpeed);

		// Generate a random size and size over lifetime
		std::uniform_real_distribution<float> size(mMinSize, mMaxSize);
		std::uniform_real_distribution<float> sizeLifetime(mMinSizeLifetime, mMaxSizeLifetime);
		float speed = spd(generator);

		// Assign the starting color
		particle.color = colorLerp(mStartColor, mEndColor, 0.0f);

		particle.position = generateParticlePosition(generator, mShape.isCircle);
		particle.velocity = generateParticleVelocity(generator, particle.position, speed, mShape.isSphere);
		// Assign the starting position in reference to the emitter's position
		particle.size = size(generator);
		particle.sizeLifetime = sizeLifetime(generator);
		particle.active = true;
		particle.lifeTime = lf(generator);
		particle.startLifetime = particle.lifeTime;
	}

	DirectX::XMFLOAT3 ParticleSystem::generateParticlePosition(std::mt19937& generator, bool isCircle)
	{
		// Get the emitter's position
		DirectX::XMFLOAT3 position = mTransform->getPosition();
		if (isCircle)
		{

			// Get the transform's current axis
			DirectX::XMFLOAT3 fwd = mTransform->getForward();
			DirectX::XMFLOAT3 up = mTransform->getUp();
			DirectX::XMFLOAT3 right = mTransform->getRight();
			// Generate a random number 0 - 1
			std::uniform_real_distribution<float> angle(0.0f, 1.0f);

			// Calculate the random angle
			float fltAngle = angle(generator) * DirectX::g_XMTwoPi.f[0];

			// Calculate a random point along a unit circle
			DirectX::XMFLOAT3 rndCircle = { (std::cosf(fltAngle) * mShape.mRadius.x), 0.0f, (std::sinf(fltAngle) * mShape.mRadius.z) };

			position.x += right.x * rndCircle.x + up.x * 0.0f + fwd.x * rndCircle.z;
			position.y += right.y * rndCircle.x + up.y * 0.0f + fwd.y * rndCircle.z;
			position.z += right.z * rndCircle.x + up.z * 0.0f + fwd.z * rndCircle.z;
			// Scale the unit circle point with the radius
			//position = { position.x + rndCircle.x, position.y, position.z + rndCircle.z };

		}
		else
		{
			// Get the transform's current axis
			DirectX::XMFLOAT3 fwd = mTransform->getForward();
			DirectX::XMFLOAT3 up = mTransform->getUp();
			DirectX::XMFLOAT3 right = mTransform->getRight();

			std::uniform_real_distribution<float> xPosition(-1.0f * mShape.mRadius.x + mShape.center.x, mShape.center.x + mShape.mRadius.x);
			std::uniform_real_distribution<float> yPosition(-1.0f * mShape.mRadius.y + mShape.center.y, mShape.center.y + mShape.mRadius.y);
			std::uniform_real_distribution<float> zPosition(-1.0f * mShape.mRadius.z + mShape.center.z, mShape.center.z + mShape.mRadius.z);
			float xPos = xPosition(generator);
			float yPos = yPosition(generator);
			float zPos = zPosition(generator);
			position.x += right.x * xPos + up.x * yPos + fwd.x * zPos;
			position.y += right.y * xPos + up.y * yPos + fwd.y * zPos;
			position.z += right.z * xPos + up.z * yPos + fwd.z * zPos;
		}
		return position;
	}

	DirectX::XMFLOAT3 ParticleSystem::generateParticleVelocity(std::mt19937& generator, DirectX::XMFLOAT3 particlePosition, float speed, bool isSphere)
	{
		if (isSphere == false)
		{
			// Get the transform's current axis
			DirectX::XMFLOAT3 fwd = mTransform->getForward();
			DirectX::XMFLOAT3 up = mTransform->getUp();
			DirectX::XMFLOAT3 right = mTransform->getRight();

			// Seed the random direction
			std::uniform_real_distribution<float> xDirection(mShape.minXDirection, mShape.maxXDirection);
			std::uniform_real_distribution<float> yDirection(mShape.minYDirection, mShape.maxYDirection);
			std::uniform_real_distribution<float> zDirection(mShape.minZDirection, mShape.maxZDirection);

			// Generate the random directional values
			float xVel = xDirection(generator);
			float yVel = yDirection(generator);
			float zVel = zDirection(generator);

			// Calculate a velocity in reference to the emitter orientation
			DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			velocity.x = right.x * xVel + up.x * yVel + fwd.x * zVel;
			velocity.y = right.y * xVel + up.y * yVel + fwd.y * zVel;
			velocity.z = right.z * xVel + up.z * yVel + fwd.z * zVel;

			// Normalize
			DirectX::XMStoreFloat3(&velocity, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)));

			// Apply the speed and return the velocity
			velocity.x *= speed;
			velocity.y *= speed;
			velocity.z *= speed;
			return velocity;
		}
		else
		{
			// Get the particle's position
			DirectX::XMFLOAT3 position = mTransform->getPosition();

			// Generate a random velocity
			DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			velocity.x = particlePosition.x - position.x;
			velocity.y = particlePosition.y - position.y;
			velocity.z = particlePosition.z - position.z;

			// Normalize
			DirectX::XMStoreFloat3(&velocity, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)));

			// Apply speed in the direction
			velocity.x *= speed;
			velocity.y *= speed;
			velocity.z *= speed;

			return velocity;
		}
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
		// Store a reference to the entity's transform
		mTransform = mEntity->getComponent<Transform>();

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

				if (ratio >= 0.8f)
				{
					mParticleData[i].color = colorLerp(mFadeStart, mFadeEnd, (ratio - 0.8f) / 0.2f);
				}
				else
				{
					mParticleData[i].color = colorLerp(mStartColor, mEndColor, ratio);
				}

				mParticleData[i].size += mParticleData[i].sizeLifetime * deltaTime;
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