struct VS_Input
{
	uint id : SV_VertexID;
};

struct GS_Input
{
	uint id : PRIMITIVE_ID;
};

GS_Input main(uint id : SV_VertexID)
{
	GS_Input gi;
	gi.id = id;
	return gi;
}