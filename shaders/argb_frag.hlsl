
uniform SamplerState sampler__CC_Texture0 : register(s0);
uniform Texture2D texture__CC_Texture0 : register(t0);

float4 gl_texture2D(Texture2D t, SamplerState s, float2 uv)
{
    return t.Sample(s, uv);
}

struct PS_INPUT
{
    float2 texCoord : TEXCOORD;
    float4 gl_Position : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	//return float4(1.0f, 0.0f, 1.0f, 1.0f);
    return float4(gl_texture2D(texture__CC_Texture0, sampler__CC_Texture0, input.texCoord.xy).rgb, 1.0f);
}
