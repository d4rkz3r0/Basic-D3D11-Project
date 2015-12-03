#include "D3DUtils.h"
#include <vector>

//Basic .OBJ Loader
//Reads in Verts, faces, vertex normals
//Normals
//Faces (3 Indices Per Face)
//Based on Given: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//Broken

class ModelLoader
{
public:

	ModelLoader();
	~ModelLoader();

	void LoadObject(const char* fileName, vector<XMFLOAT4>& vertices, vector<XMFLOAT3>& normals, vector<UINT>& faces);
	//void LoadObject(const char* fileName, PMeshData& meshData);
};
