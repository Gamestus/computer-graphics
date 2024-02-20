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
	/*void DestroyResources();
	void Draw();
	void Reload();
	void Update();*/

private:
	ID3D11InputLayout* layout;
    DirectX::XMFLOAT4 points[8];
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
	ID3D11RasterizerState* rastState;
};

