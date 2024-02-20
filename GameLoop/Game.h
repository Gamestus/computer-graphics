#pragma once
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "GameTimer.h"
#include <optional>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Game
{
public:
	static Game* Instance;
	DisplayWin32* Display;
	InputDevice* Device;
	int backBuffer;
	int Context;
	int DebugAnnotation;
	int Name;
	int PrevTime;
	int RenderSRV;
	int RenderView;
	int ScreenResized;
	int StartTime;
	int SwapChain;
	int TotalTime;

	Game(HINSTANCE hInstanceNew);
	~Game();

	int Run();

private:
	GameTimer timer;
	std::optional<int> processMessages();
	void DoFrame();
	void CreateBackBuffer();
	void CreateSwapChain();

	Microsoft::WRL::ComPtr<ID3D11Device> wrlDevice;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	HRESULT swapDevice;
protected:
	void Initialize(HINSTANCE hInstanceNew);
};

