Texture2DArray spriteSheet : register(t0);
SamplerState spriteSampler : register(s0);

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
    uint material_id : MATERIALID;
};

float4 main(VertexOut input) : SV_TARGET
{
    return spriteSheet.Sample(spriteSampler, float3(input.texcoord, input.material_id)) * input.color;
}