#include "Gaussian.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    //2.���o�����摜�Ƀu���[��������
    float totalWeight = 0, _Sigma = 0.040, _StepWidth = 0.0035;
    float4 col = float4(0, 0, 0, 0);

    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            col += tex.Sample(smp, pickUV) * weight; //Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
            totalWeight += weight; //�������u�d�݁v�̍��v�l���T���Ă���
        }
    }
    col.rgb = col.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
    col.a = 1;
    
    ////���P�x���𒊏o
    return col;
    
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}