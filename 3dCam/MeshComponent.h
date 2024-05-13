#pragma once
#include "GameComponent3D.h"
#include "Camera.h"
#include <d3dcompiler.h>
#include <directxmath.h>
#include <d3d11.h>

#include "SimpleMath.h"
#include "Vertex.h"

using namespace DirectX::SimpleMath;

class MeshComponent :
    public GameComponent3D
{
public:
	MeshComponent();
	MeshComponent(const std::vector<DirectX::XMFLOAT4>& newPoints, const std::vector<int>& nIndices);
	MeshComponent(const std::vector<DirectX::XMFLOAT4>& newPoints, const std::vector<int>& nIndices, LPCWSTR Shader);
	~MeshComponent();
	void Initialize(const std::vector<int>& nIndices);
	void Draw();

	std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;
	LPCWSTR ShaderFile = L"./Shaders/ShaderConstBuf.hlsl";
	LPCWSTR TextureFile = L"textures\\obama_c.jpg";
	Camera** camera;

protected:
	ID3D11ShaderResourceView* Texture;

	struct ConstData {
		dx::XMMATRIX projectionMatrix;
		dx::XMMATRIX globalTransform;
		dx::XMVECTOR cameraPosition;
		dx::XMMATRIX viewMatrix;
		dx::XMMATRIX inverseTransform;
	};

	ConstData data;

protected:
	std::vector<Vertex> points;
	std::vector<int> indices;
private:
	ID3D11Buffer* constantBuffer;

	ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode = nullptr;

    ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode = nullptr;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11RasterizerState* rastState;

	ID3D11SamplerState* pSamplerState;

	void SetupConstBuffer();
};

