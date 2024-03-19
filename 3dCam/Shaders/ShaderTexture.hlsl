Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
	float2 UV : TEXCOORD;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
	float2 UV : TEXCOORD;
};

struct ConstantData
{
	matrix transform;
	float4 color;
};

cbuffer ConstBuf : register(b0) {
	ConstantData ConstData;
}

PS_IN VSMain(VS_IN input, uint vId : SV_VertexID)
{
	PS_IN output = (PS_IN) 0;
	
	output.pos = mul(input.pos,ConstData.transform);
	output.UV = input.UV;
	output.col = ConstData.color;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = DiffuseMap.Sample(Sampler, input.UV) * input.col;
	return col;
}