#pragma once
#include "D3DUtils.h"
#include "XTime.h"

class D3DBase
{
	//Class Functions
	public:
		D3DBase(HINSTANCE hInstance);
		~D3DBase();

		//Trivial Accessors
		HINSTANCE AppInst() const;
		HWND MainWnd() const;
		float AspectRatio() const;

		//Handle Win32 Messages and D3D
		UINT Run();
		//Init Win32 and D3D, if either fails, this fails.
		virtual bool Init();
		//Resize Buffers
		virtual void ResizeWindow();
		//Game Logic - Pure Virtual
		virtual void Update(float deltaTime) = 0;
		//Rendering - Pure Virtual
		virtual void Draw() = 0;
		//Message Handling - Optional Override
		virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//Mouse Input handled via WinProc, maybe DirectInput later.
		virtual void OnMouseDown(WPARAM theButton, int x, int y){ }
		virtual void OnMouseUp(WPARAM theButton, int x, int y)  { }
		virtual void OnMouseMove(WPARAM theButton, int x, int y){ }

	protected:
		//Create/Register a class and a window
		bool Win32Init();
		//Initialize Necessary D3D/DXGI Objects
		bool Direct3DInit();
		//Push FPS info to Window TitleBar
		void CalcFPS();

	//Class Data Members
	protected:
		//Win32
		HINSTANCE mAppInst;
		HWND mMainWnd;
		std::wstring mMainWindowTitleBarText;
		bool mAppPaused;
		bool mIsMinimized;
		bool mIsMaximized;
		bool mIsBeingResized;
		UINT mWindowWidth;
		UINT mWindowHeight;

		//Direct3D
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DDeviceContext;
		IDXGISwapChain* mSwapChain;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		D3D11_VIEWPORT mViewPort;
		D3D_DRIVER_TYPE mD3DDriverType;
		UINT m4xMSAAQualityLevel;
		bool mEnable4xMSAA;

		//Utils
		XTime mTimer;
};