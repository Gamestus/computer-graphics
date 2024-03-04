struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
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
	output.col = ConstData.color;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
	return col;
}