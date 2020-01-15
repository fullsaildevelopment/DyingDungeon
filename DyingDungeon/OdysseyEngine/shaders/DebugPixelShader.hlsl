struct VSOut
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

float4 main(VSOut input) : SV_TARGET
{
	return input.color;
}