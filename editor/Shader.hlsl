// Vertex Shader - Shader.hlsl

cbuffer constant : register(b0)
{
    float4x4 matrices;
    float4x4 view;
    float4x4 projection;
}

// Bemeneti struktúra a vertex shaderhez
struct VS_INPUT
{   
    float4 Pos : POSITION; // Pozíció bemenet
    float4 Color : COLOR0; // Szín bemenet
    float3 Normal : Normal;
};

// Kimeneti struktúra a vertex shaderbõl
struct VS_OUTPUT
{
    float4 Pos : SV_Position; // Transzformált pozíció
    float4 Color : COLOR0; // Szín
    float3 Normal : Normal;
};

// Vertex Shader függvény
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos.w = 1.f;
    
    output.Color = input.Color; // Szín átvitele a pixel shaderhez
    output.Normal = input.Normal;
    
    output.Pos = mul(input.Pos, matrices);
    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, projection);
    
    return output;
}