#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct cbPerObjectTransformation
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorldMatrix;
};

struct cbPerObjectMaterial
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecularPower
	XMFLOAT4 Reflect;
};