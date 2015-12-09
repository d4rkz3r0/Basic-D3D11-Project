#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct cbPerObjectTransformation
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorldMatrix;
};

