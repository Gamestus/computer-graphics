#pragma once
#include "GameComponent3D.h"
class Camera :
    public GameComponent3D
{
public:
    Camera();

    dx::XMMATRIX GetMatrix();
	void Update(float delta);
private:
    bool isOrbit = true;
    float distance = 5.0f;
    float orbitTheta = 0.0f;
    float orbitPhi = 0.0f;
};
