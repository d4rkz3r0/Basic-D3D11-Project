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
	//Current Object
	XMMATRIX mWorld;

	//Camera
	Camera mCamera;
	XMMATRIX mView;
	XMMATRIX mProj;
	XMMATRIX mViewProj;
	XMMATRIX mWorldViewProj;
	float mCamScalar = 20.0f;

	//Cube
	PCMeshData cubeInfo;
	XMFLOAT4X4 mCube;
	ID3D11Buffer* mCubeVB;
	ID3D11Buffer* mCubeIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBuffer;
	
	//SkyBox
	PMeshData skyBoxInfo;
	XMFLOAT4X4 mSkyBox;  
	XMMATRIX mSkyBoxTranslationMX;
	XMMATRIX mSkyBoxScalingMX;
	XMMATRIX mSkyBoxTransformationMX;
	const wchar_t* mSkyBoxFileName = L"nightBox.dds";
	ID3D11Buffer* mSkyBoxVB;
	ID3D11Buffer* mSkyBoxIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBufferSB;
	ID3D11ShaderResourceView* mSkyBoxSRV;

	//Ground Quad
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
	
	//Stalker
	string mStalkerFileName = "stalkerModel.obj";
	const wchar_t* mStalkerTextureFileName = L"stalkerUVTexture.dds";
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

	//Lights
	ConstantBuffer<DirectionalLight> mDirectionalLightInfo;
	ConstantBuffer<PointLight> mPointLightInfo;
	ConstantBuffer<SpotLight> mSpotLightInfo;
	float mCurrPointLightPosX = 0.0f;
	float mCurrPointLightPosY = 1.0f;
	float mCurrPointLightPosZ = 0.0f;
	float mPointLightScalar = 20.0f;

	//CyberDemon
	const wchar_t* mCyberDemonTextureFileName = L"cyberDemonUVTexture.dds";
	const int mNumOfCyberDemons = 100;
	ID3D11Buffer* mCyberDemonVB;
	ID3D11Buffer* mCyberDemonInstanceBuffer;
	ID3D11ShaderResourceView* mCyberDemonSRV;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstantBufferShaderInfo;
	XMFLOAT4X4 mCyberDemons;
	XMMATRIX mCyberDemonTranslationMX;
	XMMATRIX mCyberDemonTransformationMX;


	//Materials Per Object
	ConstantBuffer<cbPerObjectMaterial> mObjectMaterialConstBufferFactors;
	
	//Pixel Shader Misc Info
	ConstantBuffer<cbMiscInfo> mShaderConstantBufferInfo;
	XMFLOAT4 shaderInfoDataBlock1;
	XMFLOAT4 shaderInfoDataBlock2;
	XMFLOAT4 shaderInfoDataBlock3;
	XMFLOAT4 shaderInfoDataBlock4;
	float mUVDefaultScalar = 1.0f;
	float mUVTiledGroundScalar = 16.0f;
	float mUVCyberDemonBillBoardUVx = 0.5f;
	float mUVCyberDemonBillBoardUVy = 1.0f;


	/////////////////////////////////////////////////////////////
	// D3D Creation Time Objects
	////////////////////////////////////////////////////////////
	//Input Layouts
	ID3D11InputLayout* mPosInputLayout;
	ID3D11InputLayout* mPosColInputLayout;
	ID3D11InputLayout* mNormVertexInputLayout;
	ID3D11InputLayout* mFullVertexInputLayout;
	ID3D11InputLayout* mBillBoardInputLayout;
	
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
	//Shaders
	ID3D11VertexShader* mSimpleVS;
	ID3D11PixelShader* mSimplePS;
	ID3D11VertexShader* mSkyBoxVS;
	ID3D11PixelShader* mSkyBoxPS;
	ID3D11VertexShader* mTextureLightVS;
	ID3D11PixelShader* mTextureLightPS;
	ID3D11VertexShader* mBillBoardVS;
	ID3D11GeometryShader* mBillBoardGS;
	ID3D11PixelShader* mBillBoardPS;

	/////////////////////////////////////////////////////////////
	// Win32 & Misc Variables
	////////////////////////////////////////////////////////////
	//Win32
	POINT mPrevMousePos;
	//XTime
	float angle = 0.0f;
	float speedScalar = 2.0f;
};