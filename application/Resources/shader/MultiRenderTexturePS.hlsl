#include "MultiTexture.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);



float4 main(VSOutput input) : SV_TARGET
{
    float4 colorTex0 = tex0.Sample(smp, input.uv);
    float4 colorTex1 = tex1.Sample(smp, input.uv);
    
    float4 color = colorTex0;
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        color = float4(1, 1, 1, 1) - colorTex1;
    }
    
    
    return float4(color.rgb, 1);
    
}