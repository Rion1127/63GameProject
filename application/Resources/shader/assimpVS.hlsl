#include "assimp.hlsli"


SkinOutput ComputeSkin(VSInput input)
{
	
    SkinOutput output = (SkinOutput)0;
    
    uint iBone;
    float weight;
    matrix m;
    
    iBone = input.bIndex.x;
    weight = input.bWeight.x;
    m = bmatrix[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.bIndex.y;
    weight = input.bWeight.y;
    m = bmatrix[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.bIndex.z;
    weight = input.bWeight.z;
    m = bmatrix[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    iBone = input.bIndex.w;
    weight = input.bWeight.w;
    m = bmatrix[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    return output;
}

VSOutput main(VSInput input)
{
    SkinOutput skinned = ComputeSkin(input);
    
    float4 wnormal = normalize(mul(mat, float4(skinned.normal, 0)));
    
    VSOutput output;
    
    output.svpos = mul(mul(viewProj, mat), skinned.pos);
    
    output.normal = wnormal.xyz;
    
    output.uv = input.uv;
    
    return output;

}