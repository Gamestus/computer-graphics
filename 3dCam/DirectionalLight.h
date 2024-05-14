#pragma once
#include "GameComponent3D.h"

class DirectionalLight :
    public GameComponent3D
{
public:
    DirectionalLight();
    DirectX::XMMATRIX GetViewMatrix();

private:
    DirectX::XMVECTOR forwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
};

