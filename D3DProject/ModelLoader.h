#include "D3DUtils.h"
#include <vector>

class ModelLoader
{
public:

	ModelLoader();
	~ModelLoader();

	void LoadObject(const char* fileName, vector<XMFLOAT4>& vertices, vector<XMFLOAT3>& normals, vector<UINT>& faces);
	//void LoadObject(const char* fileName, PMeshData& meshData);
};
