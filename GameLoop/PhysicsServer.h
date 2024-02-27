#pragma once
#include <vector>

class CollisionRect;

class PhysicsServer
{
public:
	static PhysicsServer* Instance;

	PhysicsServer();
	~PhysicsServer();
	void RegisterRect(CollisionRect* rect);
	void UpdatePhysics();
private:
	std::vector<CollisionRect*> rects;
};

