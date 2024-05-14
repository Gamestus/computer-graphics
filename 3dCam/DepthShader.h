#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

class Game;

class DepthShader
{
public:
	DepthShader(Game* game);

	bool Init();
	void Close();
	void Render(int index, DirectX::XMMATRIX WVP);

private:
	Game* pGame;

	LPCWSTR ShaderFile = L"./Shaders/Depth.hlsl";

	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode = nullptr;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode = nullptr;

	ID3D11InputLayout* pLayout;
	ID3D11Buffer* m_matrixBuffer;
};