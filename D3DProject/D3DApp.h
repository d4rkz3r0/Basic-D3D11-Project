#include "D3DBase.h"
#include "GeometryFactory.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "CBRefs.h"
#include "ShaderRefs.h"

class D3DApp : public D3DBase
{
public:
	D3DApp(HINSTANCE hInstance);
	~D3DApp();

	bool Init();
	void ResizeWindow();
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
	void GetUserInput(float deltaTime);
	void UpdateCamera(float deltaTime);
	void UpdateSkybox(float deltaTime);
	void UpdateLights(float deltaTime);
	void BuildSkyBox(const wchar_t* fileName);
	float GetAngle(float deltaTime);

private:
	////////////////////////////////////////////////////////////
	// Pipeline Factories
	////////////////////////////////////////////////////////////
	GeometryFactory GeoFactory;
	
	////////////////////////////////////////////////////////////
	// World Objects
	////////////////////////////////////////////////////////////
	// Current Object
	XMMATRIX mWorld;

	// Camera
	Camera mCamera;
	XMMATRIX mView;
	XMMATRIX mProj;
	XMMATRIX mViewProj;
	XMMATRIX mWorldViewProj;
	float mCamScalar = 10.0f;
	float mPointLightScalar = 20.0f;

	// Cube
	PCMeshData cubeInfo;
	XMFLOAT4X4 mCube;
	//Buffers
	ID3D11Buffer* mCubeVB;
	ID3D11Buffer* mCubeIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBuffer;
	//Shaders
	ID3D11VertexShader* mSimpleVS;
	ID3D11PixelShader* mSimplePS;
	
	// SkyBox
	PMeshData skyBoxInfo;
	XMFLOAT4X4 mSkyBox;  
	//Matrices
	XMMATRIX mSkyBoxTranslationMX;
	XMMATRIX mSkyBoxScalingMX;
	XMMATRIX mSkyBoxTransformationMX;
	const wchar_t* mSkyBoxFileName = L"windowsSkyBox.dds";
	//Buffers
	ID3D11Buffer* mTSkyBoxVB;
	ID3D11Buffer* mTSkyBoxIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBufferTSB;
	//Shaders
	ID3D11VertexShader* mSkyBoxVS;
	ID3D11PixelShader* mSkyBoxPS;
	//Resource Views
	ID3D11ShaderResourceView* mTrippySkyBoxSRV;

	// TestModel
	string mQuadFileName = "FloorQuad.obj";
	const wchar_t* mQuadTextureFileName = L"catShocked.dds";
	FMeshData mQuadMesh;
	XMFLOAT4X4 mQuad;
	XMFLOAT3X3 mQuadWorldSpaceMX;
	XMMATRIX mQuadScalingMX;
	XMMATRIX mQuadTransformationMX;
	ID3D11Buffer* mQuadVB;
	ID3D11Buffer* mQuadIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBufferQuad;
	ID3D11ShaderResourceView* mQuadSRV;
	ID3D11VertexShader* mTextureLightVS;
	ID3D11PixelShader* mTextureLightPS;

	// Stalker CS Model
	string mStalkerFileName = "stalker_cs_rukzak_3.obj";
	const wchar_t* mStalkerTextureFileName = L"clearsc.dds";
	FMeshData mStalkerMesh;
	XMFLOAT4X4 mStalker;
	XMFLOAT3X3 mStalkerWorldSpaceMX;
	XMMATRIX mStalkerScalingMX;
	XMMATRIX mStalkerRotationMX;
	XMMATRIX mStalkerTranslationMX;
	XMMATRIX mStalkerTransformationMX;
	ID3D11Buffer* mStalkerVB;
	ID3D11Buffer* mStalkerIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBufferStalker;
	ID3D11ShaderResourceView* mStalkerSRV;

	// Lights
	XMFLOAT4 mGlobalAmbient; 
	ConstantBuffer<DirectionalLight> mDirectionalLightInfo;
	ConstantBuffer<PointLight> mPointLightInfo;
	XMFLOAT3 mPointLightPos;
	float mCurrPointLightX = 0.0f;
	float mCurrPointLightY = 3.0f;
	float mCurrPointLightZ = 0.0f;
	
	//Tree Instancing
	ID3D11Buffer* mTreeInstanceBuffer;


	//VERTEX SHADER Const Bufer INFO

	//PIXEL SHADER Const Buffer INFO
	float mUVDefaultScalar = 1.0f;
	float mUVCatQuadScalar = 4.0f;

	//MISC SHADER INFO
	ConstantBuffer<cbMiscInfo> mShaderConstantBufferInfo;
	XMFLOAT4 shaderInfoDataBlock1;
	XMFLOAT4 shaderInfoDataBlock2;
	XMFLOAT4 shaderInfoDataBlock3;
	XMFLOAT4 shaderInfoDataBlock4;



	/////////////////////////////////////////////////////////////
	// D3D Creation Time Objects
	////////////////////////////////////////////////////////////
	//Input Layouts
	ID3D11InputLayout* mPosInputLayout;
	ID3D11InputLayout* mPosColInputLayout;
	ID3D11InputLayout* mNormVertexInputLayout;
	ID3D11InputLayout* mFullVertexInputLayout;
	ID3D11InputLayout* mGSInputLayout;
	//Raster and Blend States
	ID3D11RasterizerState* mDefaultRasterState;
	ID3D11RasterizerState* mNoCullRasterState;
	ID3D11RasterizerState* mWireFrameRasterState;
	ID3D11BlendState* mAlphaToCoverageBlendState;
	ID3D11BlendState* mTransparentBlendState;
	ID3D11BlendState* mOpaqueBlendState;
	//Depth/Stencil Buffer States
	ID3D11DepthStencilState* mLessEqualDSS;
	//Sampler States
	ID3D11SamplerState* mLinearSamplerState;

	/////////////////////////////////////////////////////////////
	// Win32 & Misc Variables
	////////////////////////////////////////////////////////////
	//Win32
	POINT mPrevMousePos;
	//XTime
	float angle = 0.0f;
	float speedScalar = 2.0f;
};