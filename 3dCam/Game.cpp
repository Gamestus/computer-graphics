#include "Game.h"
#include "CollisionRect.h"
#include "MeshComponent.h"
#include "Camera.h"
#include "Strsafe.h"
#include "Sphere.h"
#include "Planet.h"
#include "Rotator.h"
#include "FlexingCube.h"
#include "AssimpMesh.h"
#include "CatamariPlayer.h"
#include <filesystem>
#include <cstdlib>
#include "ShadowRenderTarget.h"
#include "GRenderTarget.h"
#include "DepthShader.h"
#include "DirectionalLight.h"
#include "ScreenQuad.h"

namespace fs = std::filesystem;

Game* Game::Instance = nullptr;

LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		/* wchar_t buffer[256];
		wsprintf(buffer, L"Key pressed: %d\n", static_cast<unsigned int>(wparam));
		OutputDebugStringW(buffer); */

		Game::Instance->InDevice->AddPressedKey(static_cast<unsigned int>(wparam));
		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		break;
	}
	case WM_KEYUP:
	{
		Game::Instance->InDevice->RemovePressedKey(static_cast<unsigned int>(wparam));
		break;
	}
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		Game::Instance->InDevice->MouseMove(pt);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		Game::Instance->InDevice->MouseLeftClick(pt);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		Game::Instance->InDevice->MouseRightClick(pt);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

Game::Game(HINSTANCE hInstanceNew) {
	Game::Instance = this;
	Initialize(hInstanceNew);
}

void Game::Initialize(HINSTANCE hInstanceNew) {
	InDevice = new InputDevice();
	Display = new DisplayWin32(hInstanceNew, MessageHandler);
	PhysServer = new PhysicsServer();
	CreateSwapChain();
	CreateBackBuffer();
	CreateDepthStencilBuffer();
	CreateShadowsRT();
	UpdateViewport();


	pQuad = new ScreenQuad();
	RootComponent = new GameComponent();


	// Add Player
	auto player = RootComponent->AddChild<CatamariPlayer>(std::make_unique<CatamariPlayer>());
	CurrCam = player->Cam;

	//RootComponent->AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models\\UV_cube.obj"));

	//RootComponent->AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models\\house\\Banana_low.obj",L"models\\house\\BananaShdr_albedo.jpeg", 4.0));



	std::string folderPath = "models/house/";


	for (int i = 0; i < 1; i++)
	{
		for (const auto& entry : fs::directory_iterator(folderPath)) {
			std::string filePath = entry.path().string();

			if (filePath.find(".obj") != std::string::npos) {
				std::string texturePath = filePath;
				texturePath.replace(texturePath.find("_low.obj"), 8, "Shdr_albedo.jpeg");

				AssimpMesh* mesh = RootComponent->AddChild<AssimpMesh>(
					std::make_unique<AssimpMesh>(filePath, std::wstring(texturePath.begin(), texturePath.end()).c_str(), 4.0, true)
				);
				float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 60.0f - 30.0f;
				float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 60.0f - 30.0f;

				SimpleMath::Vector3 randomPosition(x, 0.0f, z);
				mesh->SetLocalPosition(randomPosition);
				//mesh->SetLocalRotation(Vector3(1,0,0));
			}
		}
	}
	AssimpMesh* mesh = RootComponent->AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models/plane.obj", L"models/house/DcolaBottleShdr_albedo.jpeg"));
	mesh->SetLocalPosition(Vector3(0,-1,0));

	pLight = RootComponent->AddChild<DirectionalLight>(std::make_unique<DirectionalLight>());
}

Game::~Game() {
	delete[] Display;
	delete[] InDevice;
}

std::optional<int> Game::processMessages() {


	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		
		if (msg.message == WM_QUIT){
			return msg.wParam;
		}
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return {};
}

int Game::Run() {
	while (true)
	{

		if (const auto ecode = processMessages()) {
			return *ecode;
		}

		Update(timer.Mark());
		Draw();
		
	}

}

void Game::UpdateViewport() {

	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

void Game::Update(float delta) {
	RootComponent->UpdateChildren(delta);
	PhysServer->UpdatePhysics();
}

void Game::Draw() {

	DeviceContext->ClearState();

	//Render to Texture
	pRenderTexture->ClearRenderTarget(1.0, 1.0, 1.0, 1.0);
	pRenderTexture->SetRenderTarget();
	RootComponent->DrawChildren(true);

	//Render To Screen

	pGRenderTarget->ClearRenderTarget(0.1, 0.1, 0.1, 0.1);
	pGRenderTarget->SetRenderTarget();
	RootComponent->DrawChildren(false);

	//Render Quad
	DeviceContext->RSSetViewports(1, &viewport);

	float color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	DeviceContext->ClearRenderTargetView(RenderView, color);
	DeviceContext->ClearDepthStencilView(DSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	DeviceContext->OMSetRenderTargets(1, &RenderView, DSView);

	//RootComponent->DrawChildren(false);
	pQuad->Draw(false);


	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::CreateBackBuffer() {
	auto res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	if (FAILED(res))
	{
		OutputDebugStringW(L"GetBuffer! Oh, that was unexpected!");
	}
	res = WrlDevice->CreateRenderTargetView(BackBuffer, nullptr, &RenderView);
	if (FAILED(res))
	{
		OutputDebugStringW(L"CreateRenderTargetView! Oh, that was unexpected!");
	}

}

void Game::CreateDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ID3D11DepthStencilState* DSState;
	WrlDevice->CreateDepthStencilState(&dsDesc, &DSState);

	DeviceContext->OMSetDepthStencilState( DSState, 1 );

	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = Display->ClientWidth;
	descDepth.Height = Display->ClientHeight;
	descDepth.MipLevels - 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* DepthStencil;
	WrlDevice->CreateTexture2D(&descDepth, nullptr, &DepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	WrlDevice->CreateDepthStencilView(
		DepthStencil, &descDSV, &DSView
	);
}

void Game::CreateSwapChain() {

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&WrlDevice,
		nullptr,
		&DeviceContext);
	if (FAILED(res))
	{
		OutputDebugStringW(L"D3D11CreateDeviceAndSwapChain! Oh, that was unexpected!");
	}
}

bool Game::CreateShadowsRT()
{
	pRenderTexture = new ShadowRenderTarget(this);
	if (!pRenderTexture->Init(1.0f, 100.0f))
		return false;
	pDepthShader = new DepthShader(this);
	if (!pDepthShader->Init())
		return false;
	pGRenderTarget = new GRenderTarget(this);
	if (!pGRenderTarget->Init())
		return false;
}

void Game::AddScore(bool IsPlayerOne) {
	if (IsPlayerOne) {
		OutputDebugStringW(L"Player 1 Scores!\n");
		p1Score++;
	}
	else {
		OutputDebugStringW(L"Player 2 Scores!\n");
		p2Score++;
	}
	OutputDebugStringW(L"Current score: \n");
	wchar_t buffer[256];
	wsprintf(buffer, L"%d : %d\n", p1Score, p2Score);
	OutputDebugStringW(buffer);
}