#pragma pack_matrix( row_major )

struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float3 uv : TEXCOORD;
	float3 norm : NORMAL;
};

struct OUTPUT_VERTEX
{
	float4 coordinateOut : SV_POSITION;
	float4 uvOut : TEXCOORD;
	float4 normOut : NORMAL;
};


cbuffer OBJECT : register( b0 )
{
	float4x4 WVP;
}

OUTPUT_VERTEX main( INPUT_VERTEX fromVertexBuffer )
{
	//Initialization
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	//Position
	sendToRasterizer.coordinateOut = float4(fromVertexBuffer.coordinate, 1.0f);
	sendToRasterizer.coordinateOut = mul(sendToRasterizer.coordinateOut, WVP);

	//UVs
	sendToRasterizer.uvOut = float4(fromVertexBuffer.uv, 1.0f);

	//Normals
	sendToRasterizer.normOut = float4(fromVertexBuffer.norm, 1.0f);

	return sendToRasterizer;
}