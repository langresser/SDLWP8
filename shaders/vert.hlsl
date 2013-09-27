float2x2 mat2(float x0, float x1, float x2, float x3)
{
    return float2x2(x0, x1, x2, x3);
}
float4 vec4(float2 x0, float x1, float x2)
{
    return float4(x0, x1, x2);
}
// Attributes
static float _a_angle = {0};
static float2 _a_center = {0, 0};
static float2 _a_position = {0, 0};
static float2 _a_texCoord = {0, 0};

static float4 gl_Position = float4(0, 0, 0, 0);
static float gl_PointSize = float(1);

// Varyings
static float2 _v_texCoord = {0, 0};

uniform float4x4 _u_projection : register(c0);

struct VS_INPUT
{
    float _a_angle : TEXCOORD0;
    float2 _a_center : TEXCOORD1;
    float2 _a_position : TEXCOORD2;
    float2 _a_texCoord : TEXCOORD3;
};

struct VS_OUTPUT
{
    float2 v0 : TEXCOORD0;
    float gl_PointSize : PSIZE;
    float4 gl_Position : SV_Position;
};

VS_OUTPUT main(VS_INPUT input)
{
    _a_angle = (input._a_angle);
    _a_center = (input._a_center);
    _a_position = (input._a_position);
    _a_texCoord = (input._a_texCoord);

    float _angle = radians(_a_angle);
	float _c = cos(_angle);
	float _s = sin(_angle);
	float2x2 _rotationMatrix = mat2(_c, (-_s), _s, _c);
	float2 _position = (mul(transpose(_rotationMatrix), (_a_position - _a_center)) + _a_center);
	(_v_texCoord = _a_texCoord);
	(gl_Position = mul(transpose(_u_projection), vec4(_position, 0.0, 1.0)));
	(gl_PointSize = 1.0);

    VS_OUTPUT output;
    //output.gl_Position.x = gl_Position.x;   竖屏处理
    //output.gl_Position.y = -gl_Position.y;
	output.gl_Position.x = gl_Position.y;		// 横屏处理
    output.gl_Position.y = gl_Position.x;
    output.gl_Position.z = (gl_Position.z + gl_Position.w) * 0.5;
    output.gl_Position.w = gl_Position.w;
    output.gl_PointSize = gl_PointSize;
    output.v0 = _v_texCoord;

    return output;
}
