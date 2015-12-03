#include "ModelLoader.h"
#include "GeometryFactory.h"
#include <iostream>
using namespace std;

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::LoadObject(const char* fileName, vector<XMFLOAT4>& vertices, vector<XMFLOAT3>& normals, vector<UINT>& faces)
{
	std::ifstream fin(fileName, ios::in);
	string inputLine;

	while (getline(fin, inputLine))
	{
		//Verts (Position)
		if (inputLine.substr(0, 2) == "v ")
		{
			istringstream ss(inputLine.substr(2));
			XMFLOAT4 tempVec;
			ss >> tempVec.x;
			ss >> tempVec.y;
			ss >> tempVec.z;
			tempVec.w = 1.0f;

			vertices.push_back(tempVec);
		}

		//Faces
		else if (inputLine.substr(0, 2) == "f ")
		{
			istringstream ss(inputLine.substr(2));
			UINT i1;
			UINT i2;
			UINT i3;
			ss >> i1;
			ss >> i2;
			ss >> i3;
			i1--;
			i2--;
			i3--;

			faces.push_back(i1);
			faces.push_back(i2);
			faces.push_back(i3);
		}
	}

	//Normals
	normals.resize(vertices.size(), XMFLOAT3(0.0f, 0.0f, 0.0f));

	for (UINT i = 0; i < faces.size(); i += 3)
	{
		UINT ia = faces[i];
		UINT ib = faces[i + 1];
		UINT ic = faces[i + 2];
		XMVECTOR iaV = XMLoadFloat4(&vertices[ia]);
		XMVECTOR ibV = XMLoadFloat4(&vertices[ib]);
		XMVECTOR icV = XMLoadFloat4(&vertices[ic]);

		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(XMVectorSubtract(ibV, iaV), XMVectorSubtract(icV, iaV)));
		XMStoreFloat3(&normals[ia], normal);
		XMStoreFloat3(&normals[ib], normal);
		XMStoreFloat3(&normals[ic], normal);
	}
}