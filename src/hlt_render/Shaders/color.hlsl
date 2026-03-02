//***************************************************************************************
// color.hlsl - Version avec texture + lighting
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
    float3 NormalL : NORMAL;
    float2 TexC : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 TexC : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout = (VertexOut) 0.0f;

    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosW = posW.xyz;

    vout.NormalW = mul(vin.NormalL, (float3x3) gWorld);

    vout.PosH = mul(posW, gViewProj);

    float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), gTexTransform);
    vout.TexC = texC.xy;

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