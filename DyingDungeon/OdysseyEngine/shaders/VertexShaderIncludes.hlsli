cbuffer PerFrameBuffer
{
	matrix view;
	matrix viewProj;
	matrix lightViewProj;
};

cbuffer PerObjectBuffer
{
	matrix world;
};