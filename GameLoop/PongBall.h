#pragma once
#include "GameComponent.h"
class PongBall :
    public GameComponent
{
public:
    float speed = 0.0f;

    Vector2 Velocity;

    PongBall();
    void Update(float delta);
    void OnColliderEntered();
private:
    void Initialize();
};

