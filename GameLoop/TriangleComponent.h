#pragma once
#include "GameComponent.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <d3d11.h>

class TriangleComponent :
    public GameComponent
{
public:
	TriangleComponent();
	void Initialize();
	void Draw();
	/*void DestroyResources();
	void Reload();
	void Update();*/
	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;

private:
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	ID3DBlob* vertexBC = nullptr;
	ID3DBlob* pixelBC;
	ID3D11InputLayout* layout;
    DirectX::XMFLOAT4 points[8];
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
	ID3D11RasterizerState* rastState;
};

