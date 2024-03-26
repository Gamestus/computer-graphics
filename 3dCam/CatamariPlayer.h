#pragma once
#include "GameComponent3D.h"
#include "AssimpMesh.h"
#include "Camera.h"

class CatamariPlayer :
    public GameComponent3D
{
public:
    Camera* Cam;
    CatamariPlayer();
    void Initialize();
    void Update(float delta);
    float size = 1.0f;
    void Eat(AssimpMesh* mesh);
private:
    AssimpMesh* centralMesh;
    float moveSpeed = 2.0f;
    dx::BoundingSphere collision{};
};

