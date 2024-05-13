#include "MeshComponent.h"
#include <chrono>
#include <WICTextureLoader.h>

MeshComponent::MeshComponent()
{
}

MeshComponent::MeshComponent(const std::vector<DirectX::XMFLOAT4>& newPoints, const std::vector<int>& nIndices) {
	points.reserve(newPoints.size());
	for(int i = 0; i < newPoints.size(); i++)
	{
		points.push_back(Vertex(newPoints[i].x, newPoints[i].y, newPoints[i].z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f));
	}
	Initialize(nIndices);
}

MeshComponent::MeshComponent(const std::vector<DirectX::XMFLOAT4>& newPoints, const std::vector<int>& nIndices, LPCWSTR Shader) {

	points.reserve(newPoints.size());
	for (int i = 0; i < newPoints.size(); i++)
	{
		points.push_back(Vertex(newPoints[i].x, newPoints[i].y, newPoints[i].z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f));
	}
	ShaderFile = Shader;
	Initialize(nIndices);
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Initialize(const std::vector<int>& nIndices) {
	
	game = Game::Instance;
	indices = nIndices;
	camera = &game->CurrCam;

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
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};


	game->WrlDevice->CreateInputLayout(
		inputElements,
		3,
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
	vertexBufDesc.ByteWidth = sizeof(Vertex) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	game->WrlDevice->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	//create index buffer
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * indices.size();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	game->WrlDevice->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	//rasterizer

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	res = game->WrlDevice->CreateRasterizerState(&rastDesc, &rastState);

	SetupConstBuffer();

	//CreateSamplerState
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;

	samplerDesc.MaxLOD = INT_MAX;

	game->WrlDevice->CreateSamplerState(&samplerDesc, &pSamplerState);

	//texture

	HRESULT hr = dx::CreateWICTextureFromFile(game->WrlDevice.Get(), TextureFile, nullptr, &Texture);
}

void MeshComponent::Draw() {
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };

	//data.offset = Vector4(globalPosition.x, globalPosition.y, globalPosition.z, 0.0f);
	data.projectionMatrix = dx::XMMatrixTranspose(
		GetGlobalTransform() *
		(*camera)->GetMatrix()
	);
	data.viewMatrix = dx::XMMatrixTranspose(
		GetGlobalTransform() *
		(*camera)->GetViewMatrix()
	);
	data.globalTransform = dx::XMMatrixTranspose(GetGlobalTransform());
	data.cameraPosition = (*camera)->GetCameraPosition();
	data.inverseTransform = dx::XMMatrixTranspose(dx::XMMatrixInverse(nullptr, GetGlobalTransform()));;

	game->DeviceContext->RSSetState(rastState);
	game->DeviceContext->PSSetShaderResources(0, 1, &Texture);	
	game->DeviceContext->PSSetSamplers(0, 1, &pSamplerState);


	//setup IA
	game->DeviceContext->IASetInputLayout(layout);
	game->DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	//set shaders
	game->DeviceContext->VSSetShader(vertexShader, nullptr, 0);
	game->DeviceContext->PSSetShader(pixelShader, nullptr, 0);

	//set buffer to vertex shader
	game->DeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
	game->DeviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);

	//Update buffer
	D3D11_MAPPED_SUBRESOURCE res = {};
	game->DeviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &data, sizeof(ConstData));

	game->DeviceContext->Unmap(constantBuffer, 0);



	game->DeviceContext->DrawIndexed(indices.size(), 0, 0);



}

void MeshComponent::SetupConstBuffer()
{

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstData);

	game->WrlDevice->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);

}
