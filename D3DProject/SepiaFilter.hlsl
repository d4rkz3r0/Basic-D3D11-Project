struct PS_IN
{
	float4 inPosH : SV_POSITION;
	float3 inNorm : NORMAL;
	float2 inUVs  : TEXCOORD;
};

Texture2D mColorMap   : register(t0);
SamplerState mSampler : register(s0);

float4 main(PS_IN fragmentIn) : SV_TARGET
{
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 textureColor = mColorMap.Sample(mSampler, fragmentIn.inUVs);

	finalColor += textureColor;

	finalColor.r = (textureColor.r * 0.393) + (textureColor.g * 0.769) + (textureColor.b * 0.189);
	finalColor.g = (textureColor.r * 0.349) + (textureColor.g * 0.686) + (textureColor.b * 0.168);
	finalColor.b = (textureColor.r * 0.272) + (textureColor.g * 0.534) + (textureColor.b * 0.131);

	return finalColor;
}