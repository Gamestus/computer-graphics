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




static const float4 directionalLight = {0.5f, 0.5f, 0.0f, 1.0f};
static const float diffuseReflectionCoeff = 0.2f;
static const float lightIntencity = 5.0f;
static float4 one = {1.0f,1.0f,1.0f,1.0f};
static float4 ambient = {0.2f,0.2f,0.3f,0.1f};
static const float specularPower = 16.0f;
static const float4 specularColor = {1.0f, 1.0f, 1.0f, 1.0f};

float CalcShadowFactor(SamplerComparisonState samShadow,
                       Texture2D shadowMap,
                       float4 shadowPosH,
                       float depthBias)
{
    // 透视除法
    shadowPosH.xyz /= shadowPosH.w;
    
    // NDC空间的深度值
    float depth = shadowPosH.z - depthBias;

    // 纹素在纹理坐标下的宽高
    const float dx = 1.0f / 2048.0f;

    float percentLit = 0.0f;

    // samShadow为compareValue <= sampleValue时为1.0f(反之为0.0f), 对相邻四个纹素进行采样比较
    // 并根据采样点位置进行双线性插值
    // float result0 = depth <= s0;  // .s0      .s1          
    // float result1 = depth <= s1;
    // float result2 = depth <= s2;  //     .depth
    // float result3 = depth <= s3;  // .s2      .s3
    // float result = BilinearLerp(result0, result1, result2, result3, a, b);  // a b为算出的插值相对位置                           
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow,
            shadowPosH.xy, depth, int2(i % 3 - 1, i / 3 - 1)).r;
    }
    
    return percentLit /= 9.0f;
}

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
	
	float3 lightVector = normalize(directionalLight.xyz);
	
	float3 worldNormal = normalize(mul(ConstData.inverseTransform, input.col).xyz);
	float cosinLN = max(dot(lightVector, worldNormal), 0.0f);
	float diffuse = saturate(lightIntencity * diffuseReflectionCoeff * cosinLN);
	
	// Specular
	
	float3 viewDirection = normalize(ConstData.cameraPosition.xyz - input.worldPos.xyz);
	//float reflCosin = max(dot(worldNormal, lightVector), 0.0f);
	//float3 reflectedDirection = 2 * reflCosin * worldNormal - lightVector;
	float3 reflectedDirection = reflect(-lightVector, worldNormal);
	float specularIntensity = pow(max(dot(reflectedDirection, viewDirection), 0.0f), specularPower);
	
	// Final
	float4 lighting = (ambient + diffuse) * textureColor + (specularColor * specularIntensity);
	
	return lighting;
}