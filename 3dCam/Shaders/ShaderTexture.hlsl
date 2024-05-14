Texture2D DiffuseMap : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SamplerTypeClamp : register(s0);
SamplerState SamplerTypeClampNew : register(s1);



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
	float4 lightViewPosition: TEXCOORD2;
	float3 lightPos : TEXCOORD3;
};

struct ConstantData
{
	matrix projectionMatrix;
	matrix globalTransform;
	float4 cameraPosition;
	matrix modelMatrix;
	matrix inverseTransform;
	matrix lightMatrix;
	float4 lightPosition;
};

cbuffer ConstBuf : register(b0) {
	ConstantData ConstData;
}




static const float4 directionalLight = {0.0f, 0.4f, -0.5f, 1.0f};
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
	
	float3 p = mul(ConstData.inverseTransform, input.pos).xyz;

	output.lightViewPosition =  mul(float4(p, 1.0f), ConstData.lightMatrix);
	
	// Определение позиции света на основе позиции света и позиции вершины в мире
	output.lightPos = ConstData.lightPosition.xyz - output.worldPos.xyz;
	output.lightPos = normalize(output.lightPos);
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float3 worldNormal = normalize(mul(ConstData.inverseTransform, input.col).xyz);

	
	//texture
	float4 textureColor = DiffuseMap.Sample(SamplerTypeClamp, input.UV);
	float4 color = ambient * textureColor;
	// Shadows
	
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	
	float bias = 0.001f;
	
	projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture.Sample(SamplerTypeClampNew, projectTexCoord).r;
		
		// Вычисление глубины света
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		
		// Вычитание смещения из lightDepthValue
		lightDepthValue = lightDepthValue - bias;
		
		// Сравнение глубины теневой карты и глубины света, для определения того, освещен или затенен пиксель
		// Если свет перед объектом, то пиксель освещен; если нет, то пиксель затенен и объект бросает тень за ним
		if(lightDepthValue < depthValue)
		{
			float3 lightVector = normalize(ConstData.lightPosition.xyz - input.worldPos.xyz);
			float cosinLN = max(dot(lightVector, worldNormal), 0.0f);
			float diffuse = saturate(lightIntencity * diffuseReflectionCoeff * cosinLN);
			
			float3 viewDirection = normalize(ConstData.cameraPosition.xyz - input.worldPos.xyz);
			//float reflCosin = max(dot(worldNormal, lightVector), 0.0f);
			//float3 reflectedDirection = 2 * reflCosin * worldNormal - lightVector;
			float3 reflectedDirection = reflect(-lightVector, worldNormal);
			float specularIntensity = pow(max(dot(reflectedDirection, viewDirection), 0.0f), specularPower);
			
			
			
			float4 specular = specularColor * specularIntensity;
			
			color = (diffuse + ambient) * textureColor + specular;
		}
	}


	
	return color;
}