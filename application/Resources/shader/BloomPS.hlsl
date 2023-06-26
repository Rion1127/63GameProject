#include "Bloom.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col0 = tex0.Sample(smp, input.uv);
    float4 col1 = tex1.Sample(smp, input.uv);
    
    col0.a = 1.f;
    col1.a = 1.f;
    return col0 + col1;
    
}