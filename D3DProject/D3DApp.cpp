#include "D3DBase.h"
#include "GeometryFactory.h"
#include "Camera.h"
#include "VertexShader.csh"
#include "PixelShader.csh"
#include "SkyBoxVertexShader.csh"
#include "SkyBoxPixelShader.csh"

class D3DApp : public D3DBase
{
	//Class Functions
	public:
		D3DApp(HINSTANCE hInstance);
		~D3DApp();

		//Boilerplate Code Abstracted Away
		bool Init();
		void ResizeWindow();
		
		//Implementations Must Be Provided
		void Update(float deltaTime);
		void Draw();

		void OnMouseDown(WPARAM theButton, int x, int y);
		void OnMouseUp(WPARAM theButton, int x, int y);
		void OnMouseMove(WPARAM theButton, int x, int y);

	private:
		void BuildStateConfigurations();
		void BuildGeometryAndBuffers();
		void CompileShaders();
		void DefineInputLayouts();

	//Class Data Members
	private:
		GeometryFactory GeoFactory; //Object Factory v0.01, Static, ToDo: Model Loader encapsulation...
		Camera mCamera;
		//*******Scene Objects*******//
		////Cube
		//Info
		PCMeshData cubeInfo; //Local Space
		XMFLOAT4X4 mCube;  //World Space
		//Buffers
		ID3D11Buffer* mCubeVB;
		ID3D11Buffer* mCubeIB;
		ID3D11Buffer* mPerObjWVP;
		//Shaders
		ID3D11VertexShader* mCubeVS;
		ID3D11PixelShader* mCubePS;

		////Grid
		//Info
		PCMeshData gridInfo; //Local Space
		XMFLOAT4X4 mGrid;  //World Space
		//Buffers
		ID3D11Buffer* mGridVB;
		ID3D11Buffer* mPerObjWVPGrid;

		////Star
		//Info
		PCMeshData starInfo; //Local Space
		XMFLOAT4X4 mStar;  //World Space
		//Buffers
		ID3D11Buffer* mStarVB;
		ID3D11Buffer* mStarIB;
		ID3D11Buffer* mPerObjWVPStar;
		//Orientation Matrices
		XMMATRIX mStarRotationMX;
		
		////SkyBox
		//Info
		PMeshData skyBoxInfo; //Local Space
		XMFLOAT4X4 mSkyBox;  //World Space
		const wchar_t* mSkyBoxFileName;
		//Buffers
		ID3D11Buffer* mSkyBoxVB;
		ID3D11Buffer* mSkyBoxIB;
		ID3D11Buffer* mPerObjWVPSkyBox;
		//Shaders
		ID3D11VertexShader* mSkyBoxVS;
		ID3D11PixelShader* mSkyBoxPS;
		//Texture
		ID3D11Texture2D* mCubeMapTexture;
		//Resource Views
		ID3D11ShaderResourceView* mCubeMapSRV;

		////Current Object
		XMMATRIX mWorld;

		//*******D3D Objects*******//
		//Creation Time Objects (InputLayouts, RasterStates, etc)
		ID3D11InputLayout* mPosIL;
		ID3D11InputLayout* mPosColIL;
		ID3D11RasterizerState* mNoCullingRS;
		ID3D11DepthStencilState* mLessEqualDSS;
		ID3D11SamplerState* mLinearSSO;

		//*******CPU Objects*******//
		//CPU Side Const Buffers
		cbPerObject mObjCB;
		cbPerObject mObjCBGrid;
		cbPerObject mObjCBStar;
		cbPerObject mObjCBSkyBox;
		//Win32
		POINT mPrevMousePos;
};

//Just Because
float angle = 0.0f;
float speedScalar = 2.0f;
float GetAngle(float deltaTime)
{
	deltaTime = deltaTime * speedScalar;
	angle += deltaTime;
	return angle;
}


//Application Entry Point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	D3DApp myApp(hInstance);

	if (!myApp.Init())
	{
		return 0;
	}

	return myApp.Run();
}

D3DApp::D3DApp(HINSTANCE hInstance) :
D3DBase(hInstance),
mCubeVB(NULL),
mCubeIB(NULL),
mCubeVS(NULL),
mCubePS(NULL),
mPosIL(NULL),
mPosColIL(NULL),
mGridVB(NULL),
mStarVB(NULL),
mStarIB(NULL)
{
	mWorld = XMMatrixIdentity();

	XMMATRIX IdentityMX = XMMatrixIdentity();
	XMStoreFloat4x4(&mCube, IdentityMX);
	XMStoreFloat4x4(&mGrid, IdentityMX);
	XMStoreFloat4x4(&mStar, IdentityMX);
	XMStoreFloat4x4(&mSkyBox, IdentityMX);

	//Camera Initialization
	mCamera.SetPosition(0.0f, 1.0f, -5.5f);

	//Grid's Initial Transformation
	XMMATRIX GridTransformMX = XMMatrixIdentity();
	XMMATRIX GridScalingMX = XMMatrixScaling(3.0f, 1.0f, 3.0f);
	XMMATRIX GridTranslationMX = XMMatrixTranslation(0.0f, -1.0f, 0.0f);
	GridTransformMX = XMMatrixMultiply(GridScalingMX, GridTranslationMX);
	XMStoreFloat4x4(&mGrid, GridTransformMX);

	//Star's Initial Transformation
	XMMATRIX StarTransformMX = XMMatrixIdentity();
	XMMATRIX StarScalingMX = XMMatrixScaling(0.15f, 0.15f, 0.15f);
	XMMATRIX StarTranslationMX = XMMatrixTranslation(0.0f, 3.0f, 0.0f);
	StarTransformMX = XMMatrixMultiply(StarScalingMX, StarTranslationMX);
	XMStoreFloat4x4(&mStar, StarTransformMX);

	//SkyBox Initialization
	mSkyBoxFileName = L"trippySkybox.dds";
}

D3DApp::~D3DApp()
{
	SAFE_RELEASE(mCubeVB);
	SAFE_RELEASE(mCubeIB);
	SAFE_RELEASE(mCubeVS);
	SAFE_RELEASE(mCubePS);
	SAFE_RELEASE(mPosIL);
	SAFE_RELEASE(mPerObjWVP);

	SAFE_RELEASE(mGridVB);
	SAFE_RELEASE(mPerObjWVPGrid);

	SAFE_RELEASE(mStarVB);
	SAFE_RELEASE(mStarIB);
	SAFE_RELEASE(mPerObjWVPStar);

	SAFE_RELEASE(mSkyBoxVB);
	SAFE_RELEASE(mSkyBoxIB);
	SAFE_RELEASE(mSkyBoxVS);
	SAFE_RELEASE(mSkyBoxPS);
	SAFE_RELEASE(mCubeMapSRV);
	SAFE_RELEASE(mPerObjWVPSkyBox);

	SAFE_RELEASE(mNoCullingRS);
	SAFE_RELEASE(mLessEqualDSS);
	SAFE_RELEASE(mLinearSSO);
	SAFE_RELEASE(mPosColIL);
}

bool D3DApp::Init()
{
	if (!D3DBase::Init())
	{
		return false;
	}

	BuildStateConfigurations();
	BuildGeometryAndBuffers();
	CompileShaders();
	DefineInputLayouts();

	return true;
}

void D3DApp::ResizeWindow()
{
	D3DBase::ResizeWindow();
	mCamera.SetFrustum(XM_PIDIV2, AspectRatio(), 0.5f, 1000.0f);
}

void D3DApp::Update(float deltaTime)
{

	////User Input
	//Camera
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mCamera.Step(deltaTime);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		mCamera.Step(-deltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		mCamera.Strafe(-deltaTime);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		mCamera.Strafe(deltaTime);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		mCamera.VFly(deltaTime);
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		mCamera.VFly(-deltaTime);
	}
	mCamera.UpdateViewMatrix();
	XMMATRIX viewProj = mCamera.GetViewProj();
	


	////Object Updates
	//Cube
	mWorld = XMLoadFloat4x4(&mCube);
	XMMATRIX WVP = XMMatrixMultiply(mWorld, viewProj);
	XMStoreFloat4x4(&mObjCB.WVP, WVP);
	D3D11_MAPPED_SUBRESOURCE cbSR;
	ZeroMemory(&cbSR, sizeof(cbSR));
	mD3DDeviceContext->Map(mPerObjWVP, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbSR);
	memcpy(cbSR.pData, &mObjCB, sizeof(cbPerObject));
	mD3DDeviceContext->Unmap(mPerObjWVP, 0);

	//Grid
	mWorld = XMLoadFloat4x4(&mGrid);
	XMMATRIX WVP2 = XMMatrixMultiply(mWorld, viewProj);
	XMStoreFloat4x4(&mObjCBGrid.WVP, WVP2);
	ZeroMemory(&cbSR, sizeof(cbSR));
	mD3DDeviceContext->Map(mPerObjWVPGrid, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbSR);
	memcpy(cbSR.pData, &mObjCBGrid, sizeof(cbPerObject));
	mD3DDeviceContext->Unmap(mPerObjWVPGrid, 0);

	//Star
	mStarRotationMX = XMMatrixRotationY(GetAngle(deltaTime));
	mWorld = XMMatrixMultiply(mStarRotationMX, XMLoadFloat4x4(&mStar));
	XMMATRIX WVP3 = XMMatrixMultiply(mWorld, viewProj);
	XMStoreFloat4x4(&mObjCBStar.WVP, WVP3);
	ZeroMemory(&cbSR, sizeof(cbSR));
	mD3DDeviceContext->Map(mPerObjWVPStar, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbSR);
	memcpy(cbSR.pData, &mObjCBStar, sizeof(cbPerObject));
	mD3DDeviceContext->Unmap(mPerObjWVPStar, 0);

	//SkyBox
	XMVECTOR skyBoxPos = mCamera.GetPositionXM();
	XMMATRIX SkyBoxTranslationMX = XMMatrixTranslation(XMVectorGetX(skyBoxPos), XMVectorGetY(skyBoxPos), XMVectorGetZ(skyBoxPos));
	XMStoreFloat4x4(&mSkyBox, SkyBoxTranslationMX);
	mWorld = XMLoadFloat4x4(&mSkyBox);
	XMMATRIX WVP4 = XMMatrixMultiply(mWorld, viewProj);
	XMStoreFloat4x4(&mObjCBSkyBox.WVP, WVP4);
	D3D11_MAPPED_SUBRESOURCE cbSBSR;
	ZeroMemory(&cbSBSR, sizeof(cbSBSR));
	mD3DDeviceContext->Map(mPerObjWVPSkyBox, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbSBSR);
	memcpy(cbSBSR.pData, &mObjCBSkyBox, sizeof(cbPerObject));
	mD3DDeviceContext->Unmap(mPerObjWVPSkyBox, 0);
}

void D3DApp::Draw()
{	
	assert(mD3DDevice);
	assert(mD3DDeviceContext);
	float mClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mD3DDeviceContext->ClearRenderTargetView(mRenderTargetView, mClearColor);
	mD3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Cube's Render Options
	UINT stride = sizeof(PosColVertex);
	UINT offset = 0;
	mD3DDeviceContext->IASetInputLayout(mPosColIL);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(1, 1, &mCubeVB, &stride, &offset);
	mD3DDeviceContext->IASetIndexBuffer(mCubeIB, DXGI_FORMAT_R32_UINT, NULL);
	mD3DDeviceContext->VSSetConstantBuffers(1, 1, &mPerObjWVP);
	mD3DDeviceContext->VSSetShader(mCubeVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mCubePS, NULL, 0);
	mD3DDeviceContext->DrawIndexed(cubeInfo.ibCount, 0, 0);

	//Grid's Render Options
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	mD3DDeviceContext->IASetVertexBuffers(1, 1, &mGridVB, &stride, &offset);
	mD3DDeviceContext->VSSetConstantBuffers(1, 1, &mPerObjWVPGrid);
	mD3DDeviceContext->Draw(gridInfo.vbCount, 0);

	//Star's Render Options
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(1, 1, &mStarVB, &stride, &offset);
	mD3DDeviceContext->IASetIndexBuffer(mStarIB, DXGI_FORMAT_R32_UINT, NULL);
	mD3DDeviceContext->VSSetConstantBuffers(1, 1, &mPerObjWVPStar);
	mD3DDeviceContext->DrawIndexed(starInfo.ibCount, 0, 0);

	//SkyBox's Render Options
	UINT strideSB = sizeof(PosVertex);
	mD3DDeviceContext->IASetInputLayout(mPosIL);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(0, 1, &mSkyBoxVB, &strideSB, &offset);
	mD3DDeviceContext->IASetIndexBuffer(mSkyBoxIB, DXGI_FORMAT_R32_UINT, NULL);
	mD3DDeviceContext->VSSetShader(mSkyBoxVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mSkyBoxPS, NULL, 0);
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &mPerObjWVPSkyBox);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mCubeMapSRV);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mLinearSSO);
	mD3DDeviceContext->RSSetState(mNoCullingRS);
	mD3DDeviceContext->OMSetDepthStencilState(mLessEqualDSS, 0);
	//mD3DDeviceContext->DrawIndexed(skyBoxInfo.ibCount, 0, 0);

	//Reset Render Options to Default
	mD3DDeviceContext->RSSetState(NULL);
	mD3DDeviceContext->OMSetDepthStencilState(NULL, 0);

	mSwapChain->Present(0, 0);
}

//Might concatenate buffers once this gets out of hand.
void D3DApp::BuildGeometryAndBuffers()
{
	//Cube
	GeoFactory.GenerateCube(cubeInfo);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = cubeInfo.vbSize;

	D3D11_SUBRESOURCE_DATA vbid;
	ZeroMemory(&vbid, sizeof(vbid));
	vbid.pSysMem = &cubeInfo.Vertices[0];
	mD3DDevice->CreateBuffer(&vbd, &vbid, &mCubeVB);

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = cubeInfo.ibSize;

	D3D11_SUBRESOURCE_DATA ibid;
	ZeroMemory(&ibid, sizeof(ibid));
	ibid.pSysMem = &cubeInfo.Indices[0];
	mD3DDevice->CreateBuffer(&ibd, &ibid, &mCubeIB);

	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(cbPerObject);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mD3DDevice->CreateBuffer(&cbd, NULL, &mPerObjWVP);

	//Grid
	GeoFactory.GenerateGrid(gridInfo);
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = gridInfo.vbSize;

	ZeroMemory(&vbid, sizeof(vbid));
	vbid.pSysMem = &gridInfo.Vertices[0];
	mD3DDevice->CreateBuffer(&vbd, &vbid, &mGridVB);
	mD3DDevice->CreateBuffer(&cbd, NULL, &mPerObjWVPGrid);

	//Star
	GeoFactory.GenerateStar(starInfo);
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = starInfo.vbSize;

	ZeroMemory(&vbid, sizeof(vbid));
	vbid.pSysMem = &starInfo.Vertices[0];
	mD3DDevice->CreateBuffer(&vbd, &vbid, &mStarVB);

	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = starInfo.ibSize;

	ZeroMemory(&ibid, sizeof(ibid));
	ibid.pSysMem = &starInfo.Indices[0];
	mD3DDevice->CreateBuffer(&ibd, &ibid, &mStarIB);
	mD3DDevice->CreateBuffer(&cbd, NULL, &mPerObjWVPStar);

	//SkyBox
	GeoFactory.GenerateSkyBox(skyBoxInfo);
	CreateDDSTextureFromFile(mD3DDevice, mSkyBoxFileName, NULL, &mCubeMapSRV);

	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = skyBoxInfo.vbSize;

	ZeroMemory(&vbid, sizeof(vbid));
	vbid.pSysMem = &skyBoxInfo.Vertices[0];
	mD3DDevice->CreateBuffer(&vbd, &vbid, &mSkyBoxVB);

	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = skyBoxInfo.ibSize;

	ZeroMemory(&ibid, sizeof(ibid));
	ibid.pSysMem = &skyBoxInfo.Indices[0];
	mD3DDevice->CreateBuffer(&ibd, &ibid, &mSkyBoxIB);

	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(cbPerObject);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mD3DDevice->CreateBuffer(&cbd, NULL, &mPerObjWVPSkyBox);

}

void D3DApp::CompileShaders()
{
	mD3DDevice->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &mCubeVS);
	mD3DDevice->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &mCubePS);
	mD3DDevice->CreateVertexShader(SkyBoxVertexShader, sizeof(SkyBoxVertexShader), NULL, &mSkyBoxVS);
	mD3DDevice->CreatePixelShader(SkyBoxPixelShader, sizeof(SkyBoxPixelShader), NULL, &mSkyBoxPS);

}

void D3DApp::DefineInputLayouts()
{
	D3D11_INPUT_ELEMENT_DESC posVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(posVertexDesc, ARRAYSIZE(posVertexDesc), SkyBoxVertexShader, sizeof(SkyBoxVertexShader), &mPosIL);

	D3D11_INPUT_ELEMENT_DESC posColVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(posColVertexDesc, ARRAYSIZE(posColVertexDesc), VertexShader, sizeof(VertexShader), &mPosColIL);

	/*D3D11_INPUT_ELEMENT_DESC normalVertexDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(normalVertexDesc, ARRAYSIZE(normalVertexDesc), SkyBoxVertexShader, sizeof(SkyBoxVertexShader), &mPosColIL);*/
}

void D3DApp::BuildStateConfigurations()
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	mD3DDevice->CreateSamplerState(&sd, &mLinearSSO);

	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	mD3DDevice->CreateRasterizerState(&rd, &mNoCullingRS);

	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	mD3DDevice->CreateDepthStencilState(&dsd, &mLessEqualDSS);
}

void D3DApp::OnMouseDown(WPARAM theButton, int x, int y)
{
	mPrevMousePos.x = x;
	mPrevMousePos.y = y;
	
	//All your input are belong to us
	SetCapture(mMainWnd);
}

void D3DApp::OnMouseUp(WPARAM theButton, int x, int y)
{
	ReleaseCapture();
}

void D3DApp::OnMouseMove(WPARAM theButton, int x, int y)
{
#if 0
	//Chaining Rotation Matrices Method
	////LMB - Look Left & Right
	//if (theButton & MK_LBUTTON)
	//{
	//	float currYRotationAngle = XMConvertToRadians(-0.05f * static_cast<float>(x - mPrevMousePos.x));
	//	mCamera.RotateY(currYRotationAngle);
	//	
	//}
	//mPrevMousePos.x = x;
	//
	//
	////RMB - Look Up & Down
	//if (theButton & MK_RBUTTON)
	//{
	//	float currXRotationAngle = XMConvertToRadians(-0.05f * static_cast<float>(y - mPrevMousePos.y));
	//	mCamera.Pitch(currXRotationAngle);
	//}
	//mPrevMousePos.y = y;
#endif

	//Rotation Matrices emulated via Quaternion
	if (theButton & MK_LBUTTON)
	{
		mCamera.mReverseMult = true;
		float currYRotAngle = XMConvertToRadians(-0.05f * static_cast<float>(x - mPrevMousePos.x));
		float currXRotAngle = XMConvertToRadians(-0.05f * static_cast<float>(y - mPrevMousePos.y));
		mCamera.Look(currXRotAngle, currYRotAngle, 0.0f);
	}
	mPrevMousePos.x = x;
	mPrevMousePos.y = y;
}