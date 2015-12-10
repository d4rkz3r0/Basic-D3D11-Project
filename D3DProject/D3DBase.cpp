#include "D3DBase.h"

namespace
{
	D3DBase* gD3DApp = 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Forward Messages properly, in the case of receiving messages before mMainWnd is initialized.
	return gD3DApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DBase::D3DBase(HINSTANCE hInstance) :
mAppInst(hInstance),
mMainWnd(mMainWnd),
mMainWindowTitleBarText(L"GX2 Project"),
mAppPaused(false),
mIsMinimized(false),
mIsMaximized(false),
mIsBeingResized(false),
mWindowWidth(1280),
mWindowHeight(720),
mD3DDriverType(D3D_DRIVER_TYPE_HARDWARE),
mD3DDevice(NULL),
mD3DDeviceContext(NULL),
mSwapChain(NULL),
mDepthStencilBuffer(NULL),
mRenderTargetView(NULL),
mDepthStencilView(NULL),
m4xMSAAQualityLevel(0),
mEnable4xMSAA(true)
{
	ZeroMemory(&mViewPort, sizeof(mViewPort));
	gD3DApp = this;
}

D3DBase::~D3DBase()
{
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mSwapChain);
	mD3DDeviceContext->ClearState(); //Unbind Resources and State Objects
	SAFE_RELEASE(mD3DDeviceContext);

#if 0
	ID3D11Debug* debugDev;
	mD3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));
	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SAFE_RELEASE(debugDev);
#endif
	SAFE_RELEASE(mD3DDevice);
}

HINSTANCE D3DBase::AppInst() const { return mAppInst; }
HWND D3DBase::MainWnd() const { return mMainWnd; }
float D3DBase::AspectRatio() const { return static_cast<float>(mWindowWidth) / static_cast<float>(mWindowHeight); }

UINT D3DBase::Run()
{
	MSG msg = { 0 };

	srand(UINT(time(NULL)));
	mTimer.Restart();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mTimer.Signal();

			if (!mAppPaused)
			{
				CalcFPS();
				Update(static_cast<float>(mTimer.SmoothDelta()));
				Draw();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return static_cast<int>(msg.wParam);
}

bool D3DBase::Init()
{
	
	if (!Win32Init())
	{
		return false;
	}

	if (!Direct3DInit())
	{
		return false;
	}

	return true;
}

void D3DBase::ResizeWindow()
{
	assert(mD3DDevice);
	assert(mD3DDeviceContext);
	assert(mSwapChain);

	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mDepthStencilBuffer);

	//BackBuffer
	mSwapChain->ResizeBuffers(1, mWindowWidth, mWindowHeight, DXGI_FORMAT_UNKNOWN, NULL);
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	mD3DDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
	SAFE_RELEASE(backBuffer);

	//Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = mWindowWidth;
	depthStencilDesc.Height = mWindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //Interpreted as a float in shaders
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Re-enable MSAA
	if (mEnable4xMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMSAAQualityLevel - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	mD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	mD3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	
	//ViewPort
	mViewPort.Width = static_cast<float>(mWindowWidth);
	mViewPort.Height = static_cast<float>(mWindowHeight);
	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mD3DDeviceContext->RSSetViewports(1, &mViewPort);
}

LRESULT D3DBase::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		//Window Focus - Don't Really Need to Handle this
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mAppPaused = true;
				//mTimer.Stop();
			}
			else
			{
				mAppPaused = false;
				mTimer.Restart();
			}
			break;
		}

		//Window Resizing
		case WM_SIZE:
		{
			mWindowHeight = HIWORD(lParam);
			mWindowWidth = LOWORD(lParam);

			if (mD3DDevice)
			{
				if (wParam == SIZE_MINIMIZED)
				{
					mAppPaused = true;
					mIsMinimized = true;
					mIsMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					mAppPaused = false;
					mIsMinimized = false;
					mIsMaximized = true;
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (mIsMaximized)
					{
						mAppPaused = false;
						mIsMaximized = false;
						ResizeWindow();
					}
					else if (mIsMinimized)
					{
						mAppPaused = false;
						mIsMinimized = false;
						ResizeWindow();
					}
					else if (mIsBeingResized)
					{
						/*As long as the user is trying to resize
						via them handle bars, don't try to resize.
						Wait for WM_EXITSIZEMOVE message.*/
					}
					else
					{
						ResizeWindow();
					}
				}
			}
			break;
		}
		
		//User is grabbing them resize bars.
		case WM_ENTERSIZEMOVE:
		{
			mAppPaused = true;
			mIsBeingResized = true;
			//mTimer.Stop();
			break;
		}

		//User has released them resize bars.
		case WM_EXITSIZEMOVE:
		{
			mAppPaused = false;
			mIsBeingResized = false;
			mTimer.Restart();
			ResizeWindow();
			break;
		}
		
		//Window Destruction
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		
		//Mouse Input, using Windowsx.h Macros and fallthrough
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
		case WM_MOUSEMOVE:
		{
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool D3DBase::Win32Init()
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = mAppInst;
	wndClass.lpfnWndProc = MainWndProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	wndClass.lpszClassName = L"D3DClassName";
	RegisterClassEx(&wndClass);

	RECT windowSize = { 0, 0, mWindowWidth, mWindowHeight};
	AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, false);
	auto width = windowSize.right - windowSize.left;
	auto height = windowSize.bottom - windowSize.top;

	mMainWnd = CreateWindow(
		L"D3DClassName",
		mMainWindowTitleBarText.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		mAppInst,
		NULL);

	ShowWindow(mMainWnd, SW_SHOW);
	UpdateWindow(mMainWnd);

	return true;
}

bool D3DBase::Direct3DInit()
{
	UINT deviceCreationFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
#ifdef _DEBUG
	deviceCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		//D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numOfFeatureLevels = ARRAYSIZE(featureLevels);

	//Create Device Alone to check MSAA Support
	D3D11CreateDevice(
		NULL,
		mD3DDriverType,
		NULL,
		deviceCreationFlags,
		featureLevels,
		numOfFeatureLevels,
		D3D11_SDK_VERSION,
		&mD3DDevice,
		&featureLevel,
		&mD3DDeviceContext);

	mD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMSAAQualityLevel);

	//Swapchain Desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = mWindowWidth;
	swapChainDesc.BufferDesc.Height = mWindowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.OutputWindow = mMainWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (mEnable4xMSAA)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m4xMSAAQualityLevel - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	//IDXGI Functions Yay - Swapchain must be created with same factory
	//as the device, traverse the heirarchy, then create the swapchain.
	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;
	mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(mD3DDevice, &swapChainDesc, &mSwapChain);
	
	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	ResizeWindow();
	return true;
}

void D3DBase::CalcFPS()
{
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frameCount);
		std::wostringstream OSS;
		OSS << mMainWindowTitleBarText << L"      " << L"FPS: " << fps;
		SetWindowText(mMainWnd, OSS.str().c_str());

		//Reset
		frameCount = 0;
		timeElapsed += 1.0f;
	}
}