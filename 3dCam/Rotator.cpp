#include "Rotator.h"

void Rotator::Update(float delta)
{
	Rotate(Speed * delta, RotAxis);
}

void Rotator::Initialize()
{
}

void Rotator::Rotate(float angle, Vector3 axis)
{
    axis.Normalize();

    dx::SimpleMath::Quaternion rotationQuaternion = dx::XMQuaternionRotationNormal(axis, angle);

    dx::SimpleMath::Quaternion currentRotation(rotation.x, rotation.y, rotation.z, 1.0f);
    dx::SimpleMath::Quaternion rotatedQuaternion = dx::XMQuaternionMultiply(currentRotation, rotationQuaternion);

    rotation.x = rotatedQuaternion.x;
    rotation.y = rotatedQuaternion.y;
    rotation.z = rotatedQuaternion.z;
}
