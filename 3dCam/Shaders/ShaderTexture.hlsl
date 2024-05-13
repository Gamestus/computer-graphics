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
	float4 worldPos : TEXCOORD1;
};

struct ConstantData
{
	matrix projectionMatrix;
	matrix globalTransform;
	float4 cameraPosition;
	matrix modelMatrix;
	matrix inverseTransform;
};

cbuffer ConstBuf : register(b0) {
	ConstantData ConstData;
}

static const float4 directionalLight = {1.0f, 0.0f, 0.0f, 1.0f};
static const float diffuseReflectionCoeff = 0.2f;
static const float lightIntencity = 5.0f;
static float4 one = {1.0f,1.0f,1.0f,1.0f};
static float4 ambient = {0.2f,0.2f,0.3f,0.1f};
static const float specularPower = 16.0f;
static const float4 specularColor = {1.0f, 1.0f, 1.0f, 1.0f};

PS_IN VSMain(VS_IN input, uint vId : SV_VertexID)
{
	PS_IN output = (PS_IN) 0;
	
	output.pos = mul(input.pos, ConstData.projectionMatrix);
	output.worldPos = mul(input.pos, ConstData.globalTransform);
	output.UV = input.UV;
	output.col = input.col;
	
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	// Texture
	float4 textureColor = DiffuseMap.Sample(Sampler, input.UV);
	
	// DiffuseLight
	
	float3 worldNormal = normalize(mul(ConstData.inverseTransform, input.col).xyz);
	float cosinLN = max(dot(directionalLight.xyz, worldNormal), 0.0f);
	float diffuse = saturate(lightIntencity * diffuseReflectionCoeff * cosinLN);
	
	// Specular
	
	float3 viewDirection = normalize(ConstData.cameraPosition - input.worldPos.xyz);
	float reflCosin = dot(worldNormal, directionalLight);
	float3 reflectedDirection = 2 * reflCosin * worldNormal - directionalLight;
	
	float specularIntensity = pow(max(dot(reflectedDirection, viewDirection), 0.0f), specularPower);
	
	// Final
	float4 lighting = ambient + (specularColor * specularIntensity) + diffuse;
	
	return lighting * textureColor;
}