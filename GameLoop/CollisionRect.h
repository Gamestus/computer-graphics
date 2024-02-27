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


    void OnCollision() {
        if (callback_) { callback_(); }
    }

    SimpleMath::Rectangle rect2D;

    CollisionRect(CollisionCallback callback) : callback_(callback) { Initialize(); };
    CollisionRect();
    const DirectX::SimpleMath::Rectangle& GetRect() const;
private:
    CollisionCallback callback_;
    void Initialize();
};

