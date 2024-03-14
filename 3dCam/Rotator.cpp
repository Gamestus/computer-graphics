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
    auto localTransform = GetLocalTransform();

    localTransform *= XMMatrixRotationQuaternion(rotationQuaternion);


    dx::SimpleMath::Matrix matrix(localTransform);
   
    rotation = matrix.ToEuler();
}
