#pragma once
#include "D3DUtils.h"
#include "VertexRefs.h"

struct aiMesh;

struct PMeshData
{
	vector<PosVertex> Vertices;
	vector<UINT> Indices;
	UINT vbCount;
	UINT vbSize;
	UINT ibCount;
	UINT ibSize;
};

struct PCMeshData
{
	vector<PosColVertex> Vertices;
	vector<UINT> Indices;
	UINT vbCount;
	UINT vbSize;
	UINT ibCount;
	UINT ibSize;
};

struct MeshData
{
	vector<Vertex> Vertices;
	vector<UINT> Indices;
	UINT vbCount;
	UINT vbSize;
	UINT ibCount;
	UINT ibSize;
};

struct FMeshData
{
	FMeshData(){}

	FMeshData(vector<FullVertex>& verts, vector<UINT> indices, UINT faceCount) :
		mVertices(verts),
		mIndices(indices),
		mFaceCount(faceCount){}

	vector<FullVertex> mVertices;
	vector<UINT> mIndices;
	UINT mFaceCount;
	
	//Temp Containers to push into the mVertices after model loading.
	vector<XMFLOAT3> mNormals;
	vector<XMFLOAT3> mTangents;
	vector<XMFLOAT3> mTexCoords;
};

struct FModelInfo
{
	FModelInfo(){}

	vector<FMeshData> mObjectMeshes;
	UINT mNumMeshses;


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
		void GenerateModel(FMeshData& meshData, string& fileName, bool UVFlag = false, bool TriangSmoothNormalsFlag = true);
		void GenerateModelBuffers(ID3D11Device* device, FMeshData& meshData, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer);
};