#pragma once
#include <DirectXMath.h>
using namespace DirectX;


/*Quick and Dirty Info
-datablock1
x - UVMultiplier for current Object
y - 
z - 
w -
-datablock2
x - instanceXPos
y - instanceYPos
z - instanceZPos
w -
*/


struct cbMiscInfo
{
	XMFLOAT4 dataBlock1;
	XMFLOAT4 dataBlock2;
	XMFLOAT4 dataBlock3;
	XMFLOAT4 dataBlock4;
};