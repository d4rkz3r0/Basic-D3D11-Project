cbuffer ShaderMiscInfo : register(b10)
{
	float4 dataBlock1; //UVx, UVy
	float4 dataBlock2; //Camera's World Space Position
	float4 dataBlock3; //Camera's Current Up Vector
	float4 dataBlock4;
};

struct PS_IN
{
	float4 inPosW : SV_Position;
	float2 inUV : TEXCOORD;
};

Texture2D mColorMap   : register(t0);
SamplerState mSampler : register(s0);

float4 main(PS_IN fragmentIn) : SV_TARGET
{
	float textureU = fragmentIn.inUV.x / 2.0f;

	return mColorMap.Sample(mSampler, float2(textureU + dataBlock1.x, fragmentIn.inUV.y * dataBlock1.y));
}