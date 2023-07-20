#include "RadialBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = 0;
                // UVを-0.5～0.5に変換
    float2 symmetryUv = input.uv - 0.5;
                // 外側に行くほどこの値が大きくなる(0～0.707)
    float distance = length(symmetryUv);
    
    int _SampleCount = 8;
    float _Strength = 0.5f;
    for (int j = 0; j < _SampleCount; j++)
    {
        // jが大きいほど、画面の外側ほど小さくなる値
        float uvOffset = 1 - _Strength * j / _SampleCount * distance;
        // jが大きくなるにつれてより内側のピクセルをサンプリングしていく
        // また画面の外側ほどより内側のピクセルをサンプリングする
        col += tex.Sample(smp, symmetryUv * uvOffset + 0.5);
    }
 
    col /= _SampleCount;
    col.a = 1;
    return col;
}