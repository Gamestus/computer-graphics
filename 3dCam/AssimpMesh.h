#pragma once
#include "MeshComponent.h"
class AssimpMesh :
    public MeshComponent
{
public:
    std::string MeshFile;
    bool IsCollision;
    AssimpMesh(std::string filepath);
    AssimpMesh(std::string filepath, LPCWSTR texpath);
    AssimpMesh(std::string filepath, LPCWSTR texpath, float importScale, bool isCollision = false);
};

