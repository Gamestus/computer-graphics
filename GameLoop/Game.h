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
#include <vector>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class TriangleComponent;

class Game
{
public:
	static Game* Instance;
	DisplayWin32* Display;
	InputDevice* Device;
	ID3D11RenderTargetView* RenderView;
	ID3D11Texture2D* BackBuffer;
	ID3D11DeviceContext* DeviceContext;
	std::vector<TriangleComponent> components;

	int DebugAnnotation;
	int Name;
	int PrevTime;
	int RenderSRV;
	int ScreenResized;
	int StartTime;
	int TotalTime;
	IDXGISwapChain* SwapChain;
	HRESULT SwapDevice;
	Microsoft::WRL::ComPtr<ID3D11Device> WrlDevice;

	Game(HINSTANCE hInstanceNew);
	~Game();

	int Run();

private:
	GameTimer timer;
	std::optional<int> processMessages();
	void DoFrame();
	void CreateBackBuffer();
	void CreateSwapChain();
	void CompileShaders();
	void CreateVertexIndexBuffers();

protected:
	void Initialize(HINSTANCE hInstanceNew);
};

