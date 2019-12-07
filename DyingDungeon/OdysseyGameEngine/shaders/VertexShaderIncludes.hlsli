#ifdef __cplusplus
#pragma once
#define cbuffer struct
#define matrix DirectX::XMFLOAT4X4
#endif

cbuffer ShaderMatrix
{
	matrix world;
	matrix view;
	matrix proj;
	matrix worldViewProj;
	matrix lightViewProj;
};