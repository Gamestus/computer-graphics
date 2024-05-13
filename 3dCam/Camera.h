#pragma once
#include "GameComponent3D.h"
class Camera :
    public GameComponent3D
{
public:
    Camera();

    dx::XMMATRIX GetMatrix();
    dx::XMMATRIX GetViewMatrix();
    float GetTheta();
    dx::XMVECTOR GetCameraPosition();
	void Update(float delta);
    float moveSpeed = 2.0f;
    float distance = 5.0f;
private:
    bool isOrbit = true;
    bool isOrthographic = true;
    float orbitTheta = 0.0f;
    float orbitPhi = 0.0f;
    dx::XMVECTOR forwardVector = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
};
