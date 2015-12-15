#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <cassert>
#include <memory>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <random>
#include "DDSTextureLoader.h"
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace std;
using namespace DirectX;

//Colors
namespace ManualColors
{
	XMGLOBALCONST XMFLOAT4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
}

//Useful Macros
#define SAFE_RELEASE(comObj) { if ( (comObj) ) { (comObj)->Release(); (comObj) = 0; } }
#define SAFE_DELETE(myObj) if( (myObj) != nullptr ) delete (myObj); (myObj) = nullptr;
#define SAFE_DELETES(myObjs) if( (myObjs) != nullptr ) delete[] (myObjs); (myObjs) = nullptr;