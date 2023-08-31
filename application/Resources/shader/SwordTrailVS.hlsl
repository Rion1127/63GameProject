#include "SwordTrail.hlsli"

VSOutput main( float4 pos : POSITION , float2 uv : TEXCOORD)
{
	VSOutput output;
    output.svpos = mul(viewProjMat, pos);
	output.uv = uv;
	return output;
}