#pragma pack_matrix( row_major )

struct INPUT_VERTEX
{
	float4 coordinate : GPOSITION;
};

struct OUTPUT_VERTEX
{
	float4 coordinateOut : SV_POSITION;
};

cbuffer OBJECT : register(b1)
{
	float4x4 WVP;
}

OUTPUT_VERTEX main(INPUT_VERTEX fromInputAssembler)
{
	//Initialization
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	//Position
	sendToRasterizer.coordinateOut = fromInputAssembler.coordinate;
	sendToRasterizer.coordinateOut = mul(sendToRasterizer.coordinateOut, WVP);

	return sendToRasterizer;
}