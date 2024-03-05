#pragma once
#include "GameComponent3D.h"
class Camera :
    public GameComponent3D
{
public:
    Camera();

    dx::XMMATRIX GetMatrix();
private:
    float distance = 20.0f;
    float orbitTheta = 0.0f;
    float orbitPhi = 0.0f;
};

