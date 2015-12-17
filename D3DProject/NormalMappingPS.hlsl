cbuffer DirLightInfo : register(b0)
{
	float4 mDirLightAmbient;
	float4 mDirLightDiffuse;
	float4 mDirLightSpecular;
	float3 mDirLightDirection;
	float padding;
};

cbuffer PointLightInfo : register(b1)
{
	float4 mPointLightAmbient;
	float4 mPointLightDiffuse;
	float4 mPointLightSpecular;
	float3 mPointLightPosition;
	float  mPointLightRange;
	float3 mPointLightAttenuation;
	float padding1;
}

cbuffer ObjectMaterialInfo : register(b3)
{
	float4 mObjectAmbient;
	float4 mObjectDiffuse;
	float4 mObjectSpecular;
	float4 mObjectReflect;
}

cbuffer ShaderMiscInfo : register(b10)
{
	float4 dataBlock1; //UV Multiplier
	float4 dataBlock2; //Camera's World Space Position
	float4 dataBlock3;
	float4 dataBlock4;
};


//Lighting Functions
void DirectionalLightFactors(in float3 pixelNormal, in float3 pixelToCameraDir, in float4 textureDiffuse, out float4 oAmbient, out float4 oDiffuse, out float4 oSpecular)
{
	//Reset Values
	oAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	oDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	oSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Flip Direction For Dot Product
	float3 pixel2LightDirection = -mDirLightDirection;

	//Ambient Component
	oAmbient = mObjectAmbient * mDirLightAmbient;

	float diffuseFactor = dot(pixel2LightDirection, pixelNormal);
	[flatten] //Avoid Dynamic Branching 
	if (diffuseFactor > 0.0f)
	{
		//reflect, calcs reflection vector given the incident vector and a surface normal
		float3 reflectionVector = reflect(-pixel2LightDirection, pixelNormal);
			float specularFactor = pow(max(dot(reflectionVector, pixelToCameraDir), 0.0f), mObjectSpecular.w);

		//Diffuse and Specular Components
		oDiffuse = diffuseFactor * textureDiffuse * mDirLightDiffuse;
		oSpecular = specularFactor * mObjectSpecular * mDirLightSpecular;
	}
}

void PointLightFactors(in float3 pixelNormal, in float3 pixelsWorldPos, in float3 pixelToCameraDir, in float4 textureDiffuse, out float4 oAmbient, out float4 oDiffuse, out float4 oSpecular)
{
	//Reset Values
	oAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	oDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	oSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 pixel2LightDirection = mPointLightPosition - pixelsWorldPos;
		float pixel2LightDistance = length(pixel2LightDirection);

	[flatten] //Avoid Dynamic Branching 
	if (pixel2LightDistance > mPointLightRange)
	{
		return;
	}
	pixel2LightDirection = normalize(pixel2LightDirection);

	//Ambient Component
	oAmbient = mObjectAmbient * mPointLightAmbient;

	float diffuseFactor = dot(pixel2LightDirection, pixelNormal);
	[flatten] //Avoid Dynamic Branching 
	if (diffuseFactor > 0.0f)
	{

		float3 reflectionVector = reflect(-pixel2LightDirection, pixelNormal);
			float specularFactor = pow(max(dot(reflectionVector, pixelToCameraDir), 0.0f), mObjectSpecular.w);

		oDiffuse = diffuseFactor * textureDiffuse * mPointLightDiffuse;
		oSpecular = specularFactor * mObjectSpecular * mPointLightSpecular;
	}
	//Attenuation Formula.
	float attenuation = 1.0f / dot(mPointLightAttenuation, float3(1.0f, pixel2LightDistance, pixel2LightDistance * pixel2LightDistance));

	//Diffuse and Specular Components
	oDiffuse *= attenuation;
	oSpecular *= attenuation;
}

//Normal Mapping
float3 ComputeBumpedNormal(float3 normalMapSample, float3 pixelNormal, float3 pixelTangent)
{
	//Bring Normal Maps Sampled Vec to Scale
	float3 textureNormal = 2.0f * normalMapSample - 1.0f;
	//Build TBN Matrix
	float3 N = textureNormal; //Normal
	float3 T = normalize(pixelTangent - dot(pixelTangent, N) * N); //BiTangent
	float3 B = cross(N, T); //BiNormal

	float3x3 TBN = float3x3(T, B, N);

	//Sampled Normal, Tangent Space->World Space
	float3 bumpedNormal = mul(textureNormal, TBN);

	return bumpedNormal;
}


struct PS_IN
{
	float4 inPosClip   : SV_POSITION;
	float4 inPosWorld  : POSITION;
	float3 inNormVec   : NORMAL;
	float3 inTanVec    : TANGENT;
	float2 inUVs       : TEXCOORD;
};

Texture2D mColorMap   : register(t0);
Texture2D mNormalMap   : register(t1);
SamplerState mSampler : register(s0);

float4 main(PS_IN fragmentIn) : SV_TARGET
{
	//Init
	float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 FinalAmbientFactor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 FinalDiffuseFactor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 FinalSpecularFactor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 PixelNormVec = normalize(fragmentIn.inNormVec);
	float3 Pixel2CameraDirection = normalize(dataBlock2.xyz - fragmentIn.inPosWorld.xyz);
	float4 textureDiffuse = mColorMap.Sample(mSampler, fragmentIn.inUVs * dataBlock1.x);
	float3 NormalMapSample = mNormalMap.Sample(mSampler, fragmentIn.inUVs).rgb;

	//References
	float4 tempAmbientFactor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 tempDiffuseFactor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 tempSpecularFactor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Normal Mapping
	float3 BumpedNormal = ComputeBumpedNormal(NormalMapSample, PixelNormVec, fragmentIn.inTanVec);

	//Directional
	DirectionalLightFactors(BumpedNormal, Pixel2CameraDirection, textureDiffuse, tempAmbientFactor, tempDiffuseFactor, tempSpecularFactor);
	FinalAmbientFactor += tempAmbientFactor;
	FinalDiffuseFactor += tempDiffuseFactor;
	FinalSpecularFactor += tempSpecularFactor;

	//Point
	PointLightFactors(BumpedNormal, fragmentIn.inPosWorld.xyz, Pixel2CameraDirection, textureDiffuse, tempAmbientFactor, tempDiffuseFactor, tempSpecularFactor);
	FinalAmbientFactor += tempAmbientFactor;
	FinalDiffuseFactor += tempDiffuseFactor;
	FinalSpecularFactor += tempSpecularFactor;

	//SpotLight Disabled to Appreciate the bumpiness provided by the normal map

	//Combine
	FinalColor = FinalAmbientFactor + FinalDiffuseFactor + FinalSpecularFactor;
	FinalColor.a = mObjectDiffuse.a;
	return FinalColor;
}