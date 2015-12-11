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

void GeometryFactory::GenerateStaticQuad(ID3D11Device* device, FMeshData& meshData)
{
	FullVertex vertices[] =
	{
		// Front Face
		FullVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		FullVertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		FullVertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		FullVertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f)
	};
	meshData.mVertices.assign(&vertices[0], &vertices[ARRAYSIZE(vertices)]);

	DWORD indices[] =
	{
		// Front Face
		0, 1, 2,
		0, 2, 3,
	};
	meshData.mIndices.assign(&indices[0], &indices[ARRAYSIZE(indices)]);
}
 

void GeometryFactory::GenerateModel(FMeshData& meshData, string& fileName,
	bool UVFlag, bool triangulateFlag, bool genNormalsFlag, bool sortByPrimitiveType,
	bool removeDupVertFlag)
{
	//Declare Importer
	Assimp::Importer importer;

	//Triangulate Flag + SortByPrimtiveType = No Point or Line Meshes.
	UINT flags = 0;

	if (removeDupVertFlag)
	{
		flags |= aiProcess_JoinIdenticalVertices;
	}
	if (sortByPrimitiveType)
	{
		flags |= aiProcess_SortByPType;
	}

	if (triangulateFlag)
	{
		flags |= aiProcess_Triangulate;
	}

	if (genNormalsFlag)
	{
		flags |= aiProcess_GenSmoothNormals;
	}

	if (UVFlag)
	{
		flags |= aiProcess_FlipUVs;
	}

	//Read In
	const aiScene* scene = importer.ReadFile(fileName, flags);

	UINT numMeshesInModel = scene->mNumMeshes;
	if (scene->HasMeshes())
	{
		for (UINT i = 0; i < numMeshesInModel; i++)
		{
			aiMesh* tempMesh = scene->mMeshes[i];

			//Allocate Space
			UINT numOfVerticesInMesh = tempMesh->mNumVertices;
			meshData.mVertices.reserve(numOfVerticesInMesh);
			meshData.mVertices.resize(numOfVerticesInMesh);

			//Position
			for (size_t i = 0; i < numOfVerticesInMesh; i++)
			{
				float tempX = tempMesh->mVertices[i].x;
				float tempY = tempMesh->mVertices[i].y;
				float tempZ = tempMesh->mVertices[i].z;

				meshData.mVertices[i].position.x = tempX;
				meshData.mVertices[i].position.y = tempY;
				meshData.mVertices[i].position.z = tempZ;
			}

			//Indices
			if (tempMesh->HasFaces())
			{
				meshData.mFaceCount = tempMesh->mNumFaces;
				UINT numFaces = tempMesh->mNumFaces;
				UINT numInidices = numFaces * 3;

				meshData.mIndices.reserve(numInidices);
				meshData.mIndices.resize(numInidices);

				for (size_t i = 0; i < numFaces; i++)
				{
					aiFace* tempFace = &tempMesh->mFaces[i];
					assert(tempFace->mNumIndices == 3);

					meshData.mIndices.push_back(tempFace->mIndices[0]);
					meshData.mIndices.push_back(tempFace->mIndices[1]);
					meshData.mIndices.push_back(tempFace->mIndices[2]);
				}
			}

			//Normals
			if (tempMesh->HasNormals())
			{
				for (size_t i = 0; i < numOfVerticesInMesh; i++)
				{
					float tempX = tempMesh->mNormals[i].x;
					float tempY = tempMesh->mNormals[i].y;
					float tempZ = tempMesh->mNormals[i].z;

					meshData.mVertices[i].normal = XMFLOAT3(tempX, tempY, tempZ);
				}
			}

			//Tangents
			if (tempMesh->HasTangentsAndBitangents())
			{
				for (size_t i = 0; i < numOfVerticesInMesh; i++)
				{
					float tempX = tempMesh->mTangents[i].x;
					float tempY = tempMesh->mTangents[i].y;
					float tempZ = tempMesh->mTangents[i].z;

					meshData.mVertices[i].tangent = XMFLOAT3(tempX, tempY, tempZ);
				}
			}

			//Tex Coords
			if (tempMesh->GetNumUVChannels() != 0)
			{
				aiVector3D* aiTextureCoordinates = tempMesh->mTextureCoords[0];

				for (size_t i = 0; i < numOfVerticesInMesh; i++)
				{
					float tempX = aiTextureCoordinates[i].x;
					float tempY = aiTextureCoordinates[i].y;

					meshData.mVertices[i].texCoords = XMFLOAT2(tempX, tempY);
				}
			}
		}
	}
}

void GeometryFactory::GenerateModelBuffers(ID3D11Device* device, FMeshData& meshData, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(FullVertex) * meshData.mVertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = &meshData.mVertices[0];
	device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = sizeof(UINT) * meshData.mIndices.size();
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResourceData;
	ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
	indexSubResourceData.pSysMem = &meshData.mIndices[0];
	device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer);
}

void GeometryFactory::GenerateInstanceBuffer(ID3D11Device* device, std::vector<InstanceData>& instanceData, int numInstances, ID3D11Buffer** instanceBuffer)
{
	D3D11_BUFFER_DESC InstanceBufferDesc;
	ZeroMemory(&InstanceBufferDesc, sizeof(InstanceBufferDesc));
	InstanceBufferDesc.ByteWidth = sizeof(instanceData) * numInstances;
	InstanceBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InstanceSubResourceData;
	ZeroMemory(&InstanceSubResourceData, sizeof(InstanceSubResourceData));
	InstanceSubResourceData.pSysMem = &instanceData[0];
	device->CreateBuffer(&InstanceBufferDesc, &InstanceSubResourceData, instanceBuffer);
}

void GeometryFactory::GenerateBillBoards(ID3D11Device* device, ID3D11Buffer** vertexBuffer, UINT maxBillboards, float billboardSize, float xPosRange, float height, float zPosRange)
{
	default_random_engine randGen(mRandDevice());
	uniform_real_distribution<float> PosXDistribution(-xPosRange, xPosRange);
	uniform_real_distribution<float> PosZDistribution(-zPosRange, zPosRange);

	vector<PointSprite> vertices;
	vertices.reserve(maxBillboards);
	for (UINT i = 0; i < maxBillboards; i++)
	{
		float x = PosXDistribution(randGen);
		float y = height;
		float z = PosZDistribution(randGen);

		vertices.push_back(PointSprite(XMFLOAT4(x, y, z, 1.0f), XMFLOAT2(billboardSize, billboardSize)));
	}
	UINT vertSize = vertices.size();

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(PointSprite) * vertSize;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = &vertices[0];
	device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer);	
}
