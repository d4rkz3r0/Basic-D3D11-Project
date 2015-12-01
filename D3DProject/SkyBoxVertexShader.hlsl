#pragma pack_matrix(row_major)

cbuffer cbPerObject : register (b0)
{
	float4x4 WVP;
};

struct VS_IN
{
	float3 inPosL : POSITION;
};

struct VS_OUT
{
	float4 outPosH : SV_POSITION;
	float3 outPosL : POSITION;
};


VS_OUT main(VS_IN vertexIn)
{
	VS_OUT vertexOut = (VS_OUT)0;

	//Perspective Divide on z component will now always equal 1.0f;
	vertexOut.outPosH = mul(float4(vertexIn.inPosL, 1.0f), WVP).xyww;
	//Send local position to PS for sampling from the Texture
	vertexOut.outPosL = vertexIn.inPosL;

	return vertexOut;
}