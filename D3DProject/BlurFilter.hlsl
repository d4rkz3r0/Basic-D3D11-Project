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

	float4 normTextureColor = mColorMap.Sample(mSampler, fragmentIn.inUVs);
	float4 rightTextureColor = mColorMap.Sample(mSampler, (fragmentIn.inUVs.xy + (0.01f)));
	float4 leftTextureColor = mColorMap.Sample(mSampler, (fragmentIn.inUVs.xy - (0.01f)));

	finalColor = normTextureColor;
	finalColor += rightTextureColor;
	finalColor += leftTextureColor;
	finalColor = finalColor / 3;

	return finalColor;
}