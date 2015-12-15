cbuffer ShaderMiscInfo : register(b10)
{
	float4 dataBlock1; //UVx, UVy
	float4 dataBlock2; //Camera's World Space Position
	float4 dataBlock3; //Camera's Current Up Vector
	float4 dataBlock4;
};

struct PS_IN
{
	float4 inPosH : SV_POSITION;
	float4 inPosW : POSITION;
	float3 inNorm : NORMAL;
	float2 inUVs  : TEXCOORD;
	float4 inColor: COLOR;
};

Texture2D mColorMap   : register(t0);
SamplerState mSampler : register(s0);

float4 main(PS_IN fragmentIn) : SV_TARGET
{
	float4 finalColor = fragmentIn.inColor; //Instanced Color Data
	float4 textureColor = mColorMap.Sample(mSampler, fragmentIn.inUVs);
	finalColor += textureColor;
	return finalColor;
}