// Varyings
static float2 _v_texCoord = {0, 0};

static float4 gl_Color[1] =
{
    float4(0, 0, 0, 0)
};

cbuffer DriverConstants : register(b1)
{
};

uniform float4 _u_modulation : register(c0);
uniform SamplerState sampler__u_texture : register(s0);
uniform Texture2D texture__u_texture : register(t0);

float4 gl_texture2D(Texture2D t, SamplerState s, float2 uv)
{
    return t.Sample(s, uv);
}

#define GL_USES_FRAG_COLOR
;
;
;
void gl_main()
{
{
float4 _abgr = gl_texture2D(texture__u_texture, sampler__u_texture, _v_texCoord);
(gl_Color[0] = _abgr);
(gl_Color[0].x = _abgr.z);
(gl_Color[0].z = _abgr.x);
(gl_Color[0].w = 1.0);
(gl_Color[0] *= _u_modulation);
}
}
;
struct PS_INPUT
{
    float2 v0 : TEXCOORD0;
    float gl_PointSize : PSIZE;
};

struct PS_OUTPUT
{
    float4 gl_Color0 : SV_Target0;
};

PS_OUTPUT main(PS_INPUT input)
{
    _v_texCoord = input.v0.xy;

    gl_main();

    PS_OUTPUT output;
    output.gl_Color0 = gl_Color[0];

    return output;
}
