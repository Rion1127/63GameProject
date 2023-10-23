#include "Line3D.hlsli"

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    return color;
}