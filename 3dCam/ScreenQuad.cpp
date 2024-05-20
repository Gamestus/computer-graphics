#include "ScreenQuad.h"
#include "ShadowRenderTarget.h"

ScreenQuad::ScreenQuad()
{
	Initialize();
}

void ScreenQuad::Initialize()
{
	game = Game::Instance;

	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(ShaderFile,
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);


	game->WrlDevice->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(ShaderFile,
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);


	game->WrlDevice->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);


	//rasterizer

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	res = game->WrlDevice->CreateRasterizerState(&rastDesc, &rastState);

}

void ScreenQuad::Draw(bool isDepth)
{

	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };


	ID3D11ShaderResourceView* DTexture = game->pRenderTexture->GetShaderResourceView();
	game->DeviceContext->PSSetShaderResources(0, 1, &DTexture);

	game->DeviceContext->RSSetState(rastState);
	//setup IA
	game->DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//set shaders
	game->DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	game->DeviceContext->PSSetShader(pixelShader, nullptr, 0);


	game->DeviceContext->Draw(8, 0);



}


