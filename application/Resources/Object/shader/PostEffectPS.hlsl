#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    
    return float4(texcolor.rgb, 1);
    
    //ガウシアンブラー
    //float totalWeight = 0, _Sigma = 0.015, _StepWidth = 0.001;
    //float4 col = float4(0, 0, 0, 0);

    //for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    //{
    //    for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
    //    {
    //        float2 pickUV = input.uv + float2(px, py);
    //        float weight = Gaussian(input.uv, pickUV, _Sigma);
    //        col += tex.Sample(smp, pickUV) * weight; //Gaussianで取得した「重み」を色にかける
    //        totalWeight += weight; //かけた「重み」の合計値を控えておく
    //    }
    //}
    //col.rgb = col.rgb / totalWeight; //かけた「重み」分、結果から割る
    //col.a = 1;
    //return col;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}