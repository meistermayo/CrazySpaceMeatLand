//--------------------------------------------------------------------------------------
// File: MainWindow.cpp
//
// Heavily inspired by http://msdn.microsoft.com/en-us/library/windows/apps/ff729718.aspx
#include <WindowsX.h>
#include "Resource.h"
#include "../src/DXApp.h"
#include "Graphics/GraphicsBackend.h"
#include <assert.h>

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
#ifdef BACKEND_D3D
	((D3D_GraphicsBackend*)GraphicsBackend::Instance())->SetData(hInstance, nCmdShow);
#endif
#ifdef BACKEND_OGL
	((OGL_GraphicsBackend*)GraphicsBackend::Instance())->SetData();
#endif
    GraphicsBackend::Initialize();
    GraphicsBackend::InitApp();

#ifdef BACKEND_D3D
	HWND hWnd = ((D3D_GraphicsBackend*)GraphicsBackend::Instance())->GetWindowHandle();
#endif
#ifdef BACKEND_OGL
	HWND hWnd = ((OGL_GraphicsBackend*)GraphicsBackend::Instance())->GetWindowHandle();
#endif

	// Initialize the application that will make use of this window.
	// In this case, it's our DX11 demo
	DXApp* pDxApp = new DXApp(hWnd);

    // Main message loop and application calls
	// This is how MS/Windows deal with various input/widgets/UI/Menu stuff
	int msgValue = 0;
    while( GraphicsBackend::StillOpen() )
	{
		GraphicsBackend::Prepare();
		pDxApp->FrameTick();
		GraphicsBackend::Present();

		msgValue = GraphicsBackend::Poll();
	}

	// Shutting down the app
	delete pDxApp;
    GraphicsBackend::CleanupApp();

	return msgValue;
}
