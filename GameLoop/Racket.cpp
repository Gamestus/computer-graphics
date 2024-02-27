#include "Racket.h"
#include "TriangleComponent.h"
#include "CollisionRect.h"
#include "Keys.h"


Racket::Racket(bool isPlayerOne) {
	IsPlayerOne = isPlayerOne;
	Initialize();
}


void Racket::Initialize() {
	DirectX::XMFLOAT4 points1[8] = {
	DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f / 4, -0.5f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.0f, -0.5f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f / 4, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	};
	std::vector<DirectX::XMFLOAT4> vector(std::begin(points1), std::end(points1));
	children.push_back(std::make_unique<TriangleComponent>(vector, L"./Shaders/ShaderConstBuf.hlsl"));
	children.push_back(std::make_unique<CollisionRect>(Vector2(50, 200), true));
}

#define NODGI

void Racket::Update(float delta) {
	Vector2 input;

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(IsPlayerOne ? Keys::W : Keys::Up))) {
		input.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(IsPlayerOne ? Keys::S : Keys::Down))) {
		input.y = -1;
	}

	//if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(IsPlayerOne ? Keys::D : Keys::Left))) {
	//	input.x = 1;
	//}
	//else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(IsPlayerOne ? Keys::A : Keys::Right))) {
	//	input.x = -1;
	//}
	Vector2 pos = GetGlobalPosition() + speed * delta * input ;
	pos.Clamp(Vector2(-1.0f, -0.5f), Vector2(0.85f, 1.0f));
	SetGlobalPosition(pos);	
}