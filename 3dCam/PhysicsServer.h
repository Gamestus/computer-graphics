#pragma once
#include <vector>
#include <DirectXCollision.h>

class CatamariPlayer;
class AssimpMesh;
class CollisionRect;

class PhysicsServer
{
public:
	static PhysicsServer* Instance;

	PhysicsServer();
	~PhysicsServer();
	void RegisterRect(CollisionRect* rect);
	void RegisterMesh(AssimpMesh* mesh);
	void UpdatePhysics();
	DirectX::BoundingSphere* player;
	CatamariPlayer* CatamariPlayer;
private:
	std::vector<CollisionRect*> rects;
	std::vector<AssimpMesh*> meshes;
};

