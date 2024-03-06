#include "Camera.h"
#include "Keys.h"

Camera::Camera()
{
}

DirectX::XMMATRIX Camera::GetMatrix()
{
	dx::XMMATRIX matrix;
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
		dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		dx::XMVECTOR rotatedForwardVector = dx::XMVector3Transform(forwardVector, rotationMatrix);
		matrix = dx::XMMatrixLookToLH(
			dx::XMLoadFloat3(&globalPosition),
			rotatedForwardVector,
			dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
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

	float clampedValue = orbitPhi + input.y * delta * moveSpeed;
	if (clampedValue < -1.5f)
		clampedValue = -1.5f;
	else if (clampedValue > 1.5f)
		clampedValue = 1.5f;

	orbitPhi = clampedValue;
	orbitTheta -= input.x * delta * moveSpeed;
	if (!isOrbit)
	{
		rotation.x = clampedValue;
		rotation.y -= input.x * delta * moveSpeed;
	}


	Vector3 moveInput;
	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::E))) {
		moveInput.y = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::Q))) {
		moveInput.y = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::W))) {
		moveInput.z = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::S))) {
		moveInput.z = -1;
	}

	if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::D))) {
		moveInput.x = 1;
	}
	else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::A))) {
		moveInput.x = -1;
	}
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMVECTOR moveVector = DirectX::XMVectorSet(moveInput.x, moveInput.y, moveInput.z, 0.0f);

	moveVector = DirectX::XMVector3Transform(moveVector, rotationMatrix);
	moveVector = DirectX::XMVector3Normalize(moveVector);

	moveVector = DirectX::XMVectorScale(moveVector, moveSpeed * delta);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, moveVector);

	globalPosition += move;

	clampedValue = distance - moveInput.z * delta * moveSpeed;
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