#include "Game.h"
#include "TriangleComponent.h"

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

		Game::Instance->Device->AddPressedKey(static_cast<unsigned int>(wparam));
		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		break;
	}
	case WM_KEYUP:
	{
		Game::Instance->Device->RemovePressedKey(static_cast<unsigned int>(wparam));
		break;
	}
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		Game::Instance->Device->MouseMove(pt);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		Game::Instance->Device->MouseLeftClick(pt);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		Game::Instance->Device->MouseRightClick(pt);
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
	Device = new InputDevice();
	Display = new DisplayWin32(hInstanceNew, MessageHandler);
	CreateSwapChain();
	CreateBackBuffer();

	components.push_back(TriangleComponent());
}

Game::~Game() {
	delete[] Display;
	delete[] Device;
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
		components.at(0).Draw();
	}

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