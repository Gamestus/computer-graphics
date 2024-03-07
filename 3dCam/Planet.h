#pragma once
#include "GameComponent3D.h"
class Planet :
    public GameComponent3D
{
public:
    Planet();
    Planet(float size, Vector3 center, Vector4 color);

    Vector3 orbitCenter;
    float PlanetSize = 1.0;
    Vector4 PlanetColor = Vector4(1, 1, 1, 1);
    Vector3 RotAxis = Vector3(0, 1, 0);
    float Speed = 2.0;
    virtual void Update(float delta);
    virtual void Initialize();
private:
    void RotateAroundPoint(float angle, Vector3 axis);
};

