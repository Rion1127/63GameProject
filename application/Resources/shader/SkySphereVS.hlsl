#include "SkySphere.hlsli"


VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(mat, float4(normal, 0)));
    float4 wpos = mul(mat, pos);
	
	//ピクセルシェーダーに渡す値
    VSOutput output;
    output.svpos = mul(mul(viewProj, mat), pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    return output;
}