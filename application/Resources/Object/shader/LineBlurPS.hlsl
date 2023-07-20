#include "LineBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    float AngleDeg = radian;
    
    float totalWeight = 0;
    float4 color = float4(0, 0, 0, 0);
    float2 pickUV = float2(0, 0);
    float pickRange = 0.06f;
    float angleRad = AngleDeg * 3.14159 / 180;
    [loop]
    for (float i = -pickRange; i <= pickRange * 2; i += 0.005)
    {
        float x = cos(angleRad) * i;
        float y = sin(angleRad) * i;
        pickUV = input.uv + float2(x, y);
        
        float weight = Gaussian(input.uv, pickUV, pickRange);
        color += tex.Sample(smp, pickUV) * weight;
        totalWeight += weight;
    }
    color = color / totalWeight;
    return color;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}