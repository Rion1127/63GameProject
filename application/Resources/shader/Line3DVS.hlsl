#include "Line3D.hlsli"


VSOutput main(float4 pos : POSITION)
{
    VSOutput output;
    output.svpos = mul(viewProjMat, pos);
    return output;
}