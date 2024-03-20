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
private:
    AssimpMesh* centralMesh;
};

