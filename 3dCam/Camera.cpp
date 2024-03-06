#include "Camera.h"
#include "Keys.h"

Camera::Camera()
{
}

DirectX::XMMATRIX Camera::GetMatrix()
{
	DirectX::XMMATRIX matrix;
	if (isOrbit) {
		auto pos = dx::XMVector3Transform(
			dx::XMVectorSet(0, 0, -distance, 0.0f),
			dx::XMMatrixRotationRollPitchYaw(orbitPhi, -orbitTheta, 0.0f)
		);
		matrix = dx::XMMatrixLookAtLH(
			pos,
			dx::XMVectorZero(),
			dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		) * dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	}
	else {
		matrix = dx::XMMatrixTranslation(globalPosition.x, globalPosition.y, globalPosition.z);
	}

    return matrix * dx::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.4f, 10.0f);
}

#define NODGI

void Camera::Update(float delta) {
	Vector2 input;

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Down))) {
		input.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Up))) {
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
	if (!isOrbit)
	{
		rotation.x = -clampedValue;
		rotation.y += input.x * delta * 2;
	}


	Vector3 moveInput;
	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Q))) {
		moveInput.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::E))) {
		moveInput.y = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::S))) {
		moveInput.z = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::W))) {
		moveInput.z = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::A))) {
		moveInput.x = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::D))) {
		moveInput.x = -1;
	}

	globalPosition += moveInput * delta * 2;
	clampedValue = distance + moveInput.z * delta * 2;
	if (clampedValue < 0.2)
		clampedValue = 0.2;
	else if (clampedValue > 12.5f)
		clampedValue = 12.5f;
	distance = clampedValue;

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Tab))) {
		rotation = Vector3(0, 0, 0);
		globalPosition = Vector3(0, 0, distance);
		distance = 5.0f;
		isOrbit = !isOrbit;
	}
}