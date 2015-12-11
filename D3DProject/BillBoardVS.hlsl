struct VS_IN
{
	float4 inPos  : POSITION;
	float2 inSize : SIZE;
};

struct VS_OUT
{
	float4 outPos    : POSITION;
	float2 outSize   : SIZE;
};

VS_OUT main(VS_IN vertexIn)
{
	VS_OUT toGS = (VS_OUT)0;

	//Pass-through to GS
	toGS.outPos = vertexIn.inPos;
	toGS.outSize = vertexIn.inSize;

	return toGS;
}