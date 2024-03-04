#pragma once
#include "GameComponent.h"

class TriangleComponent;

class PongBall :
    public GameComponent
{
public:
    float speed = 0.5f;

    Vector2 Velocity;

    PongBall();
    void Update(float delta);
    void OnColliderEntered(CollisionRect* rect);
private:
    void Initialize();
    TriangleComponent* triangleComponent;
    float invTimer = 0;
    float invMax = 0.25;
};

