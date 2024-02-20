#include "TriangleComponent.h"
#include <chrono>

TriangleComponent::TriangleComponent(DirectX::XMFLOAT4 newPoints[6]) {

	points[0] = newPoints[0];
	points[1] = newPoints[1];
	points[2] = newPoints[2];
	points[3] = newPoints[3];
	points[4] = newPoints[4];
	points[5] = newPoints[5];
	Initialize();
}

void TriangleComponent::Initialize() {
	game = Game::Instance;


	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"./Shaders/Shader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);


	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };


	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"./Shaders/Shader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);


	game->WrlDevice->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	game->WrlDevice->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};


	game->WrlDevice->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);



	//create vertex buffer
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	game->WrlDevice->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	//create index buffer
	int indeces[] = { 0,1,2};
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	game->WrlDevice->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	//setup IA
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	game->DeviceContext->IASetInputLayout(layout);
	game->DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);



	//rasterizer

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	res = game->WrlDevice->CreateRasterizerState(&rastDesc, &rastState);



	
}

void TriangleComponent::Draw() {
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	game->DeviceContext->RSSetState(rastState);


	game->DeviceContext->IASetInputLayout(layout);
	game->DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	//set shaders
	game->DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	game->DeviceContext->PSSetShader(pixelShader, nullptr, 0);

	//set backbuffer for output
	game->DeviceContext->OMSetRenderTargets(1, &game->RenderView, nullptr);

	game->DeviceContext->OMSetRenderTargets(1, &game->RenderView, nullptr);

	

	game->DeviceContext->DrawIndexed(3, 0, 0);

	game->DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);


}