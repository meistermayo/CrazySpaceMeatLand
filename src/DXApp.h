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
#include "Graphics/d3dUtil.h"
#include "Graphics/Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Graphics/Math/Vect.h"
#include "Graphics/Math/Matrix.h"

class Camera;
class EyeballRing;
class FlatPlane;
class GraphicsObject_Color;
class GraphicsObject_Sprite;
class GraphicsObject_Texture;
class GraphicsObject_TextureLight;
class GraphicsObject_Wireframe;
class Image;
class Model;
class ShaderWireframe;
class ShaderTexture;
class ShaderColorLightTexture;
class Skybox;
class Texture;
class TerrainModel;
class Worm;

struct Rect;

class DXApp : public Align16
{
	struct UserData
	{
		float frigateSpotRange = 1000;
		float frigateSpotAtten = .01f;
	};
private:
	UserData userdata;
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	GameTimer mTimer;
	std::wstring mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Demo specific additions
	Camera* pCam3D;
	Camera* pCam2D;

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
	GraphicsObject_TextureLight* Cube1;
	GraphicsObject_TextureLight* Cube2;
	GraphicsObject_TextureLight* Cube3;

	ShaderTexture* pShader_Sprite;
	Model* pModel_Sprite;
	Texture* pTex_Sprite;
	Image* pImage_Sprite;
	Rect* pRect_Sprite;
	GraphicsObject_Sprite* pGO_Sprite;

	ShaderWireframe* pShader_WF;
	Model* pModel_WF;
	GraphicsObject_Wireframe* pGO_WF;


	// WORMY BOI >:D
	Worm* worm;

	// Frigate
	Model* pModel_Frigate;
	Texture* pTex_Frigate;
	Matrix world_Frigate;
	GraphicsObject_TextureLight* GO_Frigate;
#ifdef _TEST
	Model* _TEST_model;
	Texture* _TEST_tex;
	GraphicsObject_Texture* _TEST_go;
	Model* CubeModel;
	GraphicsObject_TextureLight* CubeGo;
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
	//GraphicsObject_Texture*	GraphObj1;
	//GraphicsObject_Texture*	GraphObj2;
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