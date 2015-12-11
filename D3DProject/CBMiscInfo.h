#pragma once
#include <DirectXMath.h>
using namespace DirectX;

/*Quick and Dirty Info
-datablock1 - Scalars
x - UVMultiplier for current Object
y - 
z - 
w - 
-datablock2
x - cameraWorldPosX
y - cameraWorldPosX
z - cameraWorldPosX
w -
-datablock2
x - cameraUpVecX
y - cameraUpVecY
z - cameraUpVecZ
w -

*/

struct cbMiscInfo
{
	XMFLOAT4 dataBlock1;
	XMFLOAT4 dataBlock2;
	XMFLOAT4 dataBlock3;
	XMFLOAT4 dataBlock4;
};