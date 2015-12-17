#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct InstanceData
{
	XMFLOAT4X4 WorldMX;
	XMFLOAT4 Color;
};

//struct InstanceRotation
//{
//	XMFLOAT4X4 WorldMX;
//};