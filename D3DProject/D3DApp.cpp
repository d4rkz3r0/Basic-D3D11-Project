#include "D3DApp.h"

#if 0
//std::wostringstream OSS;
//OSS << mMainWindowTitleBarText << L"      " << L"TIME: " << mAdjustedTime;
//SetWindowText(mMainWnd, OSS.str().c_str());
#endif

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
mSimpleVS(NULL),
mSimplePS(NULL),
mPosInputLayout(NULL),
mVertexInputLayout(NULL)
{
	//Camera Init
	mCamera.SetPosition(0.0f, 6.0f, -30.0f);
	mCamera.Pitch(XMConvertToRadians(12.5f));

	//Objects World Space Init
	XMMATRIX IdentityMX = XMMatrixIdentity();
	mWorld = XMMatrixIdentity();
	XMStoreFloat4x4(&mCube, XMMatrixTranslationFromVector(XMVectorSet(0.0f, 1.05f, 0.0f, 1.0f)));
	XMStoreFloat4x4(&mSkyBox, IdentityMX);
	XMStoreFloat4x4(&mQuad, IdentityMX);
	XMStoreFloat4x4(&mStalker, IdentityMX);
	XMStoreFloat4x4(&mCyberDemons, IdentityMX);
	XMStoreFloat4x4(&mTree, IdentityMX);
	XMStoreFloat4x4(&mMiniMap, IdentityMX);

	//Skybox
	mSkyBoxScalingMX = XMMatrixScaling(666.0f, 666.0f, 666.0f);
	//Quad
	mQuadScalingMX = XMMatrixScaling(6.0f, 6.0f, 6.0f);
	XMStoreFloat4x4(&mQuad, mQuadScalingMX);

	//Stalker
	mStalkerScalingMX = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	mStalkerRotationMX = XMMatrixRotationY(XMConvertToRadians(180.0f));
	mStalkerTranslationMX = XMMatrixTranslation(-3.0f, 0.0f, 0.0f);
	mStalkerTransformationMX = XMMatrixMultiply(mStalkerScalingMX, XMMatrixMultiply(mStalkerRotationMX, mStalkerTranslationMX));
	XMStoreFloat4x4(&mStalker, mStalkerTransformationMX);



	//Shader Info CB
	XMStoreFloat4(&shaderInfoDataBlock1, XMVectorZero());
	XMStoreFloat4(&shaderInfoDataBlock2, XMVectorZero());
	XMStoreFloat4(&shaderInfoDataBlock3, XMVectorZero());
	XMStoreFloat4(&shaderInfoDataBlock4, XMVectorZero());
}

D3DApp::~D3DApp()
{
	SAFE_RELEASE(mPosInputLayout);
	SAFE_RELEASE(mPosColInputLayout);
	SAFE_RELEASE(mVertexInputLayout);
	SAFE_RELEASE(mFullVertexInputLayout);
	SAFE_RELEASE(mSimpleVS);
	SAFE_RELEASE(mSimplePS);
	SAFE_RELEASE(mWireFrameRasterState);
	SAFE_RELEASE(mNoCullRasterState);
	SAFE_RELEASE(mDefaultRasterState);
	SAFE_RELEASE(mAlphaToCoverageBlendState);
	SAFE_RELEASE(mTransparentBlendState);
	SAFE_RELEASE(mOpaqueBlendState);
	SAFE_RELEASE(mAnisoSamplerState);
	SAFE_RELEASE(mLessEqualDSS);
	SAFE_RELEASE(mCubeVB);
	SAFE_RELEASE(mCubeIB);
	SAFE_RELEASE(mSkyBoxVB);
	SAFE_RELEASE(mSkyBoxIB);
	SAFE_RELEASE(mSkyBoxVS);
	SAFE_RELEASE(mSkyBoxPS);
	SAFE_RELEASE(mSkyBoxSRV);
	SAFE_RELEASE(mQuadVB);
	SAFE_RELEASE(mQuadIB);
	SAFE_RELEASE(mQuadSRV);
	SAFE_RELEASE(mTextureLightVS);
	SAFE_RELEASE(mTextureLightPS);
	SAFE_RELEASE(mStalkerVB);
	SAFE_RELEASE(mStalkerIB);
	SAFE_RELEASE(mStalkerSRV);
	SAFE_RELEASE(mCyberDemonVB);
	SAFE_RELEASE(mCyberDemonSRV);
	SAFE_RELEASE(mBillBoardVS);
	SAFE_RELEASE(mBillBoardGS);
	SAFE_RELEASE(mBillBoardPS);
	SAFE_RELEASE(mBillBoardInputLayout);
	SAFE_RELEASE(mCWCullRasterState);
	SAFE_RELEASE(mCCWCullRasterState);
	SAFE_RELEASE(mInstancedInputLayout);
	SAFE_RELEASE(mTreeVB);
	SAFE_RELEASE(mTreeIB);
	SAFE_RELEASE(mTreeInstBuffer);
	SAFE_RELEASE(mTreeSRV);
	SAFE_RELEASE(mTreeInstanceVS);
	SAFE_RELEASE(mTreeInstancePS);
	SAFE_RELEASE(mSkyBox2SRV);
	SAFE_RELEASE(mMiniMapVB);
	SAFE_RELEASE(mMiniMapIB);
	SAFE_RELEASE(mDepthStencilMiniMapBuffer);
	SAFE_RELEASE(mDepthStencilMiniMapView);
	SAFE_RELEASE(mRenderTargetTexture);
	SAFE_RELEASE(mRenderTargetRTV);
	SAFE_RELEASE(mRenderTargetSRV);
	SAFE_RELEASE(mMiniMapVS);
	SAFE_RELEASE(mMiniMapPS);
	SAFE_RELEASE(mAltQuadSRV);

}

bool D3DApp::Init()
{
	if (!D3DBase::Init())
	{
		return false;
	}

	BuildStateConfigurations();
	BuildGeometryAndBuffers();
	BuildOtherRenderTargets();
	CompileShaders();
	DefineInputLayouts();

	return true;
}

void D3DApp::ResizeWindow()
{
	D3DBase::ResizeWindow();
	mCamera.SetFrustum(XM_PIDIV4, AspectRatio(), 0.5f, 1000.f);
}

void D3DApp::Update(float deltaTime)
{

	GetUserInput(deltaTime);
	UpdateCamera(deltaTime);
	UpdateSkybox(deltaTime);
	UpdateLights(deltaTime);
}

void D3DApp::Draw()
{	
	float mClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mMiniMapClearColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float mBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mD3DDeviceContext->ClearRenderTargetView(mRenderTargetView, mClearColor);
	mD3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Shader Info Buffer
	cbMiscInfo ShaderMiscCB;
	ShaderMiscCB.dataBlock1 = shaderInfoDataBlock1;
	ShaderMiscCB.dataBlock2 = shaderInfoDataBlock2;
	ShaderMiscCB.dataBlock3 = shaderInfoDataBlock3;
	ShaderMiscCB.dataBlock4 = shaderInfoDataBlock4;

	//Global Material Buffer
	cbPerObjectMaterial mPerObjectMaterialCBQD;
	mPerObjectMaterialCBQD.Ambient = mObjectMaterialConstBufferFactors.Data.Ambient;
	mPerObjectMaterialCBQD.Diffuse = mObjectMaterialConstBufferFactors.Data.Diffuse;
	mPerObjectMaterialCBQD.Reflect = mObjectMaterialConstBufferFactors.Data.Reflect;
	mPerObjectMaterialCBQD.Specular = mObjectMaterialConstBufferFactors.Data.Specular;
	mObjectMaterialConstBufferFactors.Data = mPerObjectMaterialCBQD;
	mObjectMaterialConstBufferFactors.ApplyChanges(mD3DDeviceContext);
	auto cBufferQDMat = mObjectMaterialConstBufferFactors.Buffer();

	//Cube
	UINT stride = sizeof(PosColVertex);
	UINT offset = 0;
	mD3DDeviceContext->IASetInputLayout(mPosColInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(0, 1, &mCubeVB, &stride, &offset);
	mD3DDeviceContext->IASetIndexBuffer(mCubeIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mSimpleVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mSimplePS, NULL, 0);
	mWorld = XMLoadFloat4x4(&mCube);
	mWorldViewProj = XMMatrixMultiply(mWorld, mViewProj);
	cbPerObjectTransformation mPerObjectCB;
	XMStoreFloat4x4(&mPerObjectCB.mWorldViewProj, mWorldViewProj);
	mObjectConstBuffer.Data = mPerObjectCB;
	mObjectConstBuffer.ApplyChanges(mD3DDeviceContext);
	auto cBuffer = mObjectConstBuffer.Buffer();
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBuffer);
	mD3DDeviceContext->RSSetState(mDefaultRasterState);
	mD3DDeviceContext->DrawIndexed(cubeInfo.ibCount, 0, 0);

	//SkyBox
	UINT strideSB = sizeof(PosVertex);
	UINT offsetSB = 0;
	mD3DDeviceContext->IASetInputLayout(mPosInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(1, 1, &mSkyBoxVB, &strideSB, &offsetSB);
	mD3DDeviceContext->IASetIndexBuffer(mSkyBoxIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mSkyBoxVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mSkyBoxPS, NULL, 0);
	mWorld = XMLoadFloat4x4(&mSkyBox);
	mWorldViewProj = XMMatrixMultiply(mWorld, mViewProj);
	cbPerObjectTransformation mPerObjectCBSkyBox;
	XMStoreFloat4x4(&mPerObjectCBSkyBox.mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&mPerObjectCBSkyBox.mWorldMatrix, mWorld);
	mObjectConstBufferSB.Data = mPerObjectCBSkyBox;
	mObjectConstBufferSB.ApplyChanges(mD3DDeviceContext);
	auto cBufferSB = mObjectConstBufferSB.Buffer();
	ShaderMiscCB.dataBlock1.w = mTimeRatio;
	mShaderConstantBufferInfo.Data = ShaderMiscCB;
	mShaderConstantBufferInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferShaderMiscInfo = mShaderConstantBufferInfo.Buffer();
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferSB);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mSkyBoxSRV);
	mD3DDeviceContext->PSSetShaderResources(1, 1, &mSkyBox2SRV);
	mD3DDeviceContext->RSSetState(mNoCullRasterState);
	mD3DDeviceContext->OMSetDepthStencilState(mLessEqualDSS, 0);
	mD3DDeviceContext->DrawIndexed(skyBoxInfo.ibCount, 0, 0);
	mD3DDeviceContext->VSSetShader(mSimpleVS, NULL, NULL);
	mD3DDeviceContext->RSSetState(mDefaultRasterState);
	mD3DDeviceContext->OMSetDepthStencilState(NULL, 0);

	//GroundQuad
	UINT strideQuad = sizeof(FullVertex);
	UINT offsetQuad = 0;
	mD3DDeviceContext->IASetInputLayout(mFullVertexInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(6, 1, &mQuadVB, &strideQuad, &offsetQuad);
	mD3DDeviceContext->IASetIndexBuffer(mQuadIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mTextureLightVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mTextureLightPS, NULL, 0);
	mWorld = XMLoadFloat4x4(&mQuad);
	mWorldViewProj = XMMatrixMultiply(mWorld, mViewProj);
	cbPerObjectTransformation mPerObjectCBQD;
	XMStoreFloat4x4(&mPerObjectCBQD.mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&mPerObjectCBQD.mWorldMatrix, mWorld);
	mObjectConstBufferQuad.Data = mPerObjectCBQD;
	mObjectConstBufferQuad.ApplyChanges(mD3DDeviceContext);
	auto cBufferQD = mObjectConstBufferQuad.Buffer();
	ShaderMiscCB.dataBlock1.x = mUVTiledGroundScalar;
	mShaderConstantBufferInfo.Data = ShaderMiscCB;
	mShaderConstantBufferInfo.ApplyChanges(mD3DDeviceContext);
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferQD);
	mD3DDeviceContext->PSSetConstantBuffers(3, 1, &cBufferQDMat);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mQuadSRV);
	mD3DDeviceContext->RSSetState(mNoCullRasterState);
	mD3DDeviceContext->DrawIndexed(mQuadMesh.mIndices.size(), 0, 0);

	//Light Info Buffers
	mDirectionalLightInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferDirLight1 = mDirectionalLightInfo.Buffer();
	mD3DDeviceContext->PSSetConstantBuffers(0, 1, &cBufferDirLight1);
	mPointLightInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferPointLight1 = mPointLightInfo.Buffer();
	mD3DDeviceContext->PSSetConstantBuffers(1, 1, &cBufferPointLight1);
	mSpotLightInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferSpotLight1 = mSpotLightInfo.Buffer();
	mD3DDeviceContext->PSSetConstantBuffers(2, 1, &cBufferSpotLight1);

	//Stalker
	UINT strideStalker = sizeof(FullVertex);
	UINT offsetStalker = 0;
	mD3DDeviceContext->IASetInputLayout(mFullVertexInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(6, 1, &mStalkerVB, &strideStalker, &offsetStalker);
	mD3DDeviceContext->IASetIndexBuffer(mStalkerIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mTextureLightVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mTextureLightPS, NULL, 0);
	mWorld = XMLoadFloat4x4(&mStalker);
	mWorldViewProj = XMMatrixMultiply(mWorld, mViewProj);
	cbPerObjectTransformation mPerObjectCBStalker;
	XMStoreFloat4x4(&mPerObjectCBStalker.mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&mPerObjectCBStalker.mWorldMatrix, mWorld);
	mObjectConstBufferStalker.Data = mPerObjectCBStalker;
	mObjectConstBufferStalker.ApplyChanges(mD3DDeviceContext);
	auto cBufferStalker = mObjectConstBufferStalker.Buffer();
	ShaderMiscCB.dataBlock1.x = mUVDefaultScalar;
	mShaderConstantBufferInfo.Data = ShaderMiscCB;
	mShaderConstantBufferInfo.ApplyChanges(mD3DDeviceContext);
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferStalker);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mStalkerSRV);
	mD3DDeviceContext->RSSetState(mDefaultRasterState);
	mD3DDeviceContext->DrawIndexed(mStalkerMesh.mIndices.size(), 0, 0);

	//CyberDemons
	UINT strideBillBoard = sizeof(PointSprite);
	UINT offsetBillBoard = 0;
	mD3DDeviceContext->IASetInputLayout(mBillBoardInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	mD3DDeviceContext->IASetVertexBuffers(2, 1, &mCyberDemonVB, &strideBillBoard, &offsetBillBoard);
	mD3DDeviceContext->VSSetShader(mBillBoardVS, NULL, 0);
	mD3DDeviceContext->GSSetShader(mBillBoardGS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mBillBoardPS, NULL, 0);
	mWorld = XMLoadFloat4x4(&mCyberDemons);
	mWorldViewProj = XMMatrixMultiply(mWorld, mViewProj);
	mWorldViewProj = mViewProj;
	cbPerObjectTransformation mPerPointSpriteCB;
	XMStoreFloat4x4(&mPerPointSpriteCB.mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&mPerPointSpriteCB.mWorldMatrix, XMMatrixIdentity());
	mObjectConstantBufferShaderInfo.Data = mPerPointSpriteCB;
	mObjectConstantBufferShaderInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferPointSprite = mObjectConstantBufferShaderInfo.Buffer();
	AnimateBillBoards(mUVCyberDemonBillBoardUVx, mUVCyberDemonBillBoardUVy, 0);
	ShaderMiscCB.dataBlock1.x = mUVCyberDemonBillBoardUVx;
	ShaderMiscCB.dataBlock1.y = mUVCyberDemonBillBoardUVy;
	mShaderConstantBufferInfo.Data = ShaderMiscCB;
	mShaderConstantBufferInfo.ApplyChanges(mD3DDeviceContext);
	mD3DDeviceContext->GSSetConstantBuffers(0, 1, &cBufferPointSprite);
	mD3DDeviceContext->GSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mCyberDemonSRV);
	mD3DDeviceContext->RSSetState(mNoCullRasterState);
	mD3DDeviceContext->OMSetBlendState(mTransparentBlendState, mBlendFactor, 0xFFFFFFFF);
	mD3DDeviceContext->Draw(mNumOfCyberDemons, 0);
	mD3DDeviceContext->GSSetShader(NULL, NULL, NULL);

	//Mario Trees
	UINT stridesInstance[2] = { sizeof(Vertex), sizeof(InstanceData) };
	UINT offsetsInstance[2] = { 0, 0 };
	ID3D11Buffer* VBs[2] = { mTreeVB, mTreeInstBuffer };
	mD3DDeviceContext->IASetInputLayout(mInstancedInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(7, 2, VBs, stridesInstance, offsetsInstance);
	mD3DDeviceContext->IASetIndexBuffer(mTreeIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mTreeInstanceVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mTreeInstancePS, NULL, 0);
	mWorldViewProj = mViewProj;
	cbPerObjectTransformation mPerInstanceCB; //Only contains VP MX
	XMStoreFloat4x4(&mPerInstanceCB.mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&mPerInstanceCB.mWorldMatrix, XMMatrixIdentity());
	mInstanceConstantBufferShaderInfo.Data = mPerInstanceCB;
	mInstanceConstantBufferShaderInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferInstance = mInstanceConstantBufferShaderInfo.Buffer();
	ShaderMiscCB.dataBlock1.x = mUVDefaultScalar;
	ShaderMiscCB.dataBlock1.y = mUVDefaultScalar;
	mShaderConstantBufferInfo.Data = ShaderMiscCB;
	mShaderConstantBufferInfo.ApplyChanges(mD3DDeviceContext);
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferInstance);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mTreeSRV);
	mD3DDeviceContext->RSSetState(mNoCullRasterState);
	mD3DDeviceContext->DrawIndexedInstanced(mTreeMesh.Indices.size(), mNumInstances, 0, 0, 0);

	//Switch To Mini-Map Render Target
	mD3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetRTV, NULL);
	mD3DDeviceContext->ClearRenderTargetView(mRenderTargetRTV, mMiniMapClearColor);
	mD3DDeviceContext->ClearDepthStencilView(mDepthStencilMiniMapView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Start Alternate Draw
	//SkyBox
	mD3DDeviceContext->IASetInputLayout(mPosInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(1, 1, &mSkyBoxVB, &strideSB, &offsetSB);
	mD3DDeviceContext->IASetIndexBuffer(mSkyBoxIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mSkyBoxVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mSkyBoxPS, NULL, 0);
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferSB);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mSkyBoxSRV);
	mD3DDeviceContext->PSSetShaderResources(1, 1, &mSkyBox2SRV);
	mD3DDeviceContext->RSSetState(mNoCullRasterState);
	mD3DDeviceContext->OMSetDepthStencilState(mLessEqualDSS, 0);
	mD3DDeviceContext->DrawIndexed(skyBoxInfo.ibCount, 0, 0);
	mD3DDeviceContext->VSSetShader(mSimpleVS, NULL, NULL);
	mD3DDeviceContext->RSSetState(mDefaultRasterState);
	mD3DDeviceContext->OMSetDepthStencilState(NULL, 0);
	//GroundQuad
	mD3DDeviceContext->IASetInputLayout(mFullVertexInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(6, 1, &mQuadVB, &strideQuad, &offsetQuad);
	mD3DDeviceContext->IASetIndexBuffer(mQuadIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mTextureLightVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mTextureLightPS, NULL, 0);
	ShaderMiscCB.dataBlock1.x = mUVTiledGroundScalar;
	mShaderConstantBufferInfo.Data = ShaderMiscCB;
	mShaderConstantBufferInfo.ApplyChanges(mD3DDeviceContext);
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferQD);
	mD3DDeviceContext->PSSetConstantBuffers(3, 1, &cBufferQDMat);
	mD3DDeviceContext->PSSetConstantBuffers(10, 1, &cBufferShaderMiscInfo);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mAltQuadSRV);
	mD3DDeviceContext->RSSetState(mNoCullRasterState);
	mD3DDeviceContext->DrawIndexed(mQuadMesh.mIndices.size(), 0, 0);
	//End Alternate Draw
	mD3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	//Mini-Map
	UINT strideMiniMap = sizeof(Vertex);
	UINT offsetMiniMap = 0;
	mD3DDeviceContext->IASetInputLayout(mVertexInputLayout);
	mD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mD3DDeviceContext->IASetVertexBuffers(3, 1, &mMiniMapVB, &strideMiniMap, &offsetMiniMap);
	mD3DDeviceContext->IASetIndexBuffer(mMiniMapIB, DXGI_FORMAT_R32_UINT, 0);
	mD3DDeviceContext->VSSetShader(mMiniMapVS, NULL, 0);
	mD3DDeviceContext->PSSetShader(mMiniMapPS, NULL, 0);
	cbPerObjectTransformation mMiniMapCB;
	XMStoreFloat4x4(&mMiniMapCB.mWorldViewProj, mMiniMapViewProj);
	XMStoreFloat4x4(&mMiniMapCB.mWorldMatrix, XMMatrixIdentity());
	mMiniMapConstantBufferQuadInfo.Data = mMiniMapCB;
	mMiniMapConstantBufferQuadInfo.ApplyChanges(mD3DDeviceContext);
	auto cBufferMiniMapQuad = mMiniMapConstantBufferQuadInfo.Buffer();
	mD3DDeviceContext->VSSetConstantBuffers(0, 1, &cBufferMiniMapQuad);
	mD3DDeviceContext->PSSetSamplers(0, 1, &mAnisoSamplerState);
	mD3DDeviceContext->PSSetShaderResources(0, 1, &mRenderTargetSRV);
	mD3DDeviceContext->DrawIndexed(mMiniMapQuad.Indices.size(), 0, 0);




	//Hard Reset
	mD3DDeviceContext->RSSetState(mDefaultRasterState);
	//Present
	mSwapChain->Present(1, 0);
}


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
	mObjectConstBuffer.Initialize(mD3DDevice);

	//SkyBox
	BuildSkyBox(mSkyBoxFileName);

	//Quad
	GeoFactory.GenerateModel(mQuadMesh, mQuadFileName);
	CreateDDSTextureFromFile(mD3DDevice, mQuadTextureFileName, NULL, &mQuadSRV);
	CreateDDSTextureFromFile(mD3DDevice, mAltQuadTextureFileName, NULL, &mAltQuadSRV);
	GeoFactory.GenerateVertexAndIndexBuffers(mD3DDevice, mQuadMesh, &mQuadVB, &mQuadIB);
	mObjectConstBufferQuad.Initialize(mD3DDevice);

	//Object Materials
	mObjectMaterialConstBufferFactors.Initialize(mD3DDevice);

	//Lights
	mDirectionalLightInfo.Initialize(mD3DDevice);
	mPointLightInfo.Initialize(mD3DDevice);
	mSpotLightInfo.Initialize(mD3DDevice);

	//Stalker
	GeoFactory.GenerateModel(mStalkerMesh, mStalkerFileName, true, false, true, false, false);
	CreateDDSTextureFromFile(mD3DDevice, mStalkerTextureFileName, NULL, &mStalkerSRV);
	GeoFactory.GenerateVertexAndIndexBuffers(mD3DDevice, mStalkerMesh, &mStalkerVB, &mStalkerIB);
	mObjectConstBufferStalker.Initialize(mD3DDevice);

	//CyberWarrior BillBoards
	GeoFactory.GenerateBillBoards(mD3DDevice, &mCyberDemonVB, mNumOfCyberDemons, 6.0f, 50.0f, 3.0f, 50.0f);
	CreateDDSTextureFromFile(mD3DDevice, mCyberDemonTextureFileName, NULL, &mCyberDemonSRV);
	mObjectConstantBufferShaderInfo.Initialize(mD3DDevice);

	//Mario Trees
	GeoFactory.GenerateStaticQuad(mD3DDevice, mTreeMesh);
	GeoFactory.GenerateVertexAndIndexBuffersNon(mD3DDevice, mTreeMesh, &mTreeVB, &mTreeIB);
	CreateDDSTextureFromFile(mD3DDevice, mTreeTextureFileName, NULL, &mTreeSRV);
	GeoFactory.GenerateInstanceBuffer(mD3DDevice, &mTreeInstBuffer, mInstancedTreeVec, mNumInstances, 50.0f, 4.0f, 50.0f);
	mInstanceConstantBufferShaderInfo.Initialize(mD3DDevice);

	//Mini Map 
	GeoFactory.GenerateStaticQuad(mD3DDevice, mMiniMapQuad);
	GeoFactory.GenerateVertexAndIndexBuffersNon(mD3DDevice, mMiniMapQuad, &mMiniMapVB, &mMiniMapIB);
	mMiniMapConstantBufferQuadInfo.Initialize(mD3DDevice);

	//Shader Misc
	mShaderConstantBufferInfo.Initialize(mD3DDevice);
}

void D3DApp::BuildOtherRenderTargets()
{
	//RTT Textures and Resource Views
	D3D11_TEXTURE2D_DESC renderTargetTextureDesc;
	ZeroMemory(&renderTargetTextureDesc, sizeof(renderTargetTextureDesc));
	renderTargetTextureDesc.Width = mWindowWidth;
	renderTargetTextureDesc.Height = mWindowHeight / 2;
	renderTargetTextureDesc.MipLevels = 1;
	renderTargetTextureDesc.ArraySize = 1;
	renderTargetTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetTextureDesc.SampleDesc.Count = 1;
	renderTargetTextureDesc.SampleDesc.Quality = 0;
	renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTargetTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTargetTextureDesc.CPUAccessFlags = 0;
	renderTargetTextureDesc.MiscFlags = 0;
	mD3DDevice->CreateTexture2D(&renderTargetTextureDesc, NULL, &mRenderTargetTexture);


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = renderTargetTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = NULL;
	mD3DDevice->CreateRenderTargetView(mRenderTargetTexture, NULL, &mRenderTargetRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = renderTargetTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	mD3DDevice->CreateShaderResourceView(mRenderTargetTexture, NULL, &mRenderTargetSRV);

	D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
	ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
	depthStencilTextureDesc.Width = mWindowWidth;
	depthStencilTextureDesc.Height = mWindowHeight / 2;
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.ArraySize = 1;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTextureDesc.SampleDesc.Count = 1;
	depthStencilTextureDesc.SampleDesc.Quality = 0;
	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.CPUAccessFlags = 0;
	depthStencilTextureDesc.MiscFlags = 0;
	mD3DDevice->CreateTexture2D(&depthStencilTextureDesc, NULL, &mDepthStencilMiniMapBuffer);
	mD3DDevice->CreateDepthStencilView(mDepthStencilMiniMapBuffer, 0, &mDepthStencilMiniMapView);

	//RTT View Matrix Setup
	XMVECTOR mapCamPosition = mCamera.GetPositionXM();
	XMVECTOR mapCamTarget = XMVectorSet(XMVectorGetX(mCamera.GetLookXM()),
		XMVectorGetY(mCamera.GetLookXM()), XMVectorGetZ(mCamera.GetLookXM()),
		XMVectorGetW(mCamera.GetLookXM()));
//	XMVECTOR mapCamTarget = XMVectorNegate(mCamera.GetPositionXM());

	XMVECTOR mapCamUp = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	mRenderTargetViewMX = XMMatrixLookAtLH(mapCamPosition, mapCamTarget, mapCamUp);
//	mMiniMapViewProj = mRenderTargetViewMX;

	//RTT VP MXs
	mMiniMapScalingMX = XMMatrixScaling(1.0f, 0.5f, 0.0f);
	mMiniMapTranslationMX = XMMatrixTranslation(0.0f, -0.5f, 0.0f);
	mMiniMapTransformationMX = XMMatrixMultiply(mMiniMapScalingMX, mMiniMapTranslationMX);
	mMiniMapViewProj = mMiniMapTransformationMX; //* mRenderTargetViewMX;
	//mMiniMapViewProj = XMMatrixMultiply(mMiniMapTransformationMX, mRenderTargetViewMX);

}

void D3DApp::CompileShaders()
{
	mD3DDevice->CreateVertexShader(BasicVertexShader, sizeof(BasicVertexShader), NULL, &mSimpleVS);
	mD3DDevice->CreatePixelShader(BasicPixelShader, sizeof(BasicPixelShader), NULL, &mSimplePS);
	mD3DDevice->CreateVertexShader(SkyBoxVertexShader, sizeof(SkyBoxVertexShader), NULL, &mSkyBoxVS);
	mD3DDevice->CreatePixelShader(SkyBoxPixelShader, sizeof(SkyBoxPixelShader), NULL, &mSkyBoxPS);
	mD3DDevice->CreateVertexShader(TextureLightingVS, sizeof(TextureLightingVS), NULL, &mTextureLightVS);
	mD3DDevice->CreatePixelShader(TextureLightingPS, sizeof(TextureLightingPS), NULL, &mTextureLightPS);
	mD3DDevice->CreateVertexShader(BillBoardVS, sizeof(BillBoardVS), NULL, &mBillBoardVS);
	mD3DDevice->CreateGeometryShader(BillBoardGS, sizeof(BillBoardGS), NULL, &mBillBoardGS);
	mD3DDevice->CreatePixelShader(BillBoardPS, sizeof(BillBoardPS), NULL, &mBillBoardPS);
	mD3DDevice->CreateVertexShader(InstanceVS, sizeof(InstanceVS), NULL, &mTreeInstanceVS);
	mD3DDevice->CreatePixelShader(InstancePS, sizeof(InstancePS), NULL, &mTreeInstancePS);
	mD3DDevice->CreateVertexShader(RenderTextureVS, sizeof(RenderTextureVS), NULL, &mMiniMapVS);
	mD3DDevice->CreatePixelShader(RenderTexturePS, sizeof(RenderTexturePS), NULL, &mMiniMapPS);
}

void D3DApp::DefineInputLayouts()
{
	D3D11_INPUT_ELEMENT_DESC posColVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(posColVertexDesc, ARRAYSIZE(posColVertexDesc), BasicVertexShader, sizeof(BasicVertexShader), &mPosColInputLayout);


	D3D11_INPUT_ELEMENT_DESC posVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(posVertexDesc, ARRAYSIZE(posVertexDesc), SkyBoxVertexShader, sizeof(SkyBoxVertexShader), &mPosInputLayout);
	
	D3D11_INPUT_ELEMENT_DESC billboardSpriteDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(billboardSpriteDesc, ARRAYSIZE(billboardSpriteDesc), BillBoardVS, sizeof(BillBoardVS), &mBillBoardInputLayout);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), RenderTextureVS, sizeof(RenderTextureVS), &mVertexInputLayout);

	D3D11_INPUT_ELEMENT_DESC instanceBufferDesc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 7, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 7, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 7, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IWORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 8, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "IWORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 8, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "IWORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 8, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "IWORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 8, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "ICOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 8, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	mD3DDevice->CreateInputLayout(instanceBufferDesc, ARRAYSIZE(instanceBufferDesc), InstanceVS, sizeof(InstanceVS), &mInstancedInputLayout);

	D3D11_INPUT_ELEMENT_DESC fullVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 6, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 6, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 6, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    6, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mD3DDevice->CreateInputLayout(fullVertexDesc, ARRAYSIZE(fullVertexDesc), TextureLightingVS, sizeof(TextureLightingVS), &mFullVertexInputLayout);

	

	

}

void D3DApp::BuildStateConfigurations()
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	mD3DDevice->CreateSamplerState(&sd, &mAnisoSamplerState);

	D3D11_RASTERIZER_DESC rs;
	ZeroMemory(&rs, sizeof(rs));
	rs.FillMode = D3D11_FILL_SOLID;
	rs.CullMode = D3D11_CULL_BACK;
	rs.MultisampleEnable = true;
	rs.AntialiasedLineEnable = rs.DepthClipEnable = true;
	mDefaultRasterState = NULL;
	mD3DDevice->CreateRasterizerState(&rs, &mDefaultRasterState);
		
	D3D11_RASTERIZER_DESC rs1;
	ZeroMemory(&rs1, sizeof(rs1));
	rs1.FillMode = D3D11_FILL_SOLID;
	rs1.CullMode = D3D11_CULL_NONE;
	rs1.MultisampleEnable = true;
	mNoCullRasterState = NULL;
	mD3DDevice->CreateRasterizerState(&rs1, &mNoCullRasterState);

	D3D11_RASTERIZER_DESC rs2;
	ZeroMemory(&rs2, sizeof(rs2));
	rs2.FillMode = D3D11_FILL_SOLID;
	rs2.CullMode = D3D11_CULL_BACK;
	rs2.FrontCounterClockwise = false;
	rs2.MultisampleEnable = true;
	mCWCullRasterState = NULL;
	mD3DDevice->CreateRasterizerState(&rs2, &mCWCullRasterState);

	D3D11_RASTERIZER_DESC rs3;
	ZeroMemory(&rs3, sizeof(rs3));
	rs3.FillMode = D3D11_FILL_SOLID;
	rs3.CullMode = D3D11_CULL_BACK;
	rs3.FrontCounterClockwise = true;
	rs3.MultisampleEnable = true;
	mCCWCullRasterState = NULL;
	mD3DDevice->CreateRasterizerState(&rs3, &mCCWCullRasterState);

	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	mLessEqualDSS = NULL;
	mD3DDevice->CreateDepthStencilState(&dsd, &mLessEqualDSS);

	D3D11_RASTERIZER_DESC wfd;
	ZeroMemory(&wfd, sizeof(wfd));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_BACK;
	wfd.FrontCounterClockwise = false;
	wfd.DepthClipEnable = true;
	wfd.MultisampleEnable = true;
	mD3DDevice->CreateRasterizerState(&wfd, &mWireFrameRasterState);

	D3D11_BLEND_DESC aTcd;
	ZeroMemory(&aTcd, sizeof(aTcd));
	aTcd.AlphaToCoverageEnable = true;
	aTcd.IndependentBlendEnable = false;
	aTcd.RenderTarget[0].BlendEnable = false;
	aTcd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	mD3DDevice->CreateBlendState(&aTcd, &mAlphaToCoverageBlendState);

	D3D11_BLEND_DESC opbd;
	ZeroMemory(&opbd, sizeof(opbd));
	mD3DDevice->CreateBlendState(&opbd, &mOpaqueBlendState);

	D3D11_BLEND_DESC tspd;
	tspd.AlphaToCoverageEnable = false;
	tspd.IndependentBlendEnable = false;
	tspd.RenderTarget[0].BlendEnable = true;
	tspd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tspd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tspd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tspd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tspd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tspd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tspd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	mD3DDevice->CreateBlendState(&tspd, &mTransparentBlendState);
}

void D3DApp::OnMouseDown(WPARAM theButton, int x, int y)
{
	mPrevMousePos.x = x;
	mPrevMousePos.y = y;
	
	SetCapture(mMainWnd);
}

void D3DApp::OnMouseUp(WPARAM theButton, int x, int y)
{
	ReleaseCapture();
}

void D3DApp::OnMouseMove(WPARAM theButton, int x, int y)
{
	if (theButton & MK_LBUTTON)
	{
		float currYRotAngle = XMConvertToRadians(-0.25f * static_cast<float>(x - mPrevMousePos.x));
		float currXRotAngle = XMConvertToRadians(-0.25f * static_cast<float>(y - mPrevMousePos.y));
		mCamera.Pitch(-currXRotAngle);
		mCamera.RotateY(-currYRotAngle);
	}
	mPrevMousePos.x = x;
	mPrevMousePos.y = y;
}

float D3DApp::GetAngle(float deltaTime)
{
	deltaTime = deltaTime * speedScalar;
	angle += deltaTime;
	return angle;
}

void D3DApp::GetUserInput(float deltaTime)
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		mCamera.Step(mCamScalar * deltaTime);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		mCamera.Step(-mCamScalar  * deltaTime);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		mCamera.Strafe(-mCamScalar * deltaTime);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		mCamera.Strafe(mCamScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		mCamera.VFly(mCamScalar *deltaTime);
	}
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		mCamera.VFly(-mCamScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		MsgProc(mMainWnd, WM_DESTROY, NULL, NULL);
	}
	if (GetAsyncKeyState(VK_NUMPAD8) && 0x8000)
	{
		mCurrPointLightPosZ += (mPointLightScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_NUMPAD5) && 0x8000)
	{
		mCurrPointLightPosZ += (-mPointLightScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_NUMPAD4) && 0x8000)
	{
		mCurrPointLightPosX += (-mPointLightScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_NUMPAD6) && 0x8000)
	{
		mCurrPointLightPosX += (mPointLightScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_NUMPAD7) && 0x8000)
	{
		mCurrPointLightPosY += (-mPointLightScalar * deltaTime);
	}
	if (GetAsyncKeyState(VK_NUMPAD9) && 0x8000)
	{
		mCurrPointLightPosY += (mPointLightScalar * deltaTime);
	}
	
}

void D3DApp::AnimateBillBoards(float& UVx, float& UVy, UINT flag)
{
	mCurrTimeF = static_cast<float>(mTimer.TotalTime());
	mCurrTime = static_cast<UINT>(roundf(mCurrTimeF));
	
	switch (flag)
	{
		case 0: //CyberDemon
		{
			if (mCurrTime % timeInterval == 0 && !timeSwap)
			{
				if (UVx == 1.0f)
				{
					UVx = -1.0f;
					timeSwap = true;
				}
				else if (UVx == -1.0f)
				{
					UVx = -1.5f;
					timeSwap = true;
				}
				else if (UVx == -1.5f)
				{
					UVx = -1.0f;
					timeSwap = true;
				}
			}
			else if (mCurrTime % timeInterval == 1 && timeSwap)
			{
				timeSwap = false;
			}

			UVy = mUVCyberDemonBillBoardUVy;
			break;
		}
		default:
		{
			break;
		}
	}
}

void D3DApp::UpdateCamera(float deltaTime)
{
	mCamera.UpdateViewMatrix();
	mViewProj = mCamera.GetViewProj();
	XMStoreFloat4(&shaderInfoDataBlock2, mCamera.GetPositionXM());
	XMStoreFloat4(&shaderInfoDataBlock3, mCamera.GetUpXM());
}

void D3DApp::UpdateSkybox(float deltaTime)
{
	if (timeIncreasing)
	{
		if (mAdjustedTime >= 0.99f)
		{
			mAdjustedTime = 1.0f;
			timeIncreasing = false;
		}
		else
		{
			mAdjustedTime = fmod(static_cast<float>(mTimer.TotalTime()) * mSkyBoxSpeedScalar, 1.00f);
		}
		
	}
	else
	{
		if (mAdjustedTime <= 0.01f)
		{
			mAdjustedTime = 0.0f;
			timeIncreasing = true;
		}
		else
		{
			mAdjustedTime = 1.00f - fmod(static_cast<float>(mTimer.TotalTime()) * mSkyBoxSpeedScalar, 1.00f);
		}
	}
	mTimeRatio = mAdjustedTime;

	mSkyBoxTranslationMX = XMMatrixTranslationFromVector(mCamera.GetPositionXM());
	mSkyBoxTransformationMX = XMMatrixMultiply(mSkyBoxScalingMX, mSkyBoxTranslationMX);
	XMStoreFloat4x4(&mSkyBox, mSkyBoxTransformationMX);
}

void D3DApp::UpdateLights(float deltaTime)
{
	//All Objects Material Info
	//Can be set for each object.
	mObjectMaterialConstBufferFactors.Data.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mObjectMaterialConstBufferFactors.Data.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mObjectMaterialConstBufferFactors.Data.Specular = XMFLOAT4(0.75f, 0.75f, 0.75f, 256.0f);
	mObjectMaterialConstBufferFactors.Data.Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	//Directional Light
	mDirectionalLightInfo.Data.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirectionalLightInfo.Data.Diffuse = ManualColors::White;
	mDirectionalLightInfo.Data.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	mDirectionalLightInfo.Data.Direction = XMFLOAT3(0.0f, -0.25, 0.0f);

	//Point Light - Num-pad Controlled
	mPointLightInfo.Data.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLightInfo.Data.Diffuse = ManualColors::Red;
	mPointLightInfo.Data.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 512.0f);
	mPointLightInfo.Data.Attenuation = XMFLOAT3(0.0f, 0.05f, 0.0f);
	mPointLightInfo.Data.Range = 200.0f;
    mPointLightInfo.Data.Position = XMFLOAT3(mCurrPointLightPosX, mCurrPointLightPosY, mCurrPointLightPosZ);

	//Spot Light - Follows Camera
	mSpotLightInfo.Data.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSpotLightInfo.Data.Diffuse = ManualColors::Yellow;
	mSpotLightInfo.Data.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 512.0f);
	mSpotLightInfo.Data.Attenuation = XMFLOAT3(0.0f, 0.03f, 0.0f);
	mSpotLightInfo.Data.Spot = 20.0f;
	mSpotLightInfo.Data.Range = 750.0f;
	XMStoreFloat3(&mSpotLightInfo.Data.Position, mCamera.GetPositionXM());
	XMStoreFloat3(&mSpotLightInfo.Data.Direction, mCamera.GetLookXM());
}

void D3DApp::BuildSkyBox(const wchar_t* fileName)
{
	CreateDDSTextureFromFile(mD3DDevice, fileName, NULL, &mSkyBoxSRV);
	CreateDDSTextureFromFile(mD3DDevice, mSkyBox2FileName, NULL, &mSkyBox2SRV);
	GeoFactory.GenerateSkyBox(skyBoxInfo);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = skyBoxInfo.vbSize;

	D3D11_SUBRESOURCE_DATA vbid;
	ZeroMemory(&vbid, sizeof(vbid));
	vbid.pSysMem = &skyBoxInfo.Vertices[0];
	mD3DDevice->CreateBuffer(&vbd, &vbid, &mSkyBoxVB);

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = skyBoxInfo.ibSize;

	D3D11_SUBRESOURCE_DATA ibid;
	ZeroMemory(&ibid, sizeof(ibid));
	ibid.pSysMem = &skyBoxInfo.Indices[0];
	mD3DDevice->CreateBuffer(&ibd, &ibid, &mSkyBoxIB);

	mObjectConstBufferSB.Initialize(mD3DDevice);
}