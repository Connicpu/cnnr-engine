cbuffer Camera : register(b0)
{
    float2 cam_row1;
    float2 cam_row2;
    float2 cam_row3;
}

struct VertexIn
{
    // quad per-vertex data
    float2 vert_pos : POSITION;
    float2 vert_uv  : TEXCOORD0;

    // instance data
    float2 transform0 : TRANSFORM0;
    float2 transform1 : TRANSFORM1;
    float2 transform2 : TRANSFORM2;
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

float3x3 affine2d(float2 row1, float2 row2, float2 row3)
{
    return float3x3(
        float3(row1.x, row2.x, row3.x),
        float3(row1.y, row2.y, row3.y),
        float3(0, 0, 1)
        );
}

VertexOut main(VertexIn input)
{
    float3x3 model_transform = affine2d(input.transform0, input.transform1, input.transform2);
    float3x3 camera_transform = affine2d(cam_row1, cam_row2, cam_row3);

    VertexOut output;
    float3 pos = float3(input.vert_pos, 1);
    pos = mul(model_transform, pos);
    pos = mul(camera_transform, pos);
    output.position = float4(pos.xy, 1 - input.layer / 1000000, 1);
    output.color = input.tint;
    output.material_id = input.material_id;
    output.texcoord = lerp(input.rect.xy, input.rect.zw, input.vert_uv);
    return output;
}
