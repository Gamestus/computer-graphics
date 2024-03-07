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

Planet::Planet(float size, Vector3 center, Vector4 color)
{
    PlanetColor = color;
    orbitCenter = center;
    PlanetSize = size;
    Initialize();
}

void Planet::Update(float delta)
{
    RotateAroundPoint(Speed * delta, RotAxis);
}

void Planet::Initialize()
{
    AddChild(std::make_unique<MeshComponent>(
        Sphere::GenerateSphereVertices(PlanetSize, 12, 12),
        Sphere::GenerateSphereIndices(12, 12)
    ));
    auto mesh = GetChild<MeshComponent>(0);
    mesh->SetColor(PlanetColor);
    SetGlobalPosition(Vector3(0,0,9));
}
