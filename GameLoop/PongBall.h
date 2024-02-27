#pragma once
#include "GameComponent.h"
class PongBall :
    public GameComponent
{
public:
    float speed = 0.35f;

    Vector2 Velocity;

    PongBall();
    void Update(float delta);
    void OnColliderEntered();
private:
    void Initialize();
};

