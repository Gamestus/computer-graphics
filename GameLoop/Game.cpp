#include "Game.h"

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
	CompileShaders();
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
	}

}

void Game::CreateBackBuffer() {
	SwapDevice = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	SwapDevice = WrlDevice->CreateRenderTargetView(BackBuffer, nullptr, &RenderView);
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


	SwapDevice = D3D11CreateDeviceAndSwapChain(
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
}

void Game::CompileShaders() {
	ID3DBlob* vertexBC = nullptr;
	ID3DBlob* errorVertexCode = nullptr;
	SwapDevice = D3DCompileFromFile(L"./Shaders/Shader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);
	
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* pixelBC;
	ID3DBlob* errorPixelCode;
	SwapDevice = D3DCompileFromFile(L"./Shaders/Shader.hlsl",
		Shader_Macros /*macros*/, nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelBC,
		&errorPixelCode);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	WrlDevice->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	WrlDevice->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};
}