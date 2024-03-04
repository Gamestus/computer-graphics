#pragma once
#include "GameComponent.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <d3d11.h>

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class MeshComponent :
    public GameComponent
{
public:
	MeshComponent(const std::vector<DirectX::XMFLOAT4>& newPoints, LPCWSTR shader);
	~MeshComponent();
	void Initialize(LPCWSTR shader);
	void Draw();
	void SetColor(Vector4 nColor);

	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;


private:


	struct ConstData {
		Vector4 offset;
		Vector4 color;
	};

	ConstData data{ 
		Vector4(globalPosition.x * 1.0f, globalPosition.y * 1.0f, 0.5f, 0.0f),
		Vector4(0.9f, 0.6f, 0.6f, 1.0f) };

	ID3D11Buffer* constantBuffer;

	ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode = nullptr;

    ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode = nullptr;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

    std::vector<DirectX::XMFLOAT4> points;
	ID3D11RasterizerState* rastState;

	void SetupConstBuffer();
};

