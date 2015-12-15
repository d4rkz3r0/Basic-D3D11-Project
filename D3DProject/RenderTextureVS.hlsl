#pragma pack_matrix(row_major)

cbuffer perObjConstBuffer : register(b0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorldMatrix;
};

struct VS_IN
{
	float3 inPosL : POSITION;
	float3 inNorm : NORMAL;
	float2 inUVs  : TEXCOORD;
};

struct VS_OUT
{
	float4 outPosH : SV_POSITION;
	float3 outNorm : NORMAL;
	float2 outUVs  : TEXCOORD;
};


VS_OUT main(VS_IN vertexIn)
{
	//Init
	VS_OUT vertexOut = (VS_OUT)0;

	//Transformations
	//Local Position, Local->Clip
	vertexOut.outPosH = mul(float4(vertexIn.inPosL, 1.0f), mWorldViewProj);


	//Pass-throughs
	vertexOut.outNorm = vertexIn.inNorm;
	vertexOut.outUVs = vertexIn.inUVs;

	return vertexOut;
}