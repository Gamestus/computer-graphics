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

void PhysicsServer::RegisterPoint(DirectX::XMVECTOR point)
{
    points.push_back(point);
}

void PhysicsServer::UpdatePhysics()
{
    for (size_t i = 0; i < points.size(); ++i) {
        if (player->Contains(points[i]))
        {
            OutputDebugStringW(L"buffer\n");
        }
    }
}
