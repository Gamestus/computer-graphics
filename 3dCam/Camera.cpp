#include "Camera.h"
#include "Keys.h"

Camera::Camera()
{
	globalPosition.z = distance;
}

DirectX::XMMATRIX Camera::GetMatrix()
{
    auto pos = dx::XMVector3Transform(
        dx::XMVectorSet(globalPosition.x, globalPosition.y, globalPosition.z, 0.0f),
        dx::XMMatrixRotationRollPitchYaw(orbitPhi, -orbitTheta, 0.0f)
    );
    auto matrix = dx::XMMatrixLookAtLH(
        pos,
        dx::XMVectorZero(),
        dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ) * dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    return matrix * dx::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.4f, 10.0f);
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

	float clampedValue = orbitPhi + input.y * delta * 2;
	if (clampedValue < -1.5f)
		clampedValue = -1.5f;
	else if (clampedValue > 1.5f)
		clampedValue = 1.5f;

	orbitPhi = clampedValue;
	orbitTheta -= input.x * delta * 2;

	Vector3 moveInput;
	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::E))) {
		moveInput.z = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Q))) {
		moveInput.z = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::W))) {
		moveInput.x = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::S))) {
		moveInput.x = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::A))) {
		moveInput.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::D))) {
		moveInput.y = -1;
	}

	rotation += moveInput * delta * 2;
}