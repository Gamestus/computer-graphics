#pragma once
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "GameTimer.h"
#include "PhysicsServer.h"
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

class GameComponent;
class Camera;

class Game
{
public:
	static Game* Instance;
	DisplayWin32* Display;
	InputDevice* InDevice;
	ID3D11RenderTargetView* RenderView;
	ID3D11Texture2D* BackBuffer;
	ID3D11DeviceContext* DeviceContext;
	GameComponent* RootComponent;
	IDXGISwapChain* SwapChain;
	HRESULT SwapDevice;
	Microsoft::WRL::ComPtr<ID3D11Device> WrlDevice;
	ID3D11DepthStencilView* DSView;

	Camera* CurrCam;
	PhysicsServer* PhysServer;

	int p1Score = 0;
	int p2Score = 0;

	Game(HINSTANCE hInstanceNew);
	~Game();

	int Run();
	void AddScore(bool IsPlayerOne);
private:
	D3D11_VIEWPORT viewport = {};
	GameTimer timer;
	std::optional<int> processMessages();

	void Update(float delta);
	void CreateBackBuffer();
	void CreateDepthStencilBuffer();
	void CreateSwapChain();
	void CreateVertexIndexBuffers();
	void UpdateViewport();
	void Draw();
protected:
	void Initialize(HINSTANCE hInstanceNew);
};

