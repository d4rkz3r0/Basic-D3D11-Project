#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <d3d11.h>

#include <DirectXMath.h>
#include <vector>
#include <ctime>
#include <cassert>
#include <string>
#include <sstream>
#include "DDSTextureLoader.h"
#include "inc/d3dx11effect.h"
#include "inc/d3dxGlobal.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace DirectX;

//Colors
namespace Colors
{
	XMGLOBALCONST XMFLOAT4 White   = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Black   = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Red     = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Green   = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
}

//Useful Macros
#define SAFE_RELEASE(comObj) { if ( (comObj) ) { (comObj)->Release(); (comObj) = 0; } }
#define SAFE_DELETE(myObj) if( (myObj) != NULL ) delete (myObj); (myObj) = NULL;