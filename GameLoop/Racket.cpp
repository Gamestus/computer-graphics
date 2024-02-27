#include "Racket.h"
#include "TriangleComponent.h"
#include "Keys.h"


Racket::Racket() {
	Initialize();
}


void Racket::Initialize() {
	DirectX::XMFLOAT4 points1[6] = {
	DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.0f, -0.5f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	};
	children.push_back(std::make_unique<TriangleComponent>(points1, L"./Shaders/ShaderConstBuf.hlsl"));

}

#define NODGI

void Racket::Update(float delta) {
	Vector2 input;

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::W))) {
		input.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::S))) {
		input.y = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::D))) {
		input.x = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::A))) {
		input.x = -1;
	}
	Vector2 pos = GetGlobalPosition() + speed * delta * input ;
	pos.Clamp(Vector2(-1.0f, -0.5f), Vector2(0.5f, 1.0f));
	SetGlobalPosition(pos);	
}