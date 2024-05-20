#pragma once
#include "GameComponent.h"
class ScreenQuad :
    public GameComponent
{
public:
    ScreenQuad();
    void Draw(bool isDepth);
    LPCWSTR ShaderFile = L"./Shaders/FxQuad.hlsl";
private:
    virtual void Initialize();
    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode = nullptr;

    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode = nullptr;

    ID3D11RasterizerState* rastState;
};

