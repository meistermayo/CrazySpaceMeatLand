#ifndef GRAPHICS_BACKEND_H
#define GRAPHICS_BACKEND_H

#define BACKEND_D3D

#include "src/Graphics/CrazySpaceMeatLand/src/Align16.h"

class Model;
struct StandardVertex;
struct TriangleByIndex;

#ifdef BACKEND_D3D

#include <WindowsX.h>
#include "src/Graphics/CrazySpaceMeatLand/DX Application/Resource.h"
#include <cassert>
#include "src/Graphics/CrazySpaceMeatLand/src/DXApp.h"
#include "src/i Engine/i.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Math/Vect.h"

#endif

struct GraphicsDevice;
struct GraphicsContext;

class GraphicsBackend_Base : public Align16 {
	friend class GraphicsBackend;

protected:
	virtual int privInitialize() = 0;
	virtual void privInitApp() = 0;
	virtual bool privStillOpen() = 0;
	virtual void privPoll() = 0;
	virtual void privPrepare() = 0;
	virtual void privPresent() = 0;
	virtual void privCleanupApp() = 0;
	virtual void privSetClearColor(float r, float g, float b, float a = 1.0f) = 0;
	virtual const GraphicsDevice& privGetDevice() const = 0;
	virtual const GraphicsContext& privGetContext() const = 0;
};

struct GraphicsDevice : public Align16
{
#ifdef BACKEND_D3D
	ID3D11Device* md3dDevice = nullptr;
#endif
};

struct GraphicsContext : public Align16
{
#ifdef BACKEND_D3D
	ID3D11DeviceContext* md3dImmediateContext;
#endif
};

struct VertexBufferObject : public Align16
{
	int num;
	StandardVertex* pVerts;
#ifdef BACKEND_D3D
	ID3D11Buffer* mpVertexBuffer;

	~VertexBufferObject();
#endif

	void LoadToGPU();
	void Bind();
};

struct IndexBufferObject : public Align16
{
	int num;
	TriangleByIndex* pTris;
#ifdef BACKEND_D3D
	ID3D11Buffer* mpIndexBuffer;

	~IndexBufferObject();
#endif

	void LoadToGPU();
	void Bind();
};

#ifdef BACKEND_OGL
class OGL_GraphicsBackend : public GraphicsBackend_Base {
public:
};
#endif

#ifdef BACKEND_D3D
class D3D_GraphicsBackend : public GraphicsBackend_Base
{
	HINSTANCE               hInst = nullptr;
	HWND                    hWnd = nullptr;
	int nCmdShow = 0;

	Vect BackgroundColor;

	// DX application elements
	GraphicsDevice mDev;						// Connects to the graphics card
	GraphicsContext mCon;						// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	std::wstring mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	MSG msg = { 0 };

	void SetData(HINSTANCE hInstance, int inCmdShow);

protected:
	virtual int privInitialize() override;

	virtual void privInitApp() override;

	virtual bool privStillOpen() override;

	virtual void privPoll() override;

	virtual void privPrepare() override;

	virtual void privPresent() override;

	virtual void privCleanupApp() override;

	virtual void privSetClearColor(float r, float g, float b, float a);

	virtual const GraphicsDevice& privGetDevice() const override;
	virtual const GraphicsContext& privGetContext() const override;

	void InitDirect3D();

protected:
	HRESULT InitWindow()
	{
		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = LoadIcon(hInst, (LPCTSTR)IDI_TUTORIAL1);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = (LPCSTR)L"MainlWindowClass";
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
		if (!RegisterClassEx(&wcex))
			return E_FAIL;

		// Create window
		RECT rc = { 0, 0, 1920, 1080 }; // Initial window dimentions. 

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow((LPCSTR)L"MainlWindowClass", (LPCSTR)L"Main Window",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInst,
			nullptr);
		if (!hWnd)
			return E_FAIL;

		// Note: We won't bother dealing with resizing/fullscreen, etc

		ShowWindow(hWnd, nCmdShow);

		return S_OK;
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		D3D_GraphicsBackend* pBackend = (D3D_GraphicsBackend*)i::GetGraphicsBackend();
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
			// Early exit using ESC key: very useful during development
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetCapture(pBackend->hWnd);
			i::BackendOnMouseButton(wParam, 1);
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			ReleaseCapture();
			i::BackendOnMouseButton(wParam, 0);
			break;

		case WM_MOUSEMOVE:
			i::BackendOnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_MOUSEWHEEL:
			i::BackendOnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

};
#endif

class GraphicsBackend : public Align16 {
	static GraphicsBackend_Base* pInst;

	static GraphicsBackend_Base* Instance()
	{
		if (pInst == nullptr)
		{
#ifdef BACKEND_D3D
			pInst = new D3D_GraphicsBackend();
#endif
#ifdef BACKEND_OGL
			pInst = new OGL_GraphicsBackend();
#endif
		}

		return pInst;
	}

public:
	static int Initialize() { return Instance()->privInitialize(); }
	static void InitApp() { Instance()->privInitApp(); }
	static bool StillOpen() { return Instance()->privStillOpen(); }
	static void Poll() { Instance()->privPoll(); }
	static void Prepare() { Instance()->privPrepare(); }
	static void Present() { Instance()->privPresent(); }
	static void CleanupApp() { Instance()->privCleanupApp(); }
	static void SetClearColor(float r, float g, float b, float a = 1.0f) { Instance()->privSetClearColor(r, g, b, a); }
	static const GraphicsDevice& GetDevice() { return Instance()->privGetDevice(); }
	static const GraphicsContext& GetContext() { return Instance()->privGetContext(); }
};


#endif