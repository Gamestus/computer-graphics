Texture2D ScreenTexture : register(t0);


struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};


PS_IN VSMain(uint id : SV_VertexID)
{
	PS_IN output;
	
	output.tex = float2(id & 1, (id & 2) >> 1);
	output.pos = float4(output.tex * float2(2,-2) + float2(-1, 1), 0, 1);
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 screen = ScreenTexture.Load(int4(input.pos));
	return pow(screen, 3);
}