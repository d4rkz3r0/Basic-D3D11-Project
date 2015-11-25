#pragma pack_matrix( row_major )

struct INPUT_PIXEL
{
	float4 coordinateOut : SV_POSITION;
};

float4 main(INPUT_PIXEL pixelFragment) : SV_TARGET
{
	float4 greenColor = float4(0.0f, 1.0f, 0.0f, 1.0f);

	return greenColor;
}