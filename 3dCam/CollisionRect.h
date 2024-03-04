#pragma once
#include "GameComponent.h"
#include "PhysicsServer.h"
#include <SimpleMath.h>

using namespace DirectX;

class CollisionRect :
    public GameComponent
{
public:
    using CollisionCallback = std::function<void(CollisionRect* rect)>;

    bool IsHorizontal = false;

    SimpleMath::Rectangle rect2D;

    void OnCollision(CollisionRect* rect) {
        if (callback_) { callback_(rect); }
    }

    CollisionRect();
    CollisionRect(Vector2 size);
    CollisionRect(Vector2 size, bool isHorizontal);
    CollisionRect(CollisionCallback callback);

    void Update(float delta);
private:
    CollisionCallback callback_;
    void Initialize();
};

