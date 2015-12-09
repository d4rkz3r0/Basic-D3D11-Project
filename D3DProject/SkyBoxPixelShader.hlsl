struct PS_IN
{
	float4 inPosH : SV_POSITION;
	float3 inPosL : TEXCOORD;
};

//Shader Resources
TextureCube skyboxTextureCube : register(t0);
SamplerState skyboxSSO : register(s0);


float4 main(PS_IN vertexIn) : SV_TARGET
{
	return skyboxTextureCube.Sample(skyboxSSO, vertexIn.inPosL);
}