#include "Camera.h"
#include "Keys.h"

Camera::Camera()
{
}

DirectX::XMMATRIX Camera::GetMatrix()
{
    auto pos = dx::XMVector3Transform(
        dx::XMVectorSet(0.0f, 0.0f, -distance, 0.0f),
        dx::XMMatrixRotationRollPitchYaw(orbitPhi, -orbitTheta, 0.0f)
    );
    auto matrix = dx::XMMatrixLookAtLH(
        pos,
        dx::XMVectorZero(),
        dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ) * dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    return matrix * dx::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.1f, 10.0f);
}

#define NODGI

void Camera::Update(float delta) {
	Vector2 input;

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Up))) {
		input.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Down))) {
		input.y = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Left))) {
		input.x = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Right))) {
		input.x = -1;
	}
	orbitPhi += input.y * delta * 2;
	orbitTheta += input.x * delta * 2;
}