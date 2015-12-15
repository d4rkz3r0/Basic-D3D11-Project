#pragma once
#include <DirectXMath.h>
using namespace DirectX;

/*Quick and Dirty Info
-datablock1 - Scalars
x - UVx Multiplier
y - UVy Multiplier
z - 
w - currentTime
-datablock2
x - cameraWorldPosX
y - cameraWorldPosX
z - cameraWorldPosX
w -
-datablock3
x - cameraUpVecX
y - cameraUpVecY
z - cameraUpVecZ
w -
-datablock4
x - unused
y - unused
z - unused
w - unused

*/

struct cbMiscInfo
{
	XMFLOAT4 dataBlock1;
	XMFLOAT4 dataBlock2;
	XMFLOAT4 dataBlock3;
	XMFLOAT4 dataBlock4;
};