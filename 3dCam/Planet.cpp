#include "Planet.h"
#include "MeshComponent.h"
#include "Sphere.h"

void Planet::RotateAroundPoint(float angle, Vector3 axis)
{
    dx::XMMATRIX translationToOrigin = dx::XMMatrixTranslation(-orbitCenter.x, -orbitCenter.y, -orbitCenter.z);

    dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationAxis(dx::XMLoadFloat3(&axis), angle);

    dx::XMMATRIX translationFromOrigin = dx::XMMatrixTranslation(orbitCenter.x, orbitCenter.y, orbitCenter.z);

    dx::XMFLOAT3 currentGlobalPosition = GetGlobalPosition();
    dx::XMVECTOR currentPosition = dx::XMLoadFloat3(&currentGlobalPosition);

    dx::XMMATRIX transformationMatrix = translationToOrigin * rotationMatrix * translationFromOrigin;
    currentPosition = dx::XMVector3TransformCoord(currentPosition, transformationMatrix);
    SetGlobalPosition(currentPosition);
}

Planet::Planet()
{
    Initialize();
}

void Planet::Update(float delta)
{
    RotateAroundPoint(2.0 * delta, Vector3(0, 1, 0));
}

void Planet::Initialize()
{
    AddChild(std::make_unique<MeshComponent>(
        Sphere::GenerateSphereVertices(0.5, 12, 12),
        Sphere::GenerateSphereIndices(12, 12)
    ));
    auto mesh = GetChild<MeshComponent>(0);
    mesh->SetColor(Vector4(0.3, 0.6, 0.5, 1));
    SetGlobalPosition(Vector3(0,0,9));
}
