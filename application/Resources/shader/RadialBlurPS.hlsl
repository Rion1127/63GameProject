#include "RadialBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = 0;
                // UV��-0.5�`0.5�ɕϊ�
    float2 symmetryUv = input.uv - 0.5;
                // �O���ɍs���قǂ��̒l���傫���Ȃ�(0�`0.707)
    float distance = length(symmetryUv);
    
    int _SampleCount = 8;
    float _Strength = 0.5f;
    for (int j = 0; j < _SampleCount; j++)
    {
        // j���傫���قǁA��ʂ̊O���قǏ������Ȃ�l
        float uvOffset = 1 - _Strength * j / _SampleCount * distance;
        // j���傫���Ȃ�ɂ�Ă������̃s�N�Z�����T���v�����O���Ă���
        // �܂���ʂ̊O���قǂ������̃s�N�Z�����T���v�����O����
        col += tex.Sample(smp, symmetryUv * uvOffset + 0.5);
    }
 
    col /= _SampleCount;
    col.a = 1;
    return col;
}