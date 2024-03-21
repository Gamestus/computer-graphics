#pragma once
#include <vector>
#include <DirectXCollision.h>

class CollisionRect;

class PhysicsServer
{
public:
	static PhysicsServer* Instance;

	PhysicsServer();
	~PhysicsServer();
	void RegisterRect(CollisionRect* rect);
	void RegisterPoint(DirectX::XMVECTOR rect);
	void UpdatePhysics();
	DirectX::BoundingSphere* player;
private:
	std::vector<CollisionRect*> rects;
	std::vector<DirectX::XMVECTOR> points;
};

