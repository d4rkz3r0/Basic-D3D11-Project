#pragma pack_matrix(row_major)

cbuffer cbPerObject : register (b1)
{
	float4x4 WVP;
};

struct VS_IN
{
	float3 inPosL : POSITION;
	float4 inColor : COLOR;
};

struct VS_OUT
{
	float4 outPosH : SV_POSITION;
	float4 outColor : COLOR;
};


VS_OUT main(VS_IN vertexIn)
{
	VS_OUT vertexOut = (VS_OUT)0;

	vertexOut.outPosH = mul(float4(vertexIn.inPosL, 1.0f), WVP);
	vertexOut.outColor = vertexIn.inColor;

	return vertexOut;
}