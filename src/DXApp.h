// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

/*/
#ifndef _COL_L
#define _COL_L
#endif
//*/

/*/
#ifndef _TEST
#define _TEST
#endif
//*/
/*/
#ifndef FLATPLANE
#define FLATPLANE
#endif
//*/
//*/
#ifndef TERRAIN
#define TERRAIN
#endif
//*/

//*/
#ifndef SKYBOX
#define SKYBOX
#endif
//*/

#ifndef _DXApp
#define _DXApp
#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Math/Vect.h"
#include "Math/Matrix.h"
#include "Camera.h"
#include "ShaderColor.h"
#include "ShaderColorLight.h"
#include "src/Graphics/CrazySpaceMeatLand/src/GraphicObject_Texture.h"
#include "src/Graphics/CrazySpaceMeatLand/src/GraphicObject_TextureLight.h"
#include "Skybox.h"
#include "TerrainModel.h"

class Model;
class Texture;
class FlatPlane;
class TerrainModel;
class GraphicObject_Color;
class ShaderTexture;
class GraphicObject_Texture;
class EyeballRing;
class Worm;
class DXApp : public Align16
{
	struct UserData
	{
		float frigateSpotRange = 100;
		float frigateSpotAtten = .001f;
	};
private:
	UserData userdata;
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	GameTimer mTimer;
	std::wstring mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Debug utility
	void ReportLiveDXObjects();

	// Demo specific additions
	Camera mCam;

#ifdef _COL_L
	ShaderColorLight* pShaderColLight;
#endif
	ShaderTexture* pShaderTex;
	ShaderColorLightTexture* pShaderTexLight;

	// EYEBALL BOI 0.0
	EyeballRing* eyeballRing;

	// Cube
	ShaderColorLightTexture* pShader_Cube;
	Model* pModel_Cube1;
	Model* pModel_Cube2;
	Model* pModel_Cube3;
	Matrix* mWorld_Cube;
	Texture* pTex_Cube;
	GraphicObject_TextureLight* Cube1;
	GraphicObject_TextureLight* Cube2;
	GraphicObject_TextureLight* Cube3;

	// WORMY BOI >:D
	Worm* worm;

	// Frigate
	Model* pModel_Frigate;
	Texture* pTex_Frigate;
	Matrix world_Frigate;
	GraphicObject_TextureLight* GO_Frigate;
#ifdef _TEST
	Model* _TEST_model;
	Texture* _TEST_tex;
	GraphicObject_Texture* _TEST_go;
	Model* CubeModel;
	GraphicObject_TextureLight* CubeGo;
#endif
#ifdef TERRAIN
	Matrix* pTerrain_World;
	Texture* pTerrain_Texture;
	TerrainModel* pTerrain;
	ShaderTexture* pTerrain_Shader;
#endif
#ifdef SKYBOX
	Matrix* pSkyBox_World;
	Skybox* pSkyBox;
	ShaderTexture* pSkyBox_Shader;
	Texture* pSkyBox_Texture;
#endif
#ifdef FLATPLANE
	FlatPlane* flatPlane;
	Matrix flatPlane_World;
#endif
	//ShaderTexture*		pShaderTex;
	//GraphicObject_Texture*	GraphObj1;
	//GraphicObject_Texture*	GraphObj2;
	//Texture*				pTex1;
	//Texture*				pTex2;

	// gimmicky mouse control
	Vect MousePos;		
	float mTheta;
	float mPhi;
	float mRadius;
	
public:
	DXApp(HWND hwnd);
	virtual ~DXApp();
	
	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);
	void OnMouseWheel(short delta);


};

#endif _DXApp