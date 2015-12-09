cbuffer DirLightInfo : register(b0)
{
	float4 mDirLightAmbient;
	float4 mDirLightDiffuse;
	float4 mDirLightSpecular;
	float3 mDirLightDirection;
};

cbuffer PointLightInfo : register(b1)
{
	float4 mPointLightAmbient;
	float4 mPointLightDiffuse;
	float4 mPointLightSpecular;
	float3 mPointLightPosition;
	float  mPointLightRange;
	float3 mPointLightAttenuation;
}

cbuffer ShaderMiscInfo : register(b2)
{
	float4 dataBlock1;
	float4 dataBlock2;
	float4 dataBlock3;
	float4 dataBlock4;
};


struct PS_IN
{
	float4 inPosClip   : SV_POSITION;
	float4 inPosWorld  : POSITION;
	float3 inNormVec   : NORMAL;
	float3 inTanVec    : TANGENT;
	float2 inUVs       : TEXCOORD;
};

Texture2D mColorMap   : register(t0);
SamplerState mSampler : register(s0);

float4 main(PS_IN fragmentIn) : SV_TARGET
{
	//Init
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 PointLightFinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 DirectionalLightFinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 pixelNormVec = normalize(fragmentIn.inNormVec);
	float3 PointLight2PixelVec = mPointLightPosition - fragmentIn.inPosWorld.xyz;
	float  PointLight2PixelDistance = length(PointLight2PixelVec);
	float4 textureDiffuse = mColorMap.Sample(mSampler, fragmentIn.inUVs * dataBlock1.x);
	float4 ambientFactor = textureDiffuse * mPointLightAmbient;	//Ambient Color
	float DirectionalLightScalar = dot(mDirLightDirection, pixelNormVec);

	//Directional Color
	DirectionalLightFinalColor += saturate(mDirLightDiffuse * DirectionalLightScalar * textureDiffuse);
	//FC += Directional Result
	finalColor += DirectionalLightFinalColor;

	if (PointLight2PixelDistance > mPointLightRange)
	{
		//FC += Ambient Result
		finalColor += ambientFactor;
		return float4(finalColor.rgb, textureDiffuse.a); //FC = Directional + Ambient
	}
	
	float3 currPointLight2PixelDirection = PointLight2PixelVec / PointLight2PixelDistance;
	float PointLightScalar = dot(currPointLight2PixelDirection, pixelNormVec);
	
	if (PointLightScalar > 0.0f)
	{
		//FC += Point Result
		PointLightFinalColor += (textureDiffuse * mPointLightDiffuse * PointLightScalar);
		PointLightFinalColor /= (mPointLightAttenuation.x + (mPointLightAttenuation.y * PointLight2PixelDistance) + (mPointLightAttenuation.z * pow(PointLight2PixelDistance, 2.0f)));
		finalColor += PointLightFinalColor;
	}

	//Clamp and Return
	//FC += Ambient Result
	finalColor = saturate(finalColor + ambientFactor);
	return float4(finalColor.rgb, textureDiffuse.a); //FC = Directional + Ambient + Point 
}