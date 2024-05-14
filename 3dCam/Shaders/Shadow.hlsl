Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

cbuffer MatrixBuffer
{
	matrix world;
	matrix wvp;
	matrix wvplight;
};

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};

cbuffer LightBuffer2
{
	float3 lightPosition;
	float padding;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

PixelInputType VSMain(VertexInputType input)
{
	PixelInputType output;
        
	input.position.w = 1.0f;

	output.position = mul(input.position, wvp); 
	output.lightViewPosition = mul(input.position, wvplight);

	output.tex = input.tex;
    
	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	float4 worldPosition = mul(input.position, world);

	output.lightPos = lightPosition.xyz - worldPosition.xyz;
	output.lightPos = normalize(output.lightPos);

	return output;
}

float4 PSMain(PixelInputType input) : SV_TARGET
{
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
    float lightIntensity;
	float4 textureColor;

	// Установка значения смещения используемого для устранения проблем точности с плавающей запятой
	float bias = 0.001f;

	float4 color = ambientColor;

	// Вычисление координат проецирования текстуры
	projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// Находится ли спроецированные координаты в пределах 0 и 1. Если да, то пиксель находится в видимости света
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		// Вычисление глубины света
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// Вычитание смещения из lightDepthValue
		lightDepthValue = lightDepthValue - bias;

		// Сравнение глубины теневой карты и глубины света, для определения того, освещен или затенен пиксель
		// Если свет перед объектом, то пиксель освещен; если нет, то пиксель затенен и объект бросает тень за ним
		if(lightDepthValue < depthValue)
		{
		    // Вычисление количества света в пикселе
			lightIntensity = saturate(dot(input.normal, input.lightPos));

		    if(lightIntensity > 0.0f)
			{
				// Определение заключительного рассеяного (diffuse) света на основе рассеяного цвети и интенсивности света
				color += (diffuseColor * lightIntensity);
				color = saturate(color);
			}
		}
	}

	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

	return color * textureColor;
}