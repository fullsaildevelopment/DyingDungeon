#include "DebugManager.h"
#include "Buffer.h"
#include "AABB.h"
#include "RenderDevice.h"

namespace Odyssey
{
	DebugManager& DebugManager::getInstance()
	{
		static DebugManager instance;
		return instance;
	}

	void DebugManager::initialize(RenderDevice& renderDevice)
	{
		mNumVertices = 0;

		mVertexBuffer = renderDevice.createBuffer(BufferBindFlag::VertexBuffer, size_t(MAX_LINES),
			static_cast<UINT>(sizeof(ColoredVertex)), nullptr);
	}

	void DebugManager::addLine(DirectX::XMFLOAT3 positionA, DirectX::XMFLOAT3 positionB, DirectX::XMFLOAT3 colorA, DirectX::XMFLOAT3 colorB)
	{
		// Add the first vertex position and color to the list
		mVertexList[mNumVertices].position = positionA;
		mVertexList[mNumVertices].color = colorA;
		// Incremented the number of vertices
		mNumVertices++;
		// Add the second vertex position and color to the list
		mVertexList[mNumVertices].position = positionB;
		mVertexList[mNumVertices].color = colorB;
		// Incremented the number of vertices
		mNumVertices++;
	}

	void DebugManager::addSphere(DirectX::XMFLOAT3 center, float radius, DirectX::XMFLOAT3 color)
	{
		drawRing(DirectX::XMFLOAT4(center.x, center.y, center.z, 1.0f), { radius,0,0, 0.0f }, { 0,0,radius, 0.0f }, color);
		drawRing(DirectX::XMFLOAT4(center.x, center.y, center.z, 1.0f), { radius,0,0, 0.0f }, { 0,radius,0, 0.0f }, color);
		drawRing(DirectX::XMFLOAT4(center.x, center.y, center.z, 1.0f), { 0,radius,0, 0.0f }, { 0,0,radius, 0.0f }, color);
	}

	void DebugManager::addAABB(DirectX::XMFLOAT3& extents, DirectX::XMFLOAT3& center, DirectX::XMFLOAT3 color)
	{
		using namespace DirectX;

		// Far upper right
		XMVECTOR fUR = XMLoadFloat3(&center) + XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y + XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Far upper left
		XMVECTOR fUL = XMLoadFloat3(&center) - XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y + XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Far lower left
		XMVECTOR fLL = XMLoadFloat3(&center) - XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x - XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y + XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Far lower right
		XMVECTOR fLR = XMLoadFloat3(&center) + XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x - XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y + XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Near upper right
		XMVECTOR nUR = XMLoadFloat3(&center) + XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y - XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Near upper left
		XMVECTOR nUL = XMLoadFloat3(&center) - XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y - XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Near lower left
		XMVECTOR nLL = XMLoadFloat3(&center) - XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x - XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y - XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		// Near lower right
		XMVECTOR nLR = XMLoadFloat3(&center) + XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * extents.x - XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * extents.y - XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * extents.z;

		XMFLOAT3 corners[8];

		XMStoreFloat3(&corners[0], nUL);
		XMStoreFloat3(&corners[1], nUR);
		XMStoreFloat3(&corners[2], nLR);
		XMStoreFloat3(&corners[3], nLL);
		XMStoreFloat3(&corners[4], fUL);
		XMStoreFloat3(&corners[5], fUR);
		XMStoreFloat3(&corners[6], fLR);
		XMStoreFloat3(&corners[7], fLL);

		for (int i = 0; i < 4; i++)
		{
			if (i < 3)
			{
				// Draw the front face line
				addLine(corners[i], corners[i + 1], color, color);
				// Draw the corresponding back face line
				addLine(corners[i + 4], corners[i + 5], color, color);
			}
			else
			{
				// Draw from the last front face coord to the first
				addLine(corners[i], corners[0], color, color);

				// Draw from the last back face coord to the first
				addLine(corners[i + 4], corners[4], color, color);
			}
			// Draw the lines connecting the two faces
			addLine(corners[i], corners[i + 4], color, color);
		}
	}

	void DebugManager::clearBuffer()
	{
		mNumVertices = 0;
		ZeroMemory(&mVertexList, sizeof(mVertexList));
	}

	unsigned int DebugManager::getNumberOfVertices()
	{
		return static_cast<unsigned int>(mNumVertices);
	}

	void DebugManager::bind()
	{
		mVertexBuffer->updateData(&mVertexList);
		mVertexBuffer->bind();
	}

	void DebugManager::drawRing(DirectX::XMFLOAT4 center, DirectX::XMFLOAT4 majorAxis, DirectX::XMFLOAT4 minorAxis, DirectX::XMFLOAT3 color)
	{
		using namespace DirectX;
		const size_t ringSegments = 32;
		ColoredVertex verts[ringSegments + 1];

		float angleDelta = XM_2PI / float(ringSegments);

		XMVECTOR cosDelta = XMVectorReplicate(cosf(angleDelta));
		XMVECTOR sinDelta = XMVectorReplicate(sinf(angleDelta));
		XMVECTOR incrementalSin = XMVectorZero();

		const XMVECTORF32 initialCos = { 1,1,1,1 };
		XMVECTOR incrementalCos = initialCos.v;

		for (size_t i = 0; i < ringSegments; i++)
		{
			XMVECTOR pos = XMVectorMultiplyAdd(XMLoadFloat4(&majorAxis), incrementalCos, XMLoadFloat4(&center));
			pos = XMVectorMultiplyAdd(XMLoadFloat4(&minorAxis), incrementalSin, pos);
			XMStoreFloat3(&verts[i].position, pos);
			verts[i].color = color;
			XMVECTOR newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
			XMVECTOR newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
			incrementalCos = newCos;
			incrementalSin = newSin;
		}

		verts[ringSegments] = verts[0];

		for (int i = 0; i < ringSegments; i++)
		{
			mVertexList[mNumVertices] = verts[i];
			mNumVertices++;
			mVertexList[mNumVertices] = verts[i + 1];
			mNumVertices++;
		}
	}
}