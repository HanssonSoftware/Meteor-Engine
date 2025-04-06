// Vertex Shader - Shader.hlsl

cbuffer constant : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}

// Bemeneti struktúra a vertex shaderhez
struct VS_INPUT
{   
    float3 Pos : POSITION; // Pozíció bemenet
    //float4 Color : COLOR0; // Szín bemenet
    //float3 Normal : Normal;
};

// Kimeneti struktúra a vertex shaderbõl
struct VS_OUTPUT
{
    float4 Pos : SV_Position; // Transzformált pozíció
    //float4 Color : COLOR0; // Szín
    //float3 Normal : Normal;
};

// Vertex Shader függvény
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.Pos = mul(float4(input.Pos, 1.f), world);
    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, projection);
    
   
    return output;
}