#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct DirectionalLight
{
	DirectionalLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float padding;
};

struct PointLight
{
	PointLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Position;
	float Range;
	XMFLOAT3 Attenuation;
	float padding;
};

struct SpotLight
{
	SpotLight()
	{
		ZeroMemory(this, sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Position;
	float Range;
	XMFLOAT3 Direction;
	float Spot;
	XMFLOAT3 Attenuation;
	float padding;
};

struct Material
{
	
};