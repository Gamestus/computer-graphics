#pragma once
#include "GameComponent.h"
class Racket :
    public GameComponent
{
public:
    float speed = 0.95f;

    bool IsPlayerOne = true;

    Racket(bool isPlayerOne);
    void Update(float delta);
private:
    void Initialize();
};

