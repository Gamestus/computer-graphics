#include "PongBall.h"
#include "TriangleComponent.h"
#include "CollisionRect.h"
#include "Keys.h"


PongBall::PongBall() {
	Initialize();
}


void PongBall::Initialize() {
	Velocity = Vector2(-speed, 0.0);

	DirectX::XMFLOAT4 points1[8] = {
	DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f / 4, 1.0f),
	DirectX::XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),
	DirectX::XMFLOAT4(0.5f / 4, -0.5f / 4, 0.5f / 4, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.0f, -0.5f / 4, 0.5f / 4, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT4(0.5f / 4, 0.0f, 0.5f / 4, 1.0f),
	DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f),
	};
	std::vector<DirectX::XMFLOAT4> vector(std::begin(points1), std::end(points1));
	children.push_back(std::make_unique<TriangleComponent>(vector, L"./Shaders/ShaderConstBuf.hlsl"));
	
	children.push_back(std::make_unique<CollisionRect>([this](CollisionRect* otherRect) {
		OnColliderEntered(otherRect);
		}));

}

#define NODGI

void PongBall::Update(float delta) {
	Vector2 pos = GetGlobalPosition() + delta * Velocity;
	float minVelocity = -0.4f;
	float maxVelocity = 0.4f;
	if (pos.x >= 1.0f) {
		SetGlobalPosition(Vector2(0, 0));


		float randomVelocity = minVelocity + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxVelocity - minVelocity)));
		Velocity.y = randomVelocity;

		Velocity.x = -speed;
		Game::Instance->AddScore(true);
	}
	else if (pos.x <= -1.12f) {
		SetGlobalPosition(Vector2(0, 0));

		float randomVelocity = minVelocity + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxVelocity - minVelocity)));
		Velocity.y = randomVelocity;

		Velocity.x = speed;
		Game::Instance->AddScore(false);
	}
	else {
		pos.Clamp(Vector2(-1.12f, -1.0f), Vector2(1.0f, 1.0f));
		SetGlobalPosition(pos);
	}

}

void PongBall::OnColliderEntered(CollisionRect* rect)
{
	if (rect->IsHorizontal) {
		Velocity.x = -Velocity.x;
		Vector2 pos = GetGlobalPosition() + 0.02 * Velocity;
		SetGlobalPosition(pos);
		Velocity *= 1.1;
	}
	else {
		Velocity.y = -Velocity.y;
	}
	
}
