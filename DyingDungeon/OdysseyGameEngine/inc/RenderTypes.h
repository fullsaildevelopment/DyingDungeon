#pragma once
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
struct ShaderMatrix
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMFLOAT4X4 lightViewProj;
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