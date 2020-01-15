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

	struct Particle
	{
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 velocity;
		DirectX::XMFLOAT3 color;
		float lifeTime;
		float startLifetime;
		bool active;

		Particle()
		{
			origin = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			position = origin;
			velocity = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
			lifeTime = 5.0f;
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
		Left,
		Center,
		Right
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
}