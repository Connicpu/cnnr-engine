cbuffer Camera : register(b0)
{
    float3x2 cam_transform;
    float layer;
}

struct VertexIn
{
    float2 vert_pos : POSITION0;
    float2 vert_uv  : TEXCOORD0;

    float2 inst_pos : POSITION1;
    float inst_rot  : ROTATION1;
    half inst_scale : SCALE1;
    uint material   : MATERIAL1;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    uint material : MATERIAL;
};

float3x3 affine2d(float3x2 compact)
{
    return transpose(float3x3(compact, float3(0, 0, 1)));
}

float2 rotate(float2 pos, float rot)
{
    float s, c;
    sincos(rot, s, c);
    float2x2 mat = float2x2(
        float2(c, -s),
        float2(s, c)
        );
    return mul(mat, pos);
}

VertexOut main(VertexIn input)
{
    float2 pos2 = input.vert_pos;
    pos2 = rotate(pos2, input.inst_rot);
    pos2 = pos2 * input.inst_scale;
    pos2 = pos2 + input.inst_pos;
    float3 pos = float3(pos2, 1);
    pos = mul(affine2d(cam_transform), pos);

    VertexOut output;
    output.position = float4(pos.xy, layer, 1);
    output.texcoord = input.vert_uv;
    output.material = input.material;
    return output;
}