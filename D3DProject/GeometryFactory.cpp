#include "GeometryFactory.h"

GeometryFactory::GeometryFactory()
{

}

GeometryFactory::~GeometryFactory()
{

}


void GeometryFactory::GenerateCube(PCMeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	PosColVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), Colors::Red },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), Colors::Blue },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), Colors::Red },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), Colors::Blue },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), Colors::Red },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), Colors::Green }
	};
	meshData.vbCount = ARRAYSIZE(vertices);
	meshData.vbSize = sizeof(PosColVertex) * meshData.vbCount;
	meshData.Vertices.assign(&vertices[0], &vertices[meshData.vbCount]);

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7,
		1, 5, 6,
		1, 6, 2,
		4, 0, 3,
		4, 3, 7
	};
	meshData.ibCount = ARRAYSIZE(indices);
	meshData.ibSize = sizeof(UINT) * meshData.ibCount;
	meshData.Indices.assign(&indices[0], &indices[meshData.ibCount]);
}

void GeometryFactory::GenerateGrid(PCMeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	PosColVertex vertices[] =
	{
		//Vertical Line Points
		{ XMFLOAT3(-1.0f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.9f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.9f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.8f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.8f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.7f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.7f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.6f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.6f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.5f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.5f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.4f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.4f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.3f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.3f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.2f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.2f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(-0.1f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-0.1f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.0f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.0f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.1f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.1f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.2f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.2f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.3f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.3f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.4f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.4f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.5f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.5f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.6f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.6f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.7f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.7f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.8f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.8f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+0.9f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+0.9f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -1.0f), Colors::Green },
		//Horizontal Line Points
		{ XMFLOAT3(-1.0f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +1.0f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.9f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.9f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.8f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.8f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.7f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.7f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.6f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.6f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.5f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.5f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.4f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.4f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.3f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.3f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.2f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.2f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.1f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.1f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, +0.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, +0.0f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.1f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.1f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.2f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.2f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.3f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.3f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.4f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.4f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.5f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.5f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.6f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.6f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.7f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.7f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.8f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.8f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -0.9f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -0.9f), Colors::Green },
		{ XMFLOAT3(-1.0f, +0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +0.0f, -1.0f), Colors::Green }
	};
	meshData.vbCount = ARRAYSIZE(vertices);
	meshData.vbSize = sizeof(PosColVertex) * meshData.vbCount;
	meshData.Vertices.assign(&vertices[0], &vertices[meshData.vbCount]);
}

void GeometryFactory::GenerateStar(PCMeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	PosColVertex vertices[] =
	{
		{ XMFLOAT3(-1.75f, 2.0f, 0.0f), Colors::Red },
		{ XMFLOAT3(0.0f, 7.0f, 0.0f), Colors::Green },
		{ XMFLOAT3(1.75f, 2.0f, 0.0f), Colors::Blue },
		{ XMFLOAT3(-3.5f, -2.0f, 0.0f), Colors::Red },
		{ XMFLOAT3(-7.0f, 1.5f, 0.0f), Colors::Green },
		{ XMFLOAT3(7.0f, 1.5f, 0.0f), Colors::Blue },
		{ XMFLOAT3(3.5f, -2.0f, 0.0f), Colors::Red },
		{ XMFLOAT3(5.0f, -7.0f, 0.0f), Colors::Green },
		{ XMFLOAT3(0.0f, -4.5f, 0.0f), Colors::Blue },
		{ XMFLOAT3(-5.0f, -7.0f, 0.0f), Colors::Red },
		{ XMFLOAT3(0.0f, 0.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(0.0f, 0.0f, +1.0f), Colors::Blue }
	};
	meshData.vbCount = ARRAYSIZE(vertices);
	meshData.vbSize = sizeof(PosColVertex) * meshData.vbCount;
	meshData.Vertices.assign(&vertices[0], &vertices[meshData.vbCount]);

	UINT indices[] =
	{
		2, 5, 6,
		6, 7, 8,
		8, 9, 3,
		3, 4, 0,
		6, 5, 2,
		8, 7, 6,
		3, 9, 8,
		0, 4, 3,
		1, 10, 3,
		6, 10, 1,
		3, 10, 8,
		8, 10, 6,
		3, 11, 1,
		1, 11, 6,
		8, 11, 3,
		6, 11, 8
	};
	meshData.ibCount = ARRAYSIZE(indices);
	meshData.ibSize = sizeof(UINT) * meshData.ibCount;
	meshData.Indices.assign(&indices[0], &indices[meshData.ibCount]);
}

void GeometryFactory::GenerateSkyBox(PMeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	PosVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f) },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f) },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f) },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f) },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f) }
	};
	meshData.vbCount = ARRAYSIZE(vertices);
	meshData.vbSize = sizeof(PosVertex) * meshData.vbCount;
	meshData.Vertices.assign(&vertices[0], &vertices[meshData.vbCount]);

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7,
		1, 5, 6,
		1, 6, 2,
		4, 0, 3,
		4, 3, 7
	};
	meshData.ibCount = ARRAYSIZE(indices);
	meshData.ibSize = sizeof(UINT) * meshData.ibCount;
	meshData.Indices.assign(&indices[0], &indices[meshData.ibCount]);
}