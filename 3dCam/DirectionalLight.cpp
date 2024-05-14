#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	rotation.x = 0.8;
	SetLocalPosition(Vector3(0, 6, -5));
}

dx::XMMATRIX DirectionalLight::GetViewMatrix()
{
	dx::XMMATRIX matrix;
	dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	dx::XMVECTOR rotatedForwardVector = dx::XMVector3Transform(forwardVector, rotationMatrix);
	matrix = dx::XMMatrixLookToLH(
		GetGlobalPosition(),
		rotatedForwardVector,
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));


	return matrix * DirectX::XMMatrixOrthographicLH(10.0f, 10.0f, 0.4f, 60.0f);
}
