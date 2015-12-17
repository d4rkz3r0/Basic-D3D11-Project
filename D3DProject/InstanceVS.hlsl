#pragma pack_matrix(row_major)

cbuffer perObjConstBuffer : register(b0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorldMatrix;
};

cbuffer instanceRotationBuffer : register(b1)
{
	float4x4 mInstanceRotationMX;
}

struct VS_IN
{
	float3 inPosL : POSITION;
	float3 inNorm : NORMAL;
	float2 inUVs  : TEXCOORD;
	float4x4 instWorld : IWORLD;
	float4 instColor : ICOLOR;
	uint InstanceId: SV_InstanceID;

};

struct VS_OUT
{
	float4 outPosH : SV_POSITION;
	float4 outPosW : POSITION;
	float3 outNorm : NORMAL;
	float2 outUVs  : TEXCOORD;
	float4 outColor: COLOR;
};


VS_OUT main(VS_IN vertexIn)
{
	//Init
	VS_OUT vertexOut = (VS_OUT)0;

	//Transformations
	vertexOut.outPosW = mul(float4(vertexIn.inPosL, 1.0f), vertexIn.instWorld);
	vertexOut.outPosH = mul(vertexOut.outPosW, mWorldViewProj);
	vertexOut.outColor = vertexIn.instColor;

	//Pass-throughs
	vertexOut.outNorm = vertexIn.inNorm;
	vertexOut.outUVs = vertexIn.inUVs;
	vertexOut.outColor = vertexIn.instColor;

	return vertexOut;
}