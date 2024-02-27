#include "PhysicsServer.h"
#include "SimpleMath.h"
#include "CollisionRect.h"

using namespace DirectX;

PhysicsServer* PhysicsServer::Instance = nullptr;

PhysicsServer::PhysicsServer()
{
	PhysicsServer::Instance = this;
}

PhysicsServer::~PhysicsServer()
{
	PhysicsServer::Instance = nullptr;
}

void PhysicsServer::RegisterRect(CollisionRect* rect)
{
	rects.push_back(rect);
}

void PhysicsServer::UpdatePhysics()
{
    for (size_t i = 0; i < rects.size(); ++i) {
        for (size_t j = i + 1; j < rects.size(); ++j) {
            if (rects[i]->rect2D.Intersects(rects[j]->rect2D)) {
                rects[i]->OnCollision();
                rects[j]->OnCollision();
            }
        }
    }
}
