#pragma once
#include "GameComponent3D.h"
#include "Camera.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <d3d11.h>

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class MeshComponent :
    public GameComponent3D
{
public:
	MeshComponent(const std::vector<DirectX::XMFLOAT4>& newPoints, const std::vector<int>& indices);
	~MeshComponent();
	void Initialize(const std::vector<int>& indices);
	void Draw();
	void SetColor(Vector4 nColor);

	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;
	LPCWSTR ShaderFile = L"./Shaders/ShaderConstBuf.hlsl";

	Camera* camera;

	dx::XMMATRIX GetTransform();
private:


	struct ConstData {
		dx::XMMATRIX transform;
		Vector4 color;
	};

	ConstData data{ 
		dx::XMMatrixTranspose(
			GetTransform() *
				dx::XMMatrixPerspectiveLH(1.0f, 1.0f ,0.1f, 10.0f)
		),
		//Vector4(globalPosition.x, globalPosition.y, globalPosition.z, 0.0f),
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

