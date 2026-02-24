//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};
cbuffer cbColor : register(b1)
{
    float4 gObjectColor;
};
struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    vout.PosH = float4(vin.PosL, 1.0f); // PAS de matrice
    vout.Color = float4(1, 0, 0, 1); // Rouge constant
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}


