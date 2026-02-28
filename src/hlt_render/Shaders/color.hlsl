//***************************************************************************************
// color.hlsl - Version avec texture + lighting
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gWorld;
};

cbuffer cbColor : register(b1)
{
    float4 gObjectColor;
};

//cbuffer cbLight : register(b2)
//{
//    float3 gLightDir;
//    float pad;
//    float4 gLightColor;
//};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VertexIn
{
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float2 TexC : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 NormalW : NORMAL;
    float2 TexC : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Position clip space
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    // Normale en world space
    vout.NormalW = mul(vin.Normal, (float3x3) gWorld);

    vout.TexC = vin.TexC;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float3 normal = normalize(pin.NormalW);
    //float3 lightDir = normalize(-gLightDir);

    //float diffuse = saturate(dot(normal, lightDir));

    float4 texColor = gTexture.Sample(gSampler, pin.TexC);

    float4 finalColor = texColor * gObjectColor; //* gLightColor * diffuse

    return finalColor;
}