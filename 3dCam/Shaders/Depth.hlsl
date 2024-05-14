cbuffer MatrixBuffer
{
	matrix WVP;
};

struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

PixelInputType VSMain(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;
	output.position = mul(input.position, WVP);
	
	output.depthPosition = output.position;
	
	return output;
}

float4 PSMain(PixelInputType input) : SV_TARGET
{
	float depthValue = input.depthPosition.z / input.depthPosition.w;

	return float4(depthValue, depthValue, depthValue, 1.0f);
}