cbuffer Camera : register(b0)
{
    float3x2 cam_transform;
}

struct VertexIn
{
    // quad per-vertex data
    float2 vert_pos : POSITION;
    float2 vert_uv  : TEXCOORD0;

    // instance data
    float3x2 transform : TRANSFORM;
    uint material_id : MATERIALID0;
    float layer : LAYER;
    float4 rect : TEXCOORD1; // <left, top, right, bottom>
    float4 tint : COLOR0;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
    uint material_id : MATERIALID;
};

float3x3 affine2d(float3x2 compact)
{
    return transpose(float3x3(compact, float3(0, 0, 1)));
}

VertexOut main(VertexIn input)
{
    VertexOut output;
    float3 pos = float3(input.vert_pos, 1);
    pos = mul(affine2d(input.transform), pos);
    pos = mul(affine2d(cam_transform), pos);
    output.position = float4(pos.xy, input.layer, 1);
    output.color = input.tint;
    output.material_id = input.material_id;
    output.texcoord = lerp(input.rect.xy, input.rect.zw, input.vert_uv);
    return output;
}
