#pragma pack_matrix(row_major)

cbuffer perObjConstBuffer : register(b0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorldMatrix;
};

struct VS_IN
{
	float3 inPosL : POSITION;
};

struct VS_OUT
{
	float4 outPosH : SV_POSITION;
	float3 outPosL : TEXCOORD;
};


VS_OUT main(VS_IN vertexIn)
{
	VS_OUT vertexOut = (VS_OUT)0;

	//Perspective Divide on z component will now always equal 1.0f;
	vertexOut.outPosH = mul(float4(vertexIn.inPosL, 1.0f), mWorldViewProj).xyww;
	//Send local position to PS for sampling from the Texture
	vertexOut.outPosL = vertexIn.inPosL;

	return vertexOut;
}