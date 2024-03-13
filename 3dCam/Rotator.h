#pragma once
#include "GameComponent3D.h"
class Rotator :
    public GameComponent3D
{
public:

    Vector3 RotAxis = Vector3(0, 1, 0);
    float Speed = 2.0;
    virtual void Update(float delta);
    virtual void Initialize();
private:
    void Rotate(float angle, Vector3 axis);
};

