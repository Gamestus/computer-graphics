#pragma once
#include "MeshComponent.h"
class FlexingCube :
    public MeshComponent
{
public:
    FlexingCube();

    virtual void Update(float delta);
};

