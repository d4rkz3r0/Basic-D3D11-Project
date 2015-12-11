struct PS_IN
{
	float4 inPosH : SV_POSITION;
	float4 inColor : COLOR;
};


float4 main(PS_IN vertexIn) : SV_TARGET
{
	return vertexIn.inColor;
}
