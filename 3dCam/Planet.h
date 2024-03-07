#pragma once
#include "GameComponent3D.h"
class Planet :
    public GameComponent3D
{
public:
    Planet();

    Vector3 orbitCenter;
    virtual void Update(float delta);
    virtual void Initialize();
private:
    void RotateAroundPoint(float angle, Vector3 axis);
};

