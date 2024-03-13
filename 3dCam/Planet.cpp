#include "Planet.h"
#include "MeshComponent.h"
#include "Sphere.h"

Planet::Planet()
{
    Initialize();
}

Planet::Planet(float size, Vector4 color)
{
    PlanetColor = color;
    PlanetSize = size;
    Initialize();
}

void Planet::Update(float delta)
{
}

void Planet::Initialize()
{
    AddChild<MeshComponent>(std::make_unique<MeshComponent>(
        Sphere::GenerateSphereVertices(PlanetSize, 12, 12),
        Sphere::GenerateSphereIndices(12, 12)
    ));
    auto mesh = GetChild<MeshComponent>(0);
    mesh->SetColor(PlanetColor);
}
