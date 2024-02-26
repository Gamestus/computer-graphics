#pragma once
#include "GameComponent.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class TriangleComponent :
    public GameComponent
{
public:
	TriangleComponent(DirectX::XMFLOAT4 points[6], LPCWSTR shader);
	void Initialize(LPCWSTR shader);
	void Draw();
	/*void DestroyResources();
	void Reload();
	void Update();*/
	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;


private:
	float* data;

	struct ConstData {
		Vector4 offset;
		Vector4 color;
	};

	ID3D11Buffer* constantBuffer;

	ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode = nullptr;

    ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode = nullptr;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

    DirectX::XMFLOAT4 points[6];
	ID3D11RasterizerState* rastState;

	void SetupConstBuffer();
};

