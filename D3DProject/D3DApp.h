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
	void BuildOtherRenderTargets();
	void CompileShaders();
	void DefineInputLayouts();
	void SceneCubeUpdate();
	void PostProcessQuadUpdate();
	void GetUserInput(float deltaTime);
	void AnimateBillBoards(float& UVx, float& UVy, UINT flag);
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
	const wchar_t* mSkyBox2FileName = L"dayBox.dds";
	ID3D11Buffer* mSkyBoxVB;
	ID3D11Buffer* mSkyBoxIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBufferSB;
	ID3D11ShaderResourceView* mSkyBoxSRV;
	ID3D11ShaderResourceView* mSkyBox2SRV;

	//Ground Quad
	string mQuadFileName = "FloorQuad.obj";
	const wchar_t* mQuadTextureFileName = L"catShocked.dds";
	const wchar_t* mAltQuadTextureFileName = L"catBreathing.dds";
	FMeshData mQuadMesh;
	XMFLOAT4X4 mQuad;
	XMFLOAT3X3 mQuadWorldSpaceMX;
	XMMATRIX mQuadScalingMX;
	XMMATRIX mQuadTransformationMX;
	ID3D11Buffer* mQuadVB;
	ID3D11Buffer* mQuadIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstBufferQuad;
	ID3D11ShaderResourceView* mQuadSRV;
	ID3D11ShaderResourceView* mAltQuadSRV;
	
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
	UINT lightFlag = 0; 
	ConstantBuffer<DirectionalLight> mDirectionalLightInfo;
	ConstantBuffer<PointLight> mPointLightInfo;
	ConstantBuffer<SpotLight> mSpotLightInfo;
	float mCurrPointLightPosX = 0.0f;
	float mCurrPointLightPosY = 1.0f;
	float mCurrPointLightPosZ = 0.0f;
	float mPointLightScalar = 20.0f;

	float mCurrDirLightDirX = 0.0f;
	float mCurrDirLightDirY = -0.25f;
	float mCurrDirLightDirZ = 0.0f;
	float mDirLightScalar = 10.0f;

	//CyberDemon
	const wchar_t* mCyberDemonTextureFileName = L"cyberDemonUVTexture.dds";
	const int mNumOfCyberDemons = 20;
	ID3D11Buffer* mCyberDemonVB;
	ID3D11Buffer* mCyberDemonInstanceBuffer;
	ID3D11ShaderResourceView* mCyberDemonSRV;
	ConstantBuffer<cbPerObjectTransformation> mObjectConstantBufferShaderInfo;
	XMFLOAT4X4 mCyberDemons;
	XMMATRIX mCyberDemonTranslationMX;
	XMMATRIX mCyberDemonTransformationMX;

	// Instancing //
	//Mario 64 Tree
	MeshData mTreeMesh;
	XMFLOAT4X4 mTree;
	const wchar_t* mTreeTextureFileName = L"mario64Tree.dds";
	const int mNumInstances = 15;
	ID3D11Buffer* mTreeVB;
	ID3D11Buffer* mTreeIB;
	ID3D11Buffer* mTreeInstBuffer;
	ID3D11ShaderResourceView* mTreeSRV;
	vector<InstanceData> mInstancedTreeVec;
	ConstantBuffer<cbPerObjectTransformation> mInstanceConstantBufferShaderInfo;

	// RTT //
	ID3D11Texture2D* mRenderTargetTexture;
	ID3D11RenderTargetView* mRenderTargetRTV;
	ID3D11DepthStencilView* mRenderTargetDSV;
	ID3D11ShaderResourceView* mRenderTargetSRV;
	ID3D11Texture2D* mRenderTargetDepthStencilBuffer;
	D3D11_VIEWPORT mRenderTargetViewPort;

	//Scene Cube
	FMeshData mSceneCubeInfo;
	string mSceneCubeFileName = "sceneCube.obj";
	XMFLOAT4X4 mSceneCube;
	ID3D11Buffer* mSceneCubeVB;
	ID3D11Buffer* mSceneCubeIB;
	ConstantBuffer<cbPerObjectTransformation> mObjectSceneCubeConstBuffer;
	XMMATRIX mSceneCubeScalingMX;
	XMMATRIX mSceneCubeRotationMX;
	XMMATRIX mSceneCubeTranslationMX;
	XMMATRIX mSceneCubeTransformationMX;
	
	//Full Screen Quad (Post Processing)
	MeshData mFullScreenQuadInfo;
	ID3D11Buffer* mFullScreenQuadVB;
	ID3D11Buffer* mFullScreenQuadIB;
	ID3D11RenderTargetView* mFullScreenQuadRTV;
	ID3D11DepthStencilView* mFullScreenQuadDSV;
	ID3D11ShaderResourceView* mFullScreenQuadSRV;
	ID3D11Texture2D* mFullScreenQuadTexture;
	ID3D11Texture2D* mFullScreenQuadDepthStencilBuffer;
	D3D11_VIEWPORT mFullScreenQuadViewPort;
	float mFSQuadScale = 1.0f;

	ConstantBuffer<cbPerObjectTransformation> mFullScreenQuadConstBuffer;
	XMFLOAT4X4 mFullScreenQuad;
	XMMATRIX mFullScreenQuadViewMX;
	XMMATRIX mFullScreenQuadViewProj;
	XMMATRIX mFullScreenQuadScalingMX;
	XMMATRIX mFullScreenQuadTranslationMX;
	XMMATRIX mFullScreenQuadTransformationMX;
	
	
	

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
	float mUVCyberDemonBillBoardUVx = 1.0f;
	float mUVCyberDemonBillBoardUVy = 1.0f;


	/////////////////////////////////////////////////////////////
	// D3D Creation Time Objects
	////////////////////////////////////////////////////////////
	//Input Layouts
	ID3D11InputLayout* mPosInputLayout;
	ID3D11InputLayout* mPosColInputLayout;
	ID3D11InputLayout* mVertexInputLayout;
	ID3D11InputLayout* mFullVertexInputLayout;
	ID3D11InputLayout* mPostProcessingInputLayout;
	ID3D11InputLayout* mBillBoardInputLayout;
	ID3D11InputLayout* mInstancedInputLayout;
	
	//Raster and Blend States
	ID3D11RasterizerState* mDefaultRasterState;
	ID3D11RasterizerState* mNoCullRasterState;
	ID3D11RasterizerState* mCWCullRasterState;
	ID3D11RasterizerState* mCCWCullRasterState;
	ID3D11RasterizerState* mWireFrameRasterState;
	ID3D11BlendState* mAlphaToCoverageBlendState;
	ID3D11BlendState* mTransparentBlendState;
	ID3D11BlendState* mOpaqueBlendState;
	//Depth/Stencil Buffer States
	ID3D11DepthStencilState* mLessEqualDSS;
	//Sampler States
	ID3D11SamplerState* mAnisoSamplerState;
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
	ID3D11VertexShader* mTreeInstanceVS;
	ID3D11PixelShader* mTreeInstancePS;
	ID3D11VertexShader* mPostProcessingVS;
	ID3D11PixelShader* mPostProcessingPS;

	/////////////////////////////////////////////////////////////
	// Win32 & Misc Variables
	////////////////////////////////////////////////////////////
	//Win32
	POINT mPrevMousePos;
	//XTime
	float angle = 0.0f;
	float speedScalar = 5.0f;
	//Sprite Animation
	float mCurrTimeF = 0.0f;
	UINT mCurrTime = 0;
	UINT timeInterval = 2;
	bool timeSwap = false;

	//SkyBox Lerp
	float mTimeRatio = 0.0f;
	bool timeIncreasing = true;
	float mAdjustedTime = 0.0f;
	float mSkyBoxSpeedScalar = 0.25f;
};