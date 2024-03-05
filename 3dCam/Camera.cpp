#include "Camera.h"

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
    return matrix;
}
