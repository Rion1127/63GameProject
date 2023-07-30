#include "Smoke.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(GSOutput input) : SV_TARGET
{
    
    //float alpha = smoothstep(0 + uvPos, 3.0f + uvPos, input.uv.y);
    
    float4 texColor = tex.Sample(smp, input.uv);
    
    //float testAlpha = 0.5f - abs((0.5f) - input.uv.y);
    
    //texColor.a *= testAlpha;
    
    texColor *= input.color;
    
    return texColor /** input.color*/;
}