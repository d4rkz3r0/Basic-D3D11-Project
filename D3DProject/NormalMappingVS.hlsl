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
	float3 inTang : TANGENT;
	float2 inUVs  : TEXCOORD;
};

struct VS_OUT
{
	float4 outPosH : SV_POSITION;
	float4 outPosW : POSITION;
	float3 outNorm : NORMAL;
	float3 outTang : TANGENT;
	float2 outUVs  : TEXCOORD;
};


VS_OUT main(VS_IN vertexIn)
{
	//Init
	VS_OUT vertexOut = (VS_OUT)0;

	//Transformations
	//Local Position, Local->Clip
	vertexOut.outPosH = mul(float4(vertexIn.inPosL, 1.0f), mWorldViewProj);
	//Local Position, Local->World
	vertexOut.outPosW = mul(float4(vertexIn.inPosL, 1.0f), mWorldMatrix);
	//Local Normal Vector, Local->World
	vertexOut.outNorm = mul(vertexIn.inNorm, (float3x3)mWorldMatrix);
	//Local Tangent Vector, Local->World
	vertexOut.outTang = mul(vertexIn.inTang, (float3x3)mWorldMatrix);

	//Pass-throughs
	vertexOut.outUVs = vertexIn.inUVs;

	return vertexOut;
}