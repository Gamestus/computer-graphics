#include "Racket.h"
#include "TriangleComponent.h"

Racket::Racket() {
	Initialize();
}


void Racket::Initialize()
{
	DirectX::XMFLOAT4 points1[6] = {
	DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.7f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(-0.5f, 0.7f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	};
	children.push_back(std::make_unique<TriangleComponent>(points1, L"./Shaders/ShaderConstBuf.hlsl"));

}