#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct PosVertex
{
	XMFLOAT3 position;
};

struct PosColVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoords;
};

struct FullVertex
{
	FullVertex(){}

	FullVertex(const XMFLOAT3& pos, const XMFLOAT3& norm, const XMFLOAT3& tan, const XMFLOAT2& uv) :
		position(pos),
		normal(norm),
		tangent(tan),
		texCoords(uv){}

	FullVertex(float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		position(px, py, pz),
		normal(nx, ny, nz),
		tangent(tx, ty, tz),
		texCoords(u, v){}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 texCoords;
};

struct PointSprite
{
	XMFLOAT4 Position;
	XMFLOAT2 Size;

	PointSprite() { }

	PointSprite(const XMFLOAT4& position, const XMFLOAT2& size)
		: Position(position), Size(size)
	{

	}
};