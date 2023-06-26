#include "Noise.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    //スキャンノイズ
    float _Speed = 0.01f; //ノイズのスクロール速度

    float sbTime = frac(timer * _Speed);
    float seTime = sbTime + width;
    float2 uv =
        float2(
		input.uv.x + sin(smoothstep(sbTime, seTime, input.uv.y) * 2 * 3.14159) * power,
		input.uv.y
		);

    float4 texcolor = tex.Sample(smp, uv);
    return texcolor;
}