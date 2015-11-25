//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <ctime>
#include "Camera.h"
#include "Cube.h"
#include "numbers.h"
#include "XTime.h"
#include "Trivial_PS.csh"
#include "Trivial_VS.csh"
#include "Grid_VS.csh"
#include "Green_PS.csh"
using namespace DirectX;
using namespace std;

//Macros
#define SAFE_RELEASE(comObj) { if ( (comObj) ) { (comObj)->Release(); (comObj) = 0; } }
#define SAFE_DELETE(myObj) if( (myObj) != NULL ) delete (myObj); (myObj) = NULL;
#define BACKBUFFER_WIDTH	500
#define BACKBUFFER_HEIGHT	500
#define degreesToRadians(angleDegrees) (angleDegrees * XM_PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / XM_PI)

//Win32 Function Declarations
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);


//DX Globals (Yuck)
ID3D11Device* d3dDevice_ = nullptr;
ID3D11DeviceContext* d3dContext_ = nullptr;
IDXGISwapChain* swapChain_ = nullptr;
ID3D11RenderTargetView* renderTargetView_ = nullptr;
ID3D11Texture2D* depthStencilBuffer_ = nullptr;
ID3D11DepthStencilState * depthStencilState_ = nullptr;
ID3D11DepthStencilView* depthStencilView_ = nullptr;
XMMATRIX cameraViewMX;
XMMATRIX projMX;

//App Globals
float GetIncreasingRadiansOverTime(float deltaTime);
float angle = 0.0f;
float camScalarSpeed = 0.0f;

//Camera
Camera myCamera;

//Misc Globals
LPWSTR appName = L"DX Proj Template v0.00001";
UINT numOfVertsPerTriangle = 3;
UINT numOfInputLayoutObjects = 0;
UINT numOfGridInputLayoutObjects = 1;
UINT numOfRawVertices = 0;
UINT numOfGridVertices = 0;
UINT totalNumOfGridVertices = 0;
UINT totalNumOfVertices;
XTime timer;

float currTimeF = 0.0f;
UINT currTime = 0;
UINT timeInterval = 2;
bool shouldSwap;
bool checkOffsetVal;
HWND window;

//************************************************************
//********************* GLOBAL STRUCTS ***********************
//************************************************************
struct perObjConstBuffer
{
	XMMATRIX WVP;
};

struct SIMPLE_VERTEX
{
	XMFLOAT4 position;

	SIMPLE_VERTEX(float _x, float _y, float _z, float _w)
	{
		position.x = _x;
		position.y = _y;
		position.z = _z;
		position.w = _w;
	}

	SIMPLE_VERTEX()
	{

	}
};

struct textureOffsets
{
	float textureOffset;
	float padding1;
	float padding2;
	float padding3;
};


//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************
class DEMO_APP
{	
	//Win32 Objects
	HINSTANCE						application;
	WNDPROC							appWndProc;

	//DX11 Init Objects
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
	
	ID3D11InputLayout* inputLayout_ = nullptr;
	ID3D11InputLayout* gridInputLayout_ = nullptr;
	D3D11_VIEWPORT viewPort_;

	//DX11 App Specific
	ID3D11Buffer* cubeVertBuffer_ = nullptr;
	ID3D11Buffer* cubeIndexBuffer_ = nullptr;
	ID3D11Buffer* gridVertBuffer_ = nullptr;
	ID3D11Buffer* gridIndexBuffer_ = nullptr;
	ID3D11Buffer* WVPConstBuffer_ = nullptr;
	ID3D11Buffer* textureOffsetsConstBuffer_ = nullptr;
	ID3D11Texture2D* cubeColorMapTexture_ = nullptr;
	ID3D11SamplerState* textureSampler_ = nullptr;
	ID3D11ShaderResourceView* cubeColorMapSRV_ = nullptr;
	ID3D11BlendState* defaultBlendState_ = nullptr;
	ID3D11BlendState* transparencyBlendState_ = nullptr;
	ID3D11RasterizerState* CCWCullingModeRS_ = nullptr;
	ID3D11RasterizerState* CWCullingModeRS_ = nullptr;
	ID3D11RasterizerState* noAARS_ = nullptr;
	ID3D11VertexShader* VS_ = nullptr;
	ID3D11PixelShader* PS_ = nullptr;
	ID3D11VertexShader* GRID_VS_ = nullptr;
	ID3D11PixelShader* GRID_PS_ = nullptr;

	//Global 3D Objects
	XMMATRIX globalXRotationMX;
	XMMATRIX globalYRotationMX;
	XMMATRIX globalZRotationMX;
	XMVECTOR globalXAxis;
	XMVECTOR globalYAxis;
	XMVECTOR globalZAxis;

	//App Specific
	XMMATRIX cubeWorldMX;
	XMMATRIX gridWorldMX;
	XMMATRIX WVP;

	//Constant Buffers
	perObjConstBuffer perObjCB;
	textureOffsets textureOffsetCB;


public:
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	void Win32Init(HINSTANCE hInst, WNDPROC procFunction);
	void DX11Init();
	bool Run();
	bool AppShutdown();
	void DXShutDown();
	bool ShutDown();
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hInst, WNDPROC procFunction)
{
	Win32Init(hInst, procFunction);
	DX11Init();
	
	//Global 3D Object Initialization
	globalXRotationMX = XMMatrixIdentity();
	globalYRotationMX = XMMatrixIdentity();
	globalZRotationMX = XMMatrixIdentity();
	globalXAxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	globalYAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	globalZAxis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//Proj MX
	DXGI_SWAP_CHAIN_DESC config;
	ZeroMemory(&config, sizeof(config));
	swapChain_->GetDesc(&config);
	UINT initialWidth = config.BufferDesc.Width;
	UINT initialHeight = config.BufferDesc.Height;
	float initialAspectRatio = (float)(initialWidth / initialHeight);
	projMX = XMMatrixPerspectiveFovLH(XM_PIDIV2, initialAspectRatio, 0.05f, 1000.f);
	
	//Objects MX 
	gridWorldMX = XMMatrixIdentity();
	cubeWorldMX = XMMatrixIdentity();
	WVP = XMMatrixIdentity();
	textureOffsetCB.textureOffset = 0.0f;
	textureOffsetCB.padding1 = 0.0f;
	textureOffsetCB.padding2 = 0.0f;
	textureOffsetCB.padding3 = 0.0f;
	shouldSwap = false;
	checkOffsetVal = false;

	SIMPLE_VERTEX gridVerts[] = 
	{
		//Vertical Line Points
		{ -1.0f, +0.0f, +1.0f, +1.0f },
		{ -1.0f, +0.0f, -1.0f, +1.0f },
		{ -0.9f, +0.0f, +1.0f, +1.0f },
		{ -0.9f, +0.0f, -1.0f, +1.0f },
		{ -0.8f, +0.0f, +1.0f, +1.0f },
		{ -0.8f, +0.0f, -1.0f, +1.0f },
		{ -0.7f, +0.0f, +1.0f, +1.0f },
		{ -0.7f, +0.0f, -1.0f, +1.0f },
		{ -0.6f, +0.0f, +1.0f, +1.0f },
		{ -0.6f, +0.0f, -1.0f, +1.0f },
		{ -0.5f, +0.0f, +1.0f, +1.0f },
		{ -0.5f, +0.0f, -1.0f, +1.0f },
		{ -0.4f, +0.0f, +1.0f, +1.0f },
		{ -0.4f, +0.0f, -1.0f, +1.0f },
		{ -0.3f, +0.0f, +1.0f, +1.0f },
		{ -0.3f, +0.0f, -1.0f, +1.0f },
		{ -0.2f, +0.0f, +1.0f, +1.0f },
		{ -0.2f, +0.0f, -1.0f, +1.0f },
		{ -0.1f, +0.0f, +1.0f, +1.0f },
		{ -0.1f, +0.0f, -1.0f, +1.0f },
		{ +0.0f, +0.0f, +1.0f, +1.0f },
		{ +0.0f, +0.0f, -1.0f, +1.0f },
		{ +0.1f, +0.0f, +1.0f, +1.0f },
		{ +0.1f, +0.0f, -1.0f, +1.0f },
		{ +0.2f, +0.0f, +1.0f, +1.0f },
		{ +0.2f, +0.0f, -1.0f, +1.0f },
		{ +0.3f, +0.0f, +1.0f, +1.0f },
		{ +0.3f, +0.0f, -1.0f, +1.0f },
		{ +0.4f, +0.0f, +1.0f, +1.0f },
		{ +0.4f, +0.0f, -1.0f, +1.0f },
		{ +0.5f, +0.0f, +1.0f, +1.0f },
		{ +0.5f, +0.0f, -1.0f, +1.0f },
		{ +0.6f, +0.0f, +1.0f, +1.0f },
		{ +0.6f, +0.0f, -1.0f, +1.0f },
		{ +0.7f, +0.0f, +1.0f, +1.0f },
		{ +0.7f, +0.0f, -1.0f, +1.0f },
		{ +0.8f, +0.0f, +1.0f, +1.0f },
		{ +0.8f, +0.0f, -1.0f, +1.0f },
		{ +0.9f, +0.0f, +1.0f, +1.0f },
		{ +0.9f, +0.0f, -1.0f, +1.0f },
		{ +1.0f, +0.0f, +1.0f, +1.0f },
		{ +1.0f, +0.0f, -1.0f, +1.0f },

		//Horizontal Line Points
		{ -1.0f, 0.0f, +1.0f, +1.0f },
		{ +1.0f, 0.0f, +1.0f, +1.0f },
		{ -1.0f, 0.0f, +0.9f, +1.0f },
		{ +1.0f, 0.0f, +0.9f, +1.0f },
		{ -1.0f, 0.0f, +0.8f, +1.0f },
		{ +1.0f, 0.0f, +0.8f, +1.0f },
		{ -1.0f, 0.0f, +0.7f, +1.0f },
		{ +1.0f, 0.0f, +0.7f, +1.0f },
		{ -1.0f, 0.0f, +0.6f, +1.0f },
		{ +1.0f, 0.0f, +0.6f, +1.0f },
		{ -1.0f, 0.0f, +0.5f, +1.0f },
		{ +1.0f, 0.0f, +0.5f, +1.0f },
		{ -1.0f, 0.0f, +0.4f, +1.0f },
		{ +1.0f, 0.0f, +0.4f, +1.0f },
		{ -1.0f, 0.0f, +0.3f, +1.0f },
		{ +1.0f, 0.0f, +0.3f, +1.0f },
		{ -1.0f, 0.0f, +0.2f, +1.0f },
		{ +1.0f, 0.0f, +0.2f, +1.0f },
		{ -1.0f, 0.0f, +0.1f, +1.0f },
		{ +1.0f, 0.0f, +0.1f, +1.0f },
		{ -1.0f, 0.0f, +0.0f, +1.0f }, //absolute middle
		{ +1.0f, 0.0f, +0.0f, +1.0f },
		{ -1.0f, 0.0f, -0.1f, +1.0f },
		{ +1.0f, 0.0f, -0.1f, +1.0f },
		{ -1.0f, 0.0f, -0.2f, +1.0f },
		{ +1.0f, 0.0f, -0.2f, +1.0f },
		{ -1.0f, 0.0f, -0.3f, +1.0f },
		{ +1.0f, 0.0f, -0.3f, +1.0f },
		{ -1.0f, 0.0f, -0.4f, +1.0f },
		{ +1.0f, 0.0f, -0.4f, +1.0f },
		{ -1.0f, 0.0f, -0.5f, +1.0f },
		{ +1.0f, 0.0f, -0.5f, +1.0f },
		{ -1.0f, 0.0f, -0.6f, +1.0f },
		{ +1.0f, 0.0f, -0.6f, +1.0f },
		{ -1.0f, 0.0f, -0.7f, +1.0f },
		{ +1.0f, 0.0f, -0.7f, +1.0f },
		{ -1.0f, 0.0f, -0.8f, +1.0f },
		{ +1.0f, 0.0f, -0.8f, +1.0f },
		{ -1.0f, 0.0f, -0.9f, +1.0f },
		{ +1.0f, 0.0f, -0.9f, +1.0f },
		{ -1.0f, 0.0f, -1.0f, +1.0f },
		{ +1.0f, 0.0f, -1.0f, +1.0f }
	};
	
	//Camera Initialization
	myCamera.SetPosition(0.0f, 0.55f, -3.0f);
	camScalarSpeed = 5.0f;
	cameraViewMX = myCamera.GetView();

	//Load Vertex Info From Header File
	numOfRawVertices = ARRAYSIZE(Cube_data);

	//Grid Verts
	numOfGridVertices = ARRAYSIZE(gridVerts);

	//Grid VB
	D3D11_BUFFER_DESC gridVertexBufferDesc;
	ZeroMemory(&gridVertexBufferDesc, sizeof(gridVertexBufferDesc));
	gridVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	gridVertexBufferDesc.ByteWidth = sizeof(SIMPLE_VERTEX) * numOfGridVertices;
	gridVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	gridVertexBufferDesc.CPUAccessFlags = NULL;
	gridVertexBufferDesc.MiscFlags = NULL;
	gridVertexBufferDesc.StructureByteStride = NULL;

	D3D11_SUBRESOURCE_DATA gridVertexBufferData;
	ZeroMemory(&gridVertexBufferData, sizeof(gridVertexBufferData));
	gridVertexBufferData.SysMemPitch = NULL;
	gridVertexBufferData.SysMemSlicePitch = NULL;
	gridVertexBufferData.pSysMem = gridVerts;
	d3dDevice_->CreateBuffer(&gridVertexBufferDesc, &gridVertexBufferData, &gridVertBuffer_);

	//Define Unique Triangles
	UINT numOfTrianglesInShape = ARRAYSIZE(Cube_indicies) / 3;
	totalNumOfVertices = numOfTrianglesInShape * numOfVertsPerTriangle;

	////Buffer Creation and Initialization
	//VB Buffer Desc
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(_OBJ_VERT_) * numOfRawVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.MiscFlags = NULL;
	vertexBufferDesc.StructureByteStride = NULL;
	
	//VB Initial Data
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.SysMemPitch = NULL;
	vertexBufferData.SysMemSlicePitch = NULL;
	vertexBufferData.pSysMem = Cube_data;

	//Create VB
	d3dDevice_->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &cubeVertBuffer_);

	//IB Buffer Desc
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * totalNumOfVertices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = NULL;
	indexBufferDesc.MiscFlags = NULL;
	indexBufferDesc.StructureByteStride = NULL;

	//IB Initial Data
	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.SysMemPitch = NULL;
	indexBufferData.SysMemSlicePitch = NULL;
	indexBufferData.pSysMem = Cube_indicies;

	//Create IB
	d3dDevice_->CreateBuffer(&indexBufferDesc, &indexBufferData, &cubeIndexBuffer_);

	//Texture2D Color Map Desc
	D3D11_TEXTURE2D_DESC cubeTextureDesc;
	ZeroMemory(&cubeTextureDesc, sizeof(cubeTextureDesc));
	cubeTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeTextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	cubeTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	cubeTextureDesc.Width = (UINT)numbers_width;
	cubeTextureDesc.Height = (UINT)numbers_height;
	cubeTextureDesc.ArraySize = 1;
	cubeTextureDesc.MipLevels = numbers_numlevels;
	cubeTextureDesc.CPUAccessFlags = NULL;
	cubeTextureDesc.MiscFlags = NULL;
	cubeTextureDesc.SampleDesc.Count = 1;
	cubeTextureDesc.SampleDesc.Quality = 0;

	//Populate SubResource Array for Texture2D
	D3D11_SUBRESOURCE_DATA cubeTextureInitialData[numbers_numlevels];
	ZeroMemory(&cubeTextureInitialData, sizeof(cubeTextureInitialData));
	unsigned int textureArrayOffset = 0;
	unsigned int currTextureWidth = 0;
	unsigned int currTextureHeight = 0;
	
	for (size_t i = 0; i < numbers_numlevels; i++)
	{
		textureArrayOffset = numbers_leveloffsets[i];
		currTextureWidth = numbers_width >> i;
		currTextureHeight = numbers_height >> i;
		cubeTextureInitialData[i].SysMemPitch = sizeof(unsigned int) * currTextureWidth;
		cubeTextureInitialData[i].SysMemSlicePitch = NULL;
		cubeTextureInitialData[i].pSysMem = &numbers_pixels[textureArrayOffset];
	}

	//Create and Fill Texture2D Color Map
	d3dDevice_->CreateTexture2D(&cubeTextureDesc, cubeTextureInitialData, &cubeColorMapTexture_);
	//Create and Bind SRV to Cube Color Map
	d3dDevice_->CreateShaderResourceView(cubeColorMapTexture_, NULL, &cubeColorMapSRV_);

	//Texture Sampler Desc
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create SSO
	d3dDevice_->CreateSamplerState(&samplerDesc, &textureSampler_);

	//WVP MX CB Buffer Desc
	D3D11_BUFFER_DESC perObjCBDesc;
	ZeroMemory(&perObjCBDesc, sizeof(perObjCBDesc));
	perObjCBDesc.Usage = D3D11_USAGE_DYNAMIC;
	perObjCBDesc.ByteWidth = sizeof(perObjConstBuffer);
	perObjCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perObjCBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perObjCBDesc.MiscFlags = NULL;
	perObjCBDesc.StructureByteStride = NULL;
	d3dDevice_->CreateBuffer(&perObjCBDesc, NULL, &WVPConstBuffer_);

	//U Coord TextureOffsets CB Buffer Desc
	D3D11_BUFFER_DESC textureOffsetDesc;
	ZeroMemory(&textureOffsetDesc, sizeof(textureOffsetDesc));
	textureOffsetDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureOffsetDesc.ByteWidth = sizeof(perObjConstBuffer);
	textureOffsetDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	textureOffsetDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureOffsetDesc.MiscFlags = NULL;
	textureOffsetDesc.StructureByteStride = NULL;

	//Create CB
	d3dDevice_->CreateBuffer(&textureOffsetDesc, NULL, &textureOffsetsConstBuffer_);

	//Cube VS and PS
	d3dDevice_->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &VS_);
	d3dDevice_->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &PS_);

	//Grid VS and PS
	d3dDevice_->CreateVertexShader(Grid_VS, sizeof(Grid_VS), NULL, &GRID_VS_);
	d3dDevice_->CreatePixelShader(Green_PS, sizeof(Green_PS), NULL, &GRID_PS_);



	//Cube Vertex Layout Desc
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numOfInputLayoutObjects = ARRAYSIZE(inputLayoutDesc);
	d3dDevice_->CreateInputLayout(inputLayoutDesc, numOfInputLayoutObjects, Trivial_VS, sizeof(Trivial_VS), &inputLayout_);

	//Grid Vertex Layout Desc
	D3D11_INPUT_ELEMENT_DESC inputGridLayoutDesc[1] =
	{
		{ "GPOSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	numOfGridInputLayoutObjects = ARRAYSIZE(inputGridLayoutDesc);
	d3dDevice_->CreateInputLayout(inputGridLayoutDesc, numOfGridInputLayoutObjects, Grid_VS, sizeof(Grid_VS), &gridInputLayout_);
	d3dContext_->RSSetViewports(1, &viewPort_);
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	//*****************************Update Code*****************************/
	timer.Signal();
	
	currTimeF = static_cast<float>(timer.TotalTime());
	currTime = static_cast<UINT>(roundf(currTimeF));

	if (currTime % timeInterval == 0 && !shouldSwap)
	{
		//textureOffsets
		//1 - 0.75
		//2 - 0.5
		//3 - 0.25
		//4 - 0.0
		if (textureOffsetCB.textureOffset == 0.75)
		{
			textureOffsetCB.textureOffset = 0.5f;
			shouldSwap = true;
		}
		else if (textureOffsetCB.textureOffset == 0.5)
		{
			textureOffsetCB.textureOffset = 0.25f;
			shouldSwap = true;
		}
		else if (textureOffsetCB.textureOffset == 0.25)
		{
			textureOffsetCB.textureOffset = 0.0f;
			shouldSwap = true;
		}
		else if (textureOffsetCB.textureOffset == 0.0)
		{
			textureOffsetCB.textureOffset = 0.75f;
			shouldSwap = true;
		}
		
	}
	else if (currTime % timeInterval == 1 && shouldSwap)
	{
		shouldSwap = false;
	}

	//Keyboard Input
	if (GetAsyncKeyState('W') & 0x8000)
	{
		myCamera.Walk(camScalarSpeed * static_cast<float>(timer.SmoothDelta()));
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		myCamera.Walk(-camScalarSpeed * static_cast<float>(timer.SmoothDelta()));
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		myCamera.Strafe(-camScalarSpeed * static_cast<float>(timer.SmoothDelta()));
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		myCamera.Strafe(camScalarSpeed * static_cast<float>(timer.SmoothDelta()));
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		myCamera.Pitch(XMConvertToRadians(-30 * static_cast<float>(timer.SmoothDelta())));
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		myCamera.Pitch(XMConvertToRadians(50.0f * static_cast<float>(timer.SmoothDelta())));
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		myCamera.RotateY(XMConvertToRadians(-50.0f * static_cast<float>(timer.SmoothDelta())));
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		myCamera.RotateY(XMConvertToRadians(50.0f * static_cast<float>(timer.SmoothDelta())));
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		myCamera.VFly(camScalarSpeed * static_cast<float>(timer.SmoothDelta()));
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		myCamera.VFly(-camScalarSpeed * static_cast<float>(timer.SmoothDelta()));
	}

	if (GetAsyncKeyState('I') )
	{
		d3dContext_->RSSetState(noAARS_);
	}

	myCamera.UpdateViewMatrix();
	cameraViewMX = myCamera.GetView();

 
	globalYRotationMX = XMMatrixRotationAxis(globalYAxis, -GetIncreasingRadiansOverTime((float)timer.Delta()));
	globalZRotationMX = XMMatrixRotationAxis(globalZAxis, GetIncreasingRadiansOverTime((float)timer.Delta()));
	cubeWorldMX = globalYRotationMX * globalZRotationMX;

	

	

	
	
	//*****************************End Update*****************************/

	//Init
	//d3dContext_->RSSetViewports(1, &viewPort_);
	d3dContext_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	
	//*****************************Drawing Code*****************************/
	//Clear
	d3dContext_->ClearRenderTargetView(renderTargetView_, clearColor);
	d3dContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, NULL);
	
	////Grid
	//Init
	UINT stride = sizeof(SIMPLE_VERTEX);
	UINT GridVBOffset = NULL;
	d3dContext_->IASetVertexBuffers(1, 1, &gridVertBuffer_, &stride, &GridVBOffset);
	d3dContext_->IASetInputLayout(gridInputLayout_);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	d3dContext_->VSSetShader(GRID_VS_, NULL, NULL);
	d3dContext_->PSSetShader(GRID_PS_, NULL, NULL);
	d3dContext_->OMSetBlendState(defaultBlendState_, NULL, 0xFFFFFFFF);
	d3dContext_->VSSetConstantBuffers(1, 1, &WVPConstBuffer_);

	//Update WVP for Grid
	WVP = gridWorldMX * cameraViewMX * projMX;
	perObjCB.WVP = WVP;

	//WVP to Grid VS
	D3D11_MAPPED_SUBRESOURCE gridConstantBufferSubRes;
	ZeroMemory(&gridConstantBufferSubRes, sizeof(gridConstantBufferSubRes));
	d3dContext_->Map(WVPConstBuffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &gridConstantBufferSubRes);
	memcpy(gridConstantBufferSubRes.pData, &perObjCB, sizeof(perObjConstBuffer));
	d3dContext_->Unmap(WVPConstBuffer_, NULL);
	//Draw
	d3dContext_->Draw(numOfGridVertices, 0);

	//Draw Cube
	UINT Cstride = sizeof(_OBJ_VERT_);
	UINT CubeVBOffset = NULL;
	UINT CubeIBOffset = NULL;
	d3dContext_->IASetVertexBuffers(0, 1, &cubeVertBuffer_, &Cstride, &CubeVBOffset);
	d3dContext_->IASetIndexBuffer(cubeIndexBuffer_, DXGI_FORMAT_R32_UINT, CubeIBOffset);
	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(VS_, NULL, NULL);
	d3dContext_->PSSetShader(PS_, NULL, NULL);
	d3dContext_->VSSetConstantBuffers(0, 1, &WVPConstBuffer_);
	
	//Update WVP for Cube
	WVP = cubeWorldMX * cameraViewMX * projMX;
	perObjCB.WVP = WVP;

	//WVP to Cube VS
	D3D11_MAPPED_SUBRESOURCE cubeConstantBufferSubRes;
	ZeroMemory(&cubeConstantBufferSubRes, sizeof(cubeConstantBufferSubRes));
	d3dContext_->Map(WVPConstBuffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &cubeConstantBufferSubRes);
	memcpy(cubeConstantBufferSubRes.pData, &perObjCB, sizeof(perObjConstBuffer));
	d3dContext_->Unmap(WVPConstBuffer_, NULL);

	//Texture Offsets CB to Cube PS
	D3D11_MAPPED_SUBRESOURCE textureOffsetsSubRes;
	ZeroMemory(&textureOffsetsSubRes, sizeof(textureOffsetsSubRes));
	d3dContext_->Map(textureOffsetsConstBuffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &textureOffsetsSubRes);
	memcpy(textureOffsetsSubRes.pData, &textureOffsetCB, sizeof(textureOffsets));
	d3dContext_->Unmap(textureOffsetsConstBuffer_, NULL);
	d3dContext_->PSSetConstantBuffers(0, 1, &textureOffsetsConstBuffer_);
	d3dContext_->PSSetShaderResources(0, 1, &cubeColorMapSRV_);
	d3dContext_->PSSetSamplers(0, 1, &textureSampler_);


	//Draw Back Side of Cube First
	d3dContext_->VSSetShader(VS_, NULL, NULL);
	d3dContext_->PSSetShader(PS_, NULL, NULL);
	d3dContext_->RSSetState(CWCullingModeRS_);
	
	d3dContext_->OMSetBlendState(transparencyBlendState_, NULL, 0xFFFFFFFF);
	d3dContext_->DrawIndexed(totalNumOfVertices, 0, 0);

	//Then Draw Front Side
	d3dContext_->RSSetState(CCWCullingModeRS_);
	d3dContext_->DrawIndexed(totalNumOfVertices, 0, 0);


	//d3dContext_->OMSetBlendState(defaultBlendState_, NULL, 0xFFFFFFFF);
	//*****************************End Drawing*****************************/
	//Present contents of backbuffer to screen
	swapChain_->Present(0, 0);
	return true;
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::AppShutdown()
{
	SAFE_RELEASE(gridInputLayout_);
	SAFE_RELEASE(cubeVertBuffer_);
	SAFE_RELEASE(cubeIndexBuffer_);
	SAFE_RELEASE(gridVertBuffer_);
	SAFE_RELEASE(gridIndexBuffer_);
	SAFE_RELEASE(depthStencilBuffer_);
	SAFE_RELEASE(depthStencilState_);
	SAFE_RELEASE(cubeColorMapTexture_);
	SAFE_RELEASE(cubeColorMapSRV_);
	SAFE_RELEASE(textureSampler_);
	SAFE_RELEASE(transparencyBlendState_);
	SAFE_RELEASE(defaultBlendState_);
	SAFE_RELEASE(CCWCullingModeRS_);
	SAFE_RELEASE(CWCullingModeRS_);
	SAFE_RELEASE(WVPConstBuffer_);
	SAFE_RELEASE(textureOffsetsConstBuffer_);
	SAFE_RELEASE(noAARS_);
	SAFE_RELEASE(VS_);
	SAFE_RELEASE(PS_);
	SAFE_RELEASE(GRID_VS_);
	SAFE_RELEASE(GRID_PS_);
	


	//Shutdown DX
	DXShutDown();
	return true;
}
































float GetIncreasingRadiansOverTime(float deltaTime)
{
	deltaTime = deltaTime * 1.0f;

	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}
	else
	{
		angle += deltaTime;
	}

	return angle;
}



void DEMO_APP::DXShutDown()
{
	d3dContext_->ClearState();
	SAFE_RELEASE(renderTargetView_);
	SAFE_RELEASE(depthStencilView_);
	SAFE_RELEASE(depthStencilBuffer_);
	SAFE_RELEASE(swapChain_);
	SAFE_RELEASE(d3dContext_);
	SAFE_RELEASE(inputLayout_);

#ifdef _DEBUG
	ID3D11Debug *d3dDebug = nullptr;
	d3dDevice_->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug);
	d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

	SAFE_RELEASE(d3dDevice_);
	UnregisterClass(L"DirectXApplication", application);
}

//DX11 Initialization
void DEMO_APP::DX11Init()
{
	UINT driver = 0;
	UINT deviceCreationFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};
	UINT totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT totalFeatureLevels = ARRAYSIZE(featureLevels);

	RECT clientRect;
	GetClientRect(window, &clientRect);
	UINT backBufferWidth = static_cast<UINT>(clientRect.right - clientRect.left);
	UINT backBufferHeight = static_cast<UINT>(clientRect.bottom - clientRect.top);

	//DXGI Swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

#ifdef _DEBUG
	deviceCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[driver], NULL, deviceCreationFlags,
			featureLevels, totalFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc,
			&swapChain_, &d3dDevice_, &featureLevel_, &d3dContext_);

		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}

	//RTV
	ID3D11Texture2D* backBuffer_;
	swapChain_->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer_));
	d3dDevice_->CreateRenderTargetView(backBuffer_, NULL, &renderTargetView_);
	SAFE_RELEASE(backBuffer_);	
	
	//ViewPort
	ZeroMemory(&viewPort_, sizeof(D3D11_VIEWPORT));
	viewPort_.Width = (float)backBufferWidth;
	viewPort_.Height = (float)backBufferHeight;
	viewPort_.MinDepth = 0.0f; //Depth Stencil Buffer Min Z Value
	viewPort_.MaxDepth = 1.0f; //Depth Stencil Buffer Max Z Value
	viewPort_.TopLeftX = 0.0f;
	viewPort_.TopLeftY = 0.0f;

	//DSV
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = backBufferWidth;
	depthStencilDesc.Height = backBufferHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = NULL;
	depthStencilDesc.MiscFlags = NULL;
	d3dDevice_->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer_);

	//Describe Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = NULL;
	d3dDevice_->CreateDepthStencilView(depthStencilBuffer_, &depthStencilViewDesc, &depthStencilView_);

	//Describe Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d3dDevice_->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState_);
	
	////Blend States Desc for OM
	//Source Pixel Transparency
	D3D11_BLEND_DESC transparencyBlendDesc;
	ZeroMemory(&transparencyBlendDesc, sizeof(transparencyBlendDesc));
	transparencyBlendDesc.AlphaToCoverageEnable = false;
	transparencyBlendDesc.IndependentBlendEnable = false; //Only have 1 RTV
	transparencyBlendDesc.RenderTarget[0].BlendEnable = true;
	transparencyBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparencyBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparencyBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparencyBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparencyBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparencyBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	d3dDevice_->CreateBlendState(&transparencyBlendDesc, &transparencyBlendState_);
	//Default Blending - Opaque Option
	D3D11_BLEND_DESC defaultBlendDesc;
	ZeroMemory(&defaultBlendDesc, sizeof(defaultBlendDesc));
	defaultBlendDesc.AlphaToCoverageEnable = false;
	defaultBlendDesc.IndependentBlendEnable = false;
	defaultBlendDesc.RenderTarget[0].BlendEnable = false;
	defaultBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	defaultBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	defaultBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	defaultBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	defaultBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	defaultBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	defaultBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	d3dDevice_->CreateBlendState(&defaultBlendDesc, &defaultBlendState_);
	
	//Cull CCW Triangles
	D3D11_RASTERIZER_DESC cullModeDesc;
	ZeroMemory(&cullModeDesc, sizeof(cullModeDesc));
	cullModeDesc.FillMode = D3D11_FILL_SOLID;
	cullModeDesc.CullMode = D3D11_CULL_BACK;
	cullModeDesc.FrontCounterClockwise = false;
	cullModeDesc.AntialiasedLineEnable = true;
	d3dDevice_->CreateRasterizerState(&cullModeDesc, &CCWCullingModeRS_);

	//Cull CW Triangles
	D3D11_RASTERIZER_DESC cullModeDesc1;
	ZeroMemory(&cullModeDesc1, sizeof(cullModeDesc1));
	cullModeDesc1.FillMode = D3D11_FILL_SOLID;
	cullModeDesc1.CullMode = D3D11_CULL_BACK;
	cullModeDesc1.FrontCounterClockwise = true;
	cullModeDesc1.AntialiasedLineEnable = true;
	d3dDevice_->CreateRasterizerState(&cullModeDesc1, &CWCullingModeRS_);
	
	D3D11_RASTERIZER_DESC noAADesc;
	ZeroMemory(&noAADesc, sizeof(noAADesc));
	noAADesc.AntialiasedLineEnable = false;
	d3dDevice_->CreateRasterizerState(&noAADesc, &noAARS_);
}

//Application Entry Point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance, (WNDPROC)WndProc);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT && myApp.Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	myApp.AppShutdown();
	return EXIT_SUCCESS;
}
void DEMO_APP::Win32Init(HINSTANCE hInst, WNDPROC procFunction)
{
	application = hInst;
	appWndProc = procFunction;

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	RegisterClassEx(&wndClass);

	RECT windowSize = { 0, 0, BACKBUFFER_HEIGHT, BACKBUFFER_WIDTH };
	AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, false);
	auto width = windowSize.right - windowSize.left;
	auto height = windowSize.bottom - windowSize.top;

	window = CreateWindow(L"DirectXApplication",
		appName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		application,
		this);

	ShowWindow(window, SW_SHOW);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		message = WM_DESTROY;
	}

	switch (message)
	{
		case (WM_DESTROY) :
		{
			PostQuitMessage(0);
			break;
		}
		case (WM_SIZE) :
		{
			if (swapChain_)
			{
				//Window Dimensions
				UINT currWidth = LOWORD(lParam);
				UINT currHeight = HIWORD(lParam);

				//Release 
				d3dContext_->ClearState();
				d3dContext_->OMSetRenderTargets(0, NULL, NULL);
				SAFE_RELEASE(renderTargetView_);
				SAFE_RELEASE(depthStencilBuffer_);
				SAFE_RELEASE(depthStencilState_);
				SAFE_RELEASE(depthStencilView_);

				//BackBuffer
				swapChain_->ResizeBuffers(0, currWidth, currHeight, DXGI_FORMAT_UNKNOWN, NULL);
				ID3D11Texture2D* newRenderTarget_;
				swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&newRenderTarget_);
				d3dDevice_->CreateRenderTargetView(newRenderTarget_, NULL, &renderTargetView_);
				SAFE_RELEASE(newRenderTarget_);

				//DepthBuffer
				D3D11_TEXTURE2D_DESC depthStencilDesc;
				ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
				depthStencilDesc.Width = currWidth;
				depthStencilDesc.Height = currHeight;
				depthStencilDesc.MipLevels = 1;
				depthStencilDesc.ArraySize = 1;
				depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
				depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthStencilDesc.CPUAccessFlags = NULL;
				depthStencilDesc.MiscFlags = NULL;
				d3dDevice_->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer_);

				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
				ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
				depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = NULL;
				d3dDevice_->CreateDepthStencilView(depthStencilBuffer_, &depthStencilViewDesc, &depthStencilView_);

				//ViewPort
				D3D11_VIEWPORT viewPort;
				viewPort.Width = (float)currWidth;
				viewPort.Height = (float)currHeight;
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.0f;
				viewPort.TopLeftX = 0.0f;
				viewPort.TopLeftY = 0.0f;
				d3dContext_->RSSetViewports(1, &viewPort);

				//Proj Matrix
				float aspectRatio = (float)currWidth / (float)currHeight;
				projMX = XMMatrixPerspectiveFovLH(XM_PIDIV2, aspectRatio, 0.05f, 1000.f);
				
				d3dContext_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);
			}
			break;
		}

		default:
		{
			break;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}