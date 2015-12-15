cbuffer ShaderMiscInfo : register(b10)
{
	float4 dataBlock1; //UVx, UVy, ?, time
	float4 dataBlock2; //Camera's World Space Position
	float4 dataBlock3; //Camera's Current Up Vector
	float4 dataBlock4; 
};
struct PS_IN
{
	float4 inPosH : SV_POSITION;
	float3 inPosL : TEXCOORD;
};

//Shader Resources
TextureCube skyboxTextureCube : register(t0);
TextureCube skyboxTextureCube2 : register(t1);
SamplerState skyboxSSO : register(s0);


float4 main(PS_IN vertexIn) : SV_TARGET
{
	float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 SkyBoxTexture1 = skyboxTextureCube.Sample(skyboxSSO, vertexIn.inPosL);
	float4 SkyBoxTexture2 = skyboxTextureCube2.Sample(skyboxSSO, vertexIn.inPosL);

	FinalColor = lerp(SkyBoxTexture1, SkyBoxTexture2, dataBlock1.w);
	return FinalColor;
}