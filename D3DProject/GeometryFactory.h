#pragma once
#include "D3DUtils.h"

struct PosVertex
{
	XMFLOAT3 position;
};

struct PosColVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

//struct Vertex
//{
//	XMFLOAT3 position;
//	XMFLOAT3 normal;
//	XMFLOAT2 texCoord;
//};

struct PMeshData
{
	std::vector<PosVertex> Vertices;
	std::vector<UINT> Indices;
	UINT vbCount;
	UINT vbSize;
	UINT ibCount;
	UINT ibSize;
};

struct PCMeshData
{
	std::vector<PosColVertex> Vertices;
	std::vector<UINT> Indices;
	UINT vbCount;
	UINT vbSize;
	UINT ibCount;
	UINT ibSize;
};


struct cbPerObject
{
	XMFLOAT4X4 WVP;
};

class GeometryFactory
{
	public:
		GeometryFactory();
		~GeometryFactory();

		void GenerateSkyBox(PMeshData& meshData);
		void GenerateCube(PCMeshData& meshData);
		void GenerateGrid(PCMeshData& meshData);
		void GenerateStar(PCMeshData& meshData);
};