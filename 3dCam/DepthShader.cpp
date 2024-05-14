#include "DepthShader.h"
#include "Game.h"
#include "Buffer.h"

struct MatrixBufferType
{
	DirectX::XMMATRIX WVP;
};

DepthShader::DepthShader(Game* game)
{
	m_matrixBuffer = nullptr;
	pGame = game;
}

bool DepthShader::Init()
{
	// èíèöèàëèçèðóåì øåéäåð è âõîäíîé ôîðìàò
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


	pGame->WrlDevice->CreateVertexShader(
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


	pGame->WrlDevice->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};


	pGame->WrlDevice->CreateInputLayout(
		inputElements,
		1,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&pLayout);

	// ñîçäàåì êîíñòàíòíûé áóôåð
	//Buffer::CreateConstantBuffer(pGame->WrlDevice, 25, false);
	m_matrixBuffer = Buffer::CreateConstantBuffer(pGame->WrlDevice.Get(), sizeof(MatrixBufferType), false);

	return true;
}

void DepthShader::Render(int index, DirectX::XMMATRIX WVP)
{
	MatrixBufferType cb;

	cb.WVP = DirectX::XMMatrixTranspose(WVP);
	pGame->DeviceContext->UpdateSubresource(m_matrixBuffer, 0, NULL, &cb, 0, 0);
	pGame->DeviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	pGame->DeviceContext->IASetInputLayout(pLayout);
	pGame->DeviceContext->VSSetShader(vertexShader, NULL, 0);
	pGame->DeviceContext->PSSetShader(pixelShader, NULL, 0);

	pGame->DeviceContext->DrawIndexed(index, 0, 0);
}


void DepthShader::Close()
{
	/*_CLOSE(m_shader);
	_RELEASE(m_matrixBuffer);*/
}