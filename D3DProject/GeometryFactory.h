#pragma once
#include "D3DUtils.h"
#include "VertexRefs.h"
#include "InstanceRefs.h"

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
	UINT mNumMeshes;
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
		void GenerateStaticQuad(ID3D11Device* device, MeshData& meshData);
		void GenerateModel(FMeshData& meshData, string& fileName, bool UVFlag = false, bool triangulateFlag = true, bool genNormalsFlag = true,	bool sortByPrimitiveType = true, bool removeDupVertFlag = true);
		void GenerateVertexAndIndexBuffers(ID3D11Device* device, FMeshData& meshData, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer);
		void GenerateVertexAndIndexBuffersNon(ID3D11Device* device, MeshData& meshData, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer);
		void GenerateBillBoards(ID3D11Device* device, ID3D11Buffer** vertexBuffer, UINT maxBillboards, float billboardSize, float xPosRange, float height, float zPosRange);
		void GenerateInstanceBuffer(ID3D11Device* device, ID3D11Buffer** instanceBuffer, std::vector<InstanceData>& instanceData, UINT numInstances, float xPosRange, float height, float zPosRange);
		void GenerateFullscreenQuad(ID3D11Device* device, MeshData& meshData, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer);

		random_device mRandDevice;
};