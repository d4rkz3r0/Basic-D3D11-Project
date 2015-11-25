#pragma pack_matrix( row_major )

struct INPUT_PIXEL
{
	float4 coordinateOut : SV_POSITION;
	float4 uvOut : TEXCOORD;
	float4 normOut : NORMAL;
};

cbuffer textureOffsets : register(b0)
{
	float textureOffset;
	float padding1;
	float padding2;
	float padding3;
}

texture2D cubeTexture : register(t0);
SamplerState SSO : register(s0);


float4 main(INPUT_PIXEL pixelFragment) : SV_TARGET
{
	//BGRA - Given - Header File
	//RGBA - Need  - Render Target DXGI Format
	//GRAB - Works - IDK Why

	//Limit Sample Area to 128px Wide
	float textureU = pixelFragment.uvOut.x / 4;
	float textureV = pixelFragment.uvOut.y;


	float4 textureColor = cubeTexture.Sample(SSO, float2(textureU + textureOffset, textureV));

	float4 reArrangedColor = float4(textureColor.g, textureColor.r, textureColor.a, textureColor.b);
	//float4 reArrangedColor = float4(textureColor.r, textureColor.g, textureColor.b, textureColor.a);

	return textureColor.wxyz;
}

