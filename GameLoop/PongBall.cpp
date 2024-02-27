#include "PongBall.h"
#include "TriangleComponent.h"
#include "CollisionRect.h"
#include "Keys.h"


PongBall::PongBall() {
	Initialize();
}


void PongBall::Initialize() {
	Velocity = Vector2(-speed, 0.3);

	DirectX::XMFLOAT4 points1[6] = {
	DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.0f, -0.5f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	};
	std::vector<DirectX::XMFLOAT4> vector(std::begin(points1), std::end(points1));
	children.push_back(std::make_unique<TriangleComponent>(vector, L"./Shaders/ShaderConstBuf.hlsl"));
	children.push_back(std::make_unique<CollisionRect>([this]() {
		OnColliderEntered();
	}));

}

#define NODGI

void PongBall::Update(float delta) {
	Vector2 pos = GetGlobalPosition() + delta * Velocity;
	pos.Clamp(Vector2(-1.0f, -0.5f), Vector2(0.5f, 1.0f));
	SetGlobalPosition(pos);
}

void PongBall::OnColliderEntered()
{
	OutputDebugStringW(L"Collision!\n");
	Velocity = -Velocity;
}
