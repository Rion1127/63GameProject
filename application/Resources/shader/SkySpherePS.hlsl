#include "SkySphere.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
	//テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
	
	//光沢度
    const float shininess = 20.0f;
	//頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//環境反射光
    float3 ambient = m_ambient * 1.f * ambientColor;
	//シェーディング
    float4 shadecolor = float4(ambient, m_alpha);
    float4 color = { 1, 1, 1, 1 };
	
    output.target0 = shadecolor * texcolor;
    output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);
	//シェーディングによる色で描画
    return output;
}