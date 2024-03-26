#include "PhysicsServer.h"
#include "SimpleMath.h"
#include "CollisionRect.h"
#include "AssimpMesh.h"
#include "CatamariPlayer.h"
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

void PhysicsServer::RegisterMesh(AssimpMesh* mesh)
{
    meshes.push_back(mesh);
}

void PhysicsServer::UpdatePhysics()
{
    for (size_t i = 0; i < meshes.size(); ++i) {
        if (meshes[i]->IsCollision && player->Contains(meshes[i]->GetGlobalPosition()))
        {
            CatamariPlayer->Eat(meshes[i]);
        }
    }
}
