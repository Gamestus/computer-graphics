#pragma once
#include "GameComponent.h"
#include "PhysicsServer.h"
#include <SimpleMath.h>

using namespace DirectX;

class CollisionRect :
    public GameComponent
{
public:
    using CollisionCallback = std::function<void()>;

    SimpleMath::Rectangle rect2D;

    void OnCollision() {
        if (callback_) { callback_(); }
    }

    CollisionRect();
    CollisionRect(Vector2 size);
    CollisionRect(CollisionCallback callback);

    void Update(float delta);
private:
    CollisionCallback callback_;
    void Initialize();
};

