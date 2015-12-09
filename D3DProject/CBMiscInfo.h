#pragma once
#include <DirectXMath.h>
using namespace DirectX;


/*Quick and Dirty Info, unmanageable
-datablock1 x, y, z, w
x - UVMultiplier
y - 
z - 
w -
-datablock2


*/


struct cbMiscInfo
{
	XMFLOAT4 dataBlock1;
	XMFLOAT4 dataBlock2;
	XMFLOAT4 dataBlock3;
	XMFLOAT4 dataBlock4;
};