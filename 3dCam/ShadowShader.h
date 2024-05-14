#pragma once

#include <optional>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Light.h"
#include "Game.h"

using namespace DirectX;

class pGame;

class ShadowShader
{

public:
	ShadowShader(Game* game);

	bool Init();
	void Close();
	void Render(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX WVP, CXMMATRIX WVPlight, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, Light& light);

private:
	Game* m_render;

	LPCWSTR ShaderFile = L"./Shaders/Shadow.hlsl";

	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode = nullptr;

	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode = nullptr;

	ID3D11InputLayout* pLayout;

	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_lightBuffer2;
};