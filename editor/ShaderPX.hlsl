// Pixel Shader - Shader.px

// Bemeneti struktúra a pixel shaderhez
struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Pozíció a vertex shaderből
    //float4 Color : COLOR0;    // Szín a vertex shaderből
    //float3 Normal : Normal;
    //float2 TexCoord : TEXCOORD;
};

//SamplerState objSampler : SAMPLER : register(s0);

// Pixel Shader függvény
float4 PS(PS_INPUT input) : SV_Target0
{
    return float4(1.f, 1.f, 1.f, 1.f) /*input.Color*/;
}