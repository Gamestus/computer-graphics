#pragma once
#include "GameComponent.h"
class Racket :
    public GameComponent
{
public:
    float speed = 0.55f;

    Racket();
    void Update(float delta);
private:
    void Initialize();
};

