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
    float3 Pos : POSITION; // Poz�ci� bemenet
    //float4 Color : COLOR0; // Sz�n bemenet
    //float3 Normal : Normal;
};

// Kimeneti strukt�ra a vertex shaderb�l
struct VS_OUTPUT
{
    float4 Pos : SV_Position; // Transzform�lt poz�ci�
    //float4 Color : COLOR0; // Sz�n
    //float3 Normal : Normal;
};

// Vertex Shader f�ggv�ny
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.Pos = mul(float4(input.Pos, 1.f), world);
    output.Pos = mul(output.Pos, view);
    output.Pos = mul(output.Pos, projection);
    
   
    return output;
}