#pragma once
#include "MeshComponent.h"
class AssimpMesh :
    public MeshComponent
{
public:
    AssimpMesh(std::string filepath);
    AssimpMesh(std::string filepath, float importScale);
};

