//***************************************************************************************
// color.hlsl - Version texture only (sans normales)
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
    float4x4 gViewProj;
    float4x4 gWorld;
    float4x4 gTexTransform;
};

cbuffer cbColor : register(b1)
{
    float4 gObjectColor;
};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VertexIn
{
    float3 PosL : POSITION;
    float2 TexC : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 TexC : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout = (VertexOut)0.0f;

    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosH = mul(posW, gViewProj);

    float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), gTexTransform);
    vout.TexC = vin.TexC;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 texColor = gTexture.Sample(gSampler, pin.TexC);
    return texColor ;
}