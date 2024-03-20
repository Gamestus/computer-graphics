#pragma once
#include "MeshComponent.h"
class AssimpMesh :
    public MeshComponent
{
public:
    AssimpMesh(std::string filepath);
    AssimpMesh(std::string filepath, LPCWSTR texpath);
    AssimpMesh(std::string filepath, LPCWSTR texpath, float importScale);
};

