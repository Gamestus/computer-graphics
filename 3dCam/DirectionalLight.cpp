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

	float fieldOfView = (float)dx::XM_PI / 2.0f;
	float screenAspect = 1.0f;
	matrix *= dx::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 1.0f, 100.0f);

	return matrix;
}
