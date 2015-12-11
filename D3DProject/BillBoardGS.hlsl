#pragma pack_matrix(row_major)

static const float2 QuadUVs[4] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f), 
	float2(1.0f, 1.0f), 
	float2(1.0f, 0.0f) 
};

cbuffer perObjConstBuffer : register(b0)
{
	float4x4 mWorldViewProj;
	float4x4 mWorldMatrix;
};

cbuffer ShaderMiscInfo : register(b10)
{
	float4 dataBlock1; //UV Multiplier
	float4 dataBlock2; //Camera's World Space Position
	float4 dataBlock3; //Camera's Up Vector
	float4 dataBlock4;
};

struct GS_IN
{
	float4 inPosW  : POSITION;
	float2 inSizeW : SIZE;
};

struct GS_OUT
{
	float4 outPosW : SV_Position;
	float2 outUV : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GS_IN inStream[1],
	inout TriangleStream<GS_OUT> triStream)
{
	//Init
	GS_OUT geoOut = (GS_OUT)0;

	//Calc Quads Orientation Vecs
	float3 direction = dataBlock2.xyz - inStream[0].inPosW.xyz;
	float3 right = cross(normalize(direction), dataBlock3.xyz);

	float2 halfSize = inStream[0].inSizeW / 2.0f;
	float3 offsetX = halfSize.x * right;
	float3 offsetY = halfSize.y * dataBlock3.xyz;

	float4 vertices[4];
	vertices[0] = float4(inStream[0].inPosW.xyz + offsetX - offsetY, 1.0f); // Bot Left
	vertices[1] = float4(inStream[0].inPosW.xyz + offsetX + offsetY, 1.0f); // Top Left	
	vertices[2] = float4(inStream[0].inPosW.xyz - offsetX - offsetY, 1.0f); // Bot Right
	vertices[3] = float4(inStream[0].inPosW.xyz - offsetX + offsetY, 1.0f); // Top Right

	for (int i = 0; i < 4; i++)
	{
		//Do Vertex Shaders Job
		geoOut.outPosW = mul(vertices[i], mWorldViewProj);
		geoOut.outUV = QuadUVs[i];

		triStream.Append(geoOut);
	}
}