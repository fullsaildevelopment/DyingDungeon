#pragma once
#include "EngineIncludes.h"
#include "Buffer.h"

namespace Odyssey
{
#define MAX_LINES 100000
	struct ColoredVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
	};

	class AABB;
	class RenderDevice;

	class DebugManager
	{
	public:
		static DebugManager& getInstance();
		~DebugManager() { }
	private:
		DebugManager() { }
	public:
		void initialize(RenderDevice& renderDevice);
		void addLine(DirectX::XMFLOAT3 positionA, DirectX::XMFLOAT3 positionB, DirectX::XMFLOAT3 colorA, DirectX::XMFLOAT3 colorB);
		void addSphere(DirectX::XMFLOAT3 center, float radius, DirectX::XMFLOAT3 color);
		void addAABB(DirectX::XMFLOAT3& extents, DirectX::XMFLOAT3& center, DirectX::XMFLOAT3 color);
		void addText(std::string text);
		void clearBuffer();
		unsigned int getNumberOfVertices();
		void bind();
	private:
		void setupDebugConsole(short bufferWidth, short bufferHeight, short windowWidth, short windowHeight);
		void drawRing(DirectX::XMFLOAT4 center, DirectX::XMFLOAT4 majorAxis, DirectX::XMFLOAT4 minorAxis, DirectX::XMFLOAT3 color);
	private:
		std::array<ColoredVertex, MAX_LINES> mVertexList;
		size_t mNumVertices;
		std::shared_ptr<Buffer> mVertexBuffer;
	};
}