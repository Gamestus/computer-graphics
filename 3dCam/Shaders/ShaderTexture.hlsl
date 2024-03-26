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
	matrix rotationMatrix;
};

cbuffer ConstBuf : register(b0) {
	ConstantData ConstData;
}

static const float4 directionalLight = {1.0f, 0.0f, 0.0f, 0.0f};
static const float diffuseReflectionCoeff = 0.2f;
static const float lightIntencity = 5.0f;
static float4 one = {1.0f,1.0f,1.0f,1.0f};
static float4 ambient = {0.2f,0.2f,0.2f,0.1f};

PS_IN VSMain(VS_IN input, uint vId : SV_VertexID)
{
	PS_IN output = (PS_IN) 0;
	
	output.pos = mul(input.pos,ConstData.transform);
	output.UV = input.UV;
	output.col = input.col;
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 transformedDirectionalLight = mul(ConstData.rotationMatrix, directionalLight);
	float4 textureColor = DiffuseMap.Sample(Sampler, input.UV);
	float cosin = max(dot(input.col.xyz, transformedDirectionalLight.xyz), 0.0f);
	
	float4 lighting = ambient + (lightIntencity * diffuseReflectionCoeff * cosin);
	
	return lighting * textureColor;
}