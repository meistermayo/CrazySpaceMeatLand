#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>

// needed to load shaders from file
#include <d3dcompiler.h>

#include "DirectXTex.h"

#include "Graphics/d3dUtil.h"
#include "Graphics/GraphicsObject/GraphicsObject_Color.h"
#include "Graphics/GraphicsObject/GraphicsObject_TextureLight.h"

#include "Graphics/Model/Model.h"
#include "Graphics/Model/FbxModelLoader.h"
#include "Graphics/Model/TerrainModel.h"
#ifdef SKYBOX
#include "Graphics/Model/Skybox.h"
#endif

#include "Graphics/Shader/ShaderColor.h"
#include "Graphics/Shader/ShaderColorLightTexture.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Math/Constants.h"

#include "FlatPlane.h"
#include "EyeballRing.h"
#include "Worm.h"

/*/
where could my code be failing

- shader is not running the right code
- shader settings are not set up correctly
- shader calls are not being made
- bad shader mat
- bad go mat
- graphic object not setting things up correctly
- camera is being sent incorrectly
//*/

void DXApp::InitDemo()
{
	Vect pointLightPos1 = Vect(-164, 80, 164);
	float pointLightRadius1 = 100;
	Vect pointLightAtt1 = Vect(0, 1, 0) * .02f;
	Vect pointLightAmb1 = Vect(1, 0.5, 0) * .001f;
	Vect pointLightDif1 = Vect(1, 0.5, 0) * .5f;
	Vect pointLightSpc1 = Vect(1, 1, 1, 1) * 1.f;

	Vect pointLightPos2 = Vect(164, 80, -164);
	float pointLightRadius2 = 100;
	Vect pointLightAtt2 = Vect(0, 1, 0) * .02f;
	Vect pointLightAmb2 = Vect(1, 0.5, 0) * .001f;
	Vect pointLightDif2 = Vect(1, 0.5, 0) * .5f;
	Vect pointLightSpc2 = Vect(1, 1, 1, 1) * 0.5f;

	Vect spotLightPos = Vect(0, 128, 256);
	float spotLightRadius = 100;
	Vect spotLightAtt = Vect(0, 1, 0) * .02f;
	Vect spotDir = Vect(0,10,64);
	float spotExp = 1;
	Vect spotLightAmb = Vect(0, 0, 1) * .1f;
	Vect spotLightDif = Vect(0, 0, 1) * .5f;
	Vect spotLightSpc = Vect(.3f, .6f, 1, 150) * 0.5f;

	pShaderTex = new ShaderTexture("../Assets/Shaders/D3D/Texture.hlsl");

	pShaderTexLight = new ShaderColorLightTexture("../Assets/Shaders/D3D/ColorLightTexture.hlsl");

	pShaderTexLight->SetPointLightParameters1(pointLightPos1, pointLightRadius1, pointLightAtt1, pointLightAmb1, pointLightDif1, pointLightSpc1);
	pShaderTexLight->SetPointLightParameters2(pointLightPos2, pointLightRadius2, pointLightAtt2, pointLightAmb2, pointLightDif2, pointLightSpc2);
	pShaderTexLight->SetSpotLightParameters(spotLightPos , spotLightRadius, spotLightAtt, spotDir, spotExp, spotLightAmb, spotLightDif, spotLightSpc);

	eyeballRing = new EyeballRing(pShaderTexLight);
	
	// WORMY BOI
	worm = new Worm(pShaderTexLight);

	// FRIGATE )))))))
	pModel_Frigate = new Model("../Assets/Models/space_frigate.azul",false,false,.2f);
	pTex_Frigate = new Texture("../Assets/Textures/space_frigate.tga");
	world_Frigate = Matrix::RotY(MATH_PI) * Matrix::RotZ(0.f) * Matrix::Trans(Vect(40.f, 60.f, 40.f));
	GO_Frigate = new GraphicsObject_TextureLight(pShaderTexLight,pModel_Frigate);
	GO_Frigate->SetWorld(world_Frigate);
	GO_Frigate->SetTexture(pTex_Frigate, 0);

	// Cube
	FbxModelLoader fbxModelLoader = FbxModelLoader();
	FbxModelInfo fbxModelInfo = fbxModelLoader.GetModel("../Assets/Models/mouse.fbx");
	pModel_Cube1 = new Model(fbxModelInfo.meshInfo[0]);
	pModel_Cube2 = new Model(fbxModelInfo.meshInfo[1]);
	pModel_Cube3 = new Model(fbxModelInfo.meshInfo[2]);
	//pModel_Cube = new Model(Model::UnitSphere, 12.0f);
	mWorld_Cube = new Matrix(Matrix::Trans(Vect(0.f, 10.f, 0.f))); /// ??? pointer???
	pShader_Cube = new ShaderColorLightTexture("../Assets/Shaders/D3D/ColorLightTexture.hlsl");
	Cube1 = new GraphicsObject_TextureLight((ShaderColorLightTexture*)(pShader_Cube), pModel_Cube1);// Vect(1, 0, 0, 1), Vect(1, 0, 0, 1), Vect(1, 1, 1, 1));
	Cube2 = new GraphicsObject_TextureLight((ShaderColorLightTexture*)(pShader_Cube), pModel_Cube2);// Vect(1, 0, 0, 1), Vect(1, 0, 0, 1), Vect(1, 1, 1, 1));
	Cube3 = new GraphicsObject_TextureLight((ShaderColorLightTexture*)(pShader_Cube), pModel_Cube3);// Vect(1, 0, 0, 1), Vect(1, 0, 0, 1), Vect(1, 1, 1, 1));
	pTex_Cube = new Texture("../Assets/Textures/mouse.tga", D3D11_FILTER_MIN_MAG_MIP_POINT, 4U, FALSE, 0U, DirectX::TEX_FILTER_POINT);
	Cube1->SetTexture(pTex_Cube, 0);
	Cube2->SetTexture(pTex_Cube, 0);
	Cube3->SetTexture(pTex_Cube, 0);
	//Cube->SetColor(Vect(1, 0, 0, 1), 0);
	Cube1->SetWorld(*mWorld_Cube);
	Cube2->SetWorld(*mWorld_Cube);
	Cube3->SetWorld(*mWorld_Cube);

	pShader_Cube->SetPointLightParameters1(pointLightPos1, pointLightRadius1, pointLightAtt1, pointLightAmb1, pointLightDif1, pointLightSpc1);
	pShader_Cube->SetPointLightParameters2(pointLightPos2, pointLightRadius2, pointLightAtt2, pointLightAmb2, pointLightDif2, pointLightSpc2);
	pShader_Cube->SetSpotLightParameters(spotLightPos, spotLightRadius, spotLightAtt, spotDir, spotExp, spotLightAmb, spotLightDif, spotLightSpc);

#ifdef _TEST
	_TEST_model = new Model("../Assets/Models/CubeTest2.azul");
	_TEST_tex = new Texture(L"../Assets/Textures/CubeTex.tga");
	_TEST_go = new GraphicsObject_Texture(pShaderTex, _TEST_model);
	_TEST_go->SetWorld(Matrix::Scale( 1, 1, 1) * Matrix::Trans(0, 10, 0));
	_TEST_go->SetTexture(_TEST_tex,0);
	CubeModel = new Model(Model::PreMadeModels::UnitBoxRepeatedTexture);
	CubeGo = new GraphicsObject_TextureLight(pShaderTexLight, CubeModel);
	CubeGo->SetWorld(Matrix::Scale( 10, 10, 10)*Matrix::Trans(5, 10, 5));
	CubeGo->SetTexture(ppTex_WormyBoi[0],0);
#endif
#ifdef TERRAIN
	pTerrain_Texture = new Texture("../Assets/Textures/brownsand.tga");
	float len = 3;
	pTerrain = new TerrainModel("../Assets/Textures/canyon2.tga", len, 50.f, 0.f, 8, 8);
	pTerrain_Shader = new ShaderTexture("../Assets/Shaders/D3D/Texture.hlsl");
	pTerrain_World = new Matrix(Matrix::Trans(Vect(-128.0f*len, 0.0f, -128.0f*len))); // why tf is this a pointer???
#endif

#ifdef SKYBOX
	Matrix _tempMatrix = Matrix::Scale(1.f) * Matrix::Trans(Vect::Zero); // why?
	pSkyBox_World = new Matrix(_tempMatrix);
	pSkyBox_Texture = new Texture("../Assets/Textures/redspace.tga");
	pSkyBox_Shader = new ShaderTexture("../Assets/Shaders/D3D/Texture.hlsl");
	pSkyBox = new Skybox(pSkyBox_Shader, pSkyBox_Texture);
#endif
#ifdef FLATPLANE
	flatPlane = new FlatPlane(1000,1,1);
	flatPlane_World = Matrix::Scale(1,1,1) * Matrix::Trans(0, 2, 0);

#endif

	// Initialize the projection matrix
	mCam.setPerspective( 3.14159f*.45f, mClientWidth / (float)mClientHeight, 0.0001f, 5000.0f);
	mCam.setOrientAndPosition(
		Vect( - 0.0507903174f, 0.857880533f, 0.511332929f),
		Vect(-2.04233885f, 23.4840279f, -16.0365753f),
		Vect(-1.95754349f, 23.9978771f, -16.8902550f));

	// Initialize gimmicky mouse control values
	mTheta = .5f;
	mPhi = 3.1415f / 8.f;
	mRadius = 6.f;

	mTimer.Reset();
}


// make me smooth
void DXApp::UpdateScene()
{
	GO_Frigate->SetWorld(world_Frigate);

	pShaderTexLight->SetPointLightParameters3(
		world_Frigate.GetTrans() + Vect(0, -59.9f, 0), userdata.frigateSpotRange,
		Vect(0, 1, 0) * userdata.frigateSpotAtten,

		Vect(.7f, .7f, 1) * 1.f,
		Vect(.7f, .7f, 1) * 3.f,
		Vect(1, 1, 1, 1) * 1.f);

	pShader_Cube->SetPointLightParameters3(
		world_Frigate.GetTrans() + Vect(0, -59.9f, 0), userdata.frigateSpotRange,
		Vect(0, 1, 0) * userdata.frigateSpotAtten,

		Vect(.7f, .7f, 1)	* 1.f,
		Vect(.7f, .7f, 1)	* 3.f,
		Vect(1, 1, 1, 1)* 1.f);

	float camSpeed = 40 * mTimer.DeltaTime();
	if (GetKeyState('W') & 0x08000)
	{
		mCam.TranslateFwdBack(camSpeed);
	}
	else if (GetKeyState('S') & 0x08000)
	{
		mCam.TranslateFwdBack(-camSpeed);
	}

	if (GetKeyState('A') & 0x08000)
	{
		mCam.TranslateLeftRight(-camSpeed);
	}
	else if (GetKeyState('D') & 0x08000)
	{
		mCam.TranslateLeftRight(camSpeed);
	}
	else if (GetKeyState('R') & 0x08000)
	{
		userdata.frigateSpotRange += .01f;
	}
	else if (GetKeyState('F') & 0x08000)
	{
		userdata.frigateSpotRange -= .01f;
	}
	else if (GetKeyState('T') & 0x08000)
	{
		userdata.frigateSpotAtten += .0001f;
	}
	else if (GetKeyState('G') & 0x08000)
	{
		userdata.frigateSpotAtten -= .0001f;
	}
	else if (GetKeyState('Q') & 0x08000)
	{
		world_Frigate *= Matrix::RotY(-MATH_PI / 5000.f);
	}
	else if (GetKeyState('E') & 0x08000)
	{
		world_Frigate *= Matrix::RotY(MATH_PI / 5000.f);
	}

	float rotSpeed = 3 * mTimer.DeltaTime();
	if (GetKeyState(VK_LEFT) & 0x08000)
	{
		mCam.TurnLeftRight(rotSpeed);
	}
	else if (GetKeyState(VK_RIGHT) & 0x08000)
	{
		mCam.TurnLeftRight(-rotSpeed);
	}

	if (GetKeyState(VK_UP) & 0x08000)
	{
		mCam.TiltUpDown(rotSpeed);
	}
	else if (GetKeyState(VK_DOWN) & 0x08000)
	{
		mCam.TiltUpDown(-rotSpeed);
	}

	mCam.updateCamera();
}

void DXApp::DrawScene()
{
	Vect eyepos;
	mCam.getPos(eyepos);

	float fogStart = 128;
	float fogRange = 256;
	Vect fogCol = Vect(0.25f, 0, 0, 1);

#ifdef _TEST
	pShaderTex->SetToContext();
	pShaderTex->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	//pShaderTex->SendWorld();
	_TEST_go->Render(&mCam);
	pShaderTexLight->SetToContext();
	pShaderTexLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	CubeGo->Render(&mCam);
#endif 
#ifdef SKYBOX
	Vect camPos;
	mCam.getPos(camPos);
	pSkyBox_World->SetTrans(camPos);
	pSkyBox_Shader->SetToContext();
	pSkyBox_Shader->SendFogData(500, 5000, fogCol);
	pSkyBox_Shader->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pSkyBox_Shader->SendWorld(*pSkyBox_World);
	pSkyBox->Render();
#endif

	eyeballRing->Render(&mCam, eyepos, fogStart, fogRange, fogCol);
	worm->Render(&mCam, eyepos, fogStart, fogRange, fogCol);

	GO_Frigate->Render(&mCam);


#ifdef TERRAIN
	//pTerrain_Shader->SetToContext();
	//pTerrain_Shader->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pTerrain_Texture->SetToContext();
	pShaderTexLight->SendWorldAndMaterial(*pTerrain_World);

	//pShaderColLight->SetToContext();
	//pShaderColLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	//pShaderColLight->SendLightParameters(eyepos);
	//pShaderColLight->SendWorldAndMaterial(*pTerrain_World);

	pTerrain->Render();
#endif
#ifdef FLATPLANE
	//pShaderTexLight->SendWorldAndMaterial(flatPlane_World);
	pShaderColLight->SetToContext();
	pShaderColLight->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
	pShaderColLight->SendLightParameters(eyepos);
	pShaderColLight->SendWorldAndMaterial(flatPlane_World);
	//flatPlane->Render();
#endif
	pShader_Cube->SetToContext();
	pShader_Cube->SendFogData(fogStart, fogRange, fogCol);
	pShader_Cube->SendLightParameters(eyepos);
	pShader_Cube->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	Cube1->Render(&mCam);
	Cube2->Render(&mCam);
	Cube3->Render(&mCam);
}


DXApp::DXApp(HWND hwnd)
{
	assert(hwnd);
	mhMainWnd = hwnd;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	WCHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);

	mMainWndCaption = str;

	// Demo initialization
	this->InitDemo();
}

DXApp::~DXApp()
{
	delete pModel_Cube1;
	delete pModel_Cube2;
	delete pModel_Cube3;
	delete mWorld_Cube;
	delete pShader_Cube;
	delete Cube1;
	delete Cube2;
	delete Cube3;


#ifdef _COL_L
	delete pShaderColLight;
#endif

	delete pShaderTexLight;
	delete pShaderTex;

	delete eyeballRing;
	delete worm;

#ifdef _TEST
	delete _TEST_model;
	delete _TEST_tex;
	delete _TEST_go;
	delete CubeGo;
	delete CubeModel;
#endif
#ifdef TERRAIN
	delete pTerrain_Texture;
	delete pTerrain;
	delete pTerrain_World;
	delete pTerrain_Shader;
#endif
#ifdef SKYBOX
	delete pSkyBox_World;
	delete pSkyBox;
	delete pSkyBox_Texture;
	delete pSkyBox_Shader;
#endif
#ifdef FLATPLANE
	delete flatPlane;
#endif
}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::FrameTick()
{
	mTimer.Tick();
	CalculateFrameStats();

	this->UpdateScene();
	this->DrawScene();
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	// Gimmicky mouse control
	if (btnState & MK_LBUTTON)
	{
		Vect camPos, camDir;
		mCam.getPos(camPos);
		mCam.getDir(camDir);
		Matrix camWorld;
		camWorld = Matrix::RotY(0) * Matrix::Scale(1.f) * Matrix::Trans(camPos);

		//myBullet->Activate(0.1f*(((-camDir)).GetNormalized()), camWorld);
		MousePos.x = static_cast<float>(xval);
		MousePos.y = static_cast<float>(yval);
	}
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	btnState;
	xval;
	yval;
	/*
	// Gimmicky mouse control
	if (btnState & MK_LBUTTON)
	{
		float dx = 0.01f*(MousePos[x] - xval);
		float dy = 0.01f*(MousePos[y] - yval);

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi -= dy;

		static float verticalMax = MATH_PI2 - 0.2f;

		if (mPhi > verticalMax)
			mPhi = verticalMax;
		else if (mPhi < -verticalMax)
			mPhi = -verticalMax;

		// Build the view matrix using gimmicky trick
		Vect target = Vect(0, 0, 0, 0);
		Vect up = Vect(0, 1, 0, 0);
		Vect pos = Vect(0, 0, -mRadius) * Matrix::RotY(mTheta) * Matrix::RotX(mPhi);
		mCam.setOrientAndPosition(up, target, pos);
	}

	MousePos[x] = static_cast<float>(xval);
	MousePos[y] = static_cast<float>(yval);
	*/
}

void  DXApp::OnMouseWheel(short delta)
{
	if (delta > 0)
	{
		mRadius -= 1;
	}
	else
	{
		mRadius += 1;
	}
}


// Shader loading utility. Will be moved elsewhere later...
// Needs to be moved. Requires
HRESULT DXApp::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}