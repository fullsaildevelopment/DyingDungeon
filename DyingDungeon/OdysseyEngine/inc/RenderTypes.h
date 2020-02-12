#pragma once
namespace Odyssey
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
		DirectX::XMFLOAT4 jointWeights;
		int jointIndices[4];
		DirectX::XMFLOAT2 tex;
	};

	struct ShaderLight
	{
		DirectX::XMFLOAT4 worldPosition;
		//------------------------------- ( 16 bytes )
		DirectX::XMFLOAT4 worldDirection;
		//------------------------------- ( 16 bytes )
		DirectX::XMFLOAT4 color;
		//------------------------------- ( 16 bytes )
		UINT lightType;
		float intensity;
		float range;
		float cone;
		//------------------------------- ( 16 bytes )
	}; //------------------------------- ( 4 x 16 bytes  = 64 bytes)

	struct Particle
	{
		DirectX::XMFLOAT4 color;
		// 12 bytes
		DirectX::XMFLOAT3 position;
		float lifeTime;
		// 12 bytes
		DirectX::XMFLOAT3 velocity;
		float startLifetime;
		// 12 bytes
		float size;
		bool active;

		Particle()
		{
			position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			velocity = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			lifeTime = 5.0f;
			startLifetime = lifeTime;
			size = 1.0f;
			active = false;
		}
	};

	struct PerFrameBuffer
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 viewProj;
		DirectX::XMFLOAT4X4 lightViewProj;
	};

	struct PerObjectBuffer
	{
		DirectX::XMFLOAT4X4 world;
	};

	enum class TextureType
	{
		Render = -1,
		Diffuse = 0,
		Emissive = 1,
		Specular = 2,
		NormalMap = 3,
		Skybox = 7,
		Shadow = 8
	};
	enum class TextureBindFlag
	{
		ShaderResource = 0,
		RenderTargetView = 1,
		DepthStencilView = 2
	};
	enum class Blend
	{
		BLEND_ZERO = 1,
		BLEND_ONE = 2,
		BLEND_SRC_COLOR = 3,
		BLEND_INV_SRC_COLOR = 4,
		BLEND_SRC_ALPHA = 5,
		BLEND_INV_SRC_ALPHA = 6,
		BLEND_DEST_ALPHA = 7,
		BLEND_INV_DEST_ALPHA = 8,
		BLEND_DEST_COLOR = 9,
		BLEND_INV_DEST_COLOR = 10,
		BLEND_SRC_ALPHA_SAT = 11,
		BLEND_BLEND_FACTOR = 14,
		BLEND_INV_BLEND_FACTOR = 15,
		BLEND_SRC1_COLOR = 16,
		BLEND_INV_SRC1_COLOR = 17,
		BLEND_SRC1_ALPHA = 18,
		BLEND_INV_SRC1_ALPHA = 19
	};
	enum class BlendOperation
	{
		BLEND_OP_ADD = 1,
		BLEND_OP_SUBTRACT = 2,
		BLEND_OP_REV_SUBTRACT = 3,
		BLEND_OP_MIN = 4,
		BLEND_OP_MAX = 5
	};
	struct BlendDesc
	{
		bool mBlendEnable;
		Blend mSrcBlend;
		Blend mDestBlend;
		Blend mSrcAlphaBlend;
		Blend mDestAlphaBlend;
		BlendOperation mBlendOp;
		BlendOperation mAlphaBlendOp;
		bool mAlphaToCoverage;
		bool mIndependentBlendEnable;
	};
	enum class ComparisonFunc
	{
		COMPARISON_NEVER = 0,
		COMPARISON_ALWAYS = 1,
		COMPARISON_EQUAL = 2,
		COMPARISON_NOT_EQUAL = 3,
		COMPARISON_GREATER = 4,
		COMPARISON_GREATER_EQUAL = 5,
		COMPARISON_LESS = 6,
		COMPARISON_LESS_EQUAL = 7
	};
	enum class Topology
	{
		PointList = 0,
		LineList = 1,
		LineStrip = 2,
		TriangleList = 3
	};
	enum class CullMode
	{
		CULL_NONE = 0,
		CULL_BACK = 1,
		CULL_FRONT = 2
	};
	enum class FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME = 1
	};
	enum class ShaderType
	{
		PixelShader = 0,
		VertexShader = 1,
		GeometryShader = 2,
		ComputeShader = 3,
	};
	enum class BufferBindFlag
	{
		ConstantBuffer = 0,
		IndexBuffer = 1,
		VertexBuffer = 2,
		StructuredBuffer = 3
	};

	enum class TextAlignment
	{
		Left,
		Center,
		Right
	};

	enum class ParagraphAlignment
	{
		Top,
		Center,
		Bottom
	};

	struct TextProperties
	{
		TextAlignment textAlignment;
		ParagraphAlignment paragraphAlignment;
		std::wstring fontName;
		float fontSize;
		bool bold;
		bool italic;
	};
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
		bool loop;
		unsigned int maxFrames;
		unsigned int framesPerSecond;
		std::vector<Keyframe> keyframes;
		Keyframe currentFrame;
		double duration;
		double currentTime;
		unsigned int prevFrame;
		unsigned int nextFrame;
		float progress;
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

	enum class LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2
	};

	class AABB;
	class AnimatorDX11;
	class Entity;
	class MeshRenderer;
	class Transform;

	struct RenderObject
	{
		Entity* entity;
		AABB* aabb;
		MeshRenderer* meshRenderer;
		Transform* transform;
		AnimatorDX11* animator;
	};

	class ParticleSystem;
	struct VFXObject
	{
		ParticleSystem* system;
		Transform* transform;
	};

	class Entity;
	class UICanvas;
	class UIElement;
	struct CanvasObject
	{
		Entity* entity;
		UICanvas* canvas;
		std::vector<UIElement*> elements;
	};

	class Camera;
	class Frustum;
	class Light;

	struct RenderPackage
	{
		// All Passes
		Camera* camera;
		Frustum* frustum;
		DirectX::XMFLOAT3 cameraPosition;
		// Skybox Pass
		RenderObject skybox;
		// Shadow Pass
		Light* shadowLight;
		DirectX::XMFLOAT3 sceneCenter;
		float sceneRadius;
		// Shadow and Opaque Pass
		std::vector<RenderObject> renderObjects;
		// Transparent Pass
		std::vector<VFXObject> vfxObjects;
		// Sprite2D Pass
		std::vector<CanvasObject> canvasObjects;
		// Lighting
		std::vector<Light*> sceneLights;

		RenderPackage()
		{
			camera = nullptr;
			frustum = nullptr;
			shadowLight = nullptr;
		}
	};
}