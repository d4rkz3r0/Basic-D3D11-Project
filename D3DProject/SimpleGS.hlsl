//struct GSOutput
//{
//	float4 posH : SV_POSITION;
//};
//
//struct GSInput
//{
//	point float4 input[3] : SV_POSITION;
//
//};
//
//[maxvertexcount(3)]
//void main(inout TriangleStream< GSOutput > output)
//{
//	//append triangle, line, point data to a primitive
//
//	for (uint i = 0; i < 3; i++)
//	{
//		GSOutput element;
//		element.pos = input[i];
//		output.Append(element);
//	}
//	//Output vertex data in projected space 
//	output.RestartStrip();
//}
//
////Position
////Rotation
////Colors
////TextureID