// Vertex Shader - Shader.hlsl

cbuffer constant : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}

// Bemeneti strukt�ra a vertex shaderhez
struct VS_INPUT
{   
    float4 Pos : POSITION; // Poz�ci� bemenet
    float4 Color : COLOR0; // Sz�n bemenet
    float3 Normal : Normal;
};

// Kimeneti strukt�ra a vertex shaderb�l
struct VS_OUTPUT
{
    float4 Pos : SV_Position; // Transzform�lt poz�ci�
    float4 Color : COLOR0; // Sz�n
    float3 Normal : Normal;
};

// Vertex Shader f�ggv�ny
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos.w = 1.f;
    
    output.Color = input.Color; // Sz�n �tvitele a pixel shaderhez
    output.Normal = input.Normal;
    
    float4 worldPos = mul(input.Pos, world);
    float4 viewPos = mul(worldPos, view);
    //output.Pos = mul(input.Pos, world);
    output.Pos = mul(viewPos, projection);
    //output.Pos = mul(output.Pos, projection);
    
    return output;
}