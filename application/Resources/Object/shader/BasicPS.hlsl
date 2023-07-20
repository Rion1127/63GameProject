#include "Basic.hlsli"

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
    float3 ambient = m_ambient * 0.3f * ambientColor;
	//シェーディング
    float4 shadecolor = float4(ambient, m_alpha);
    float4 color = { 1, 1, 1, 1 };
	//平行光源
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//ライトに向かうベクトルと法線の内積
            float intensity = saturate(dot(normalize(input.normal), normalize(dirLights[i].lightv)));
			//反射光ベクトル
            float3 reflect = -dirLights[i].lightv + 2 * input.normal * dot(input.normal, dirLights[i].lightv);
			//拡散反射光
            float3 diffuse =  m_diffuse * intensity  * dirLights[i].lightColor;
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightColor;
			//すべて加算する
            shadecolor.rgb += (diffuse.rgb + specular.rgb) * dirLights[i].lightColor;
            //shadecolor.a = 1;
        }
    }
	//点光源
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
			//ライトのベクトル
            float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
            float d = length(lightv);
			//正規化し、単位ベクトルにする
            lightv = normalize(lightv);
			//距離減衰係数
            float atten = 1.0f / (pointLights[i].lightAtten.x + pointLights[i].lightAtten.y * d +
			pointLights[i].lightAtten.z * d * d);
			//ライトに向かうベクトルと法線の内積
            float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべて加算する
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;

        }
    }
	//スポットライト
    for (int i = 0; i < SPOTLIGHT_NUM; i++)
    {
        if (spotLights[i].active)
        {
			//ライトへの方向ベクトル
            float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);
			//距離減衰係数
            float atten =
			saturate(1.0f / (spotLights[i].lightatten.x +
			spotLights[i].lightatten.y * d +
			spotLights[i].lightatten.z * d * d));
			//角度減衰
            float cos = dot(lightv, spotLights[i].lightv);
			//減衰開始角度から、減衰終了角度にかけて減衰
			//減衰開始角度の内側は１倍 減衰終了角度の外側は0倍の輝度
            float angleatten =
			smoothstep(spotLights[i].lightActorAngleCos.y,
			spotLights[i].lightActorAngleCos.x, cos);
			//角度減衰を乗算
            atten *= angleatten;
			//ライトに向かうベクトルと法線の内積
            float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべて加算する
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
        }
    }
	//丸影
    for (int i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
			//オブジェクト表面からキャスターへのベクトル
            float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			//投影方向での距離
            float d = dot(casterv, circleShadows[i].dir);
			//距離減衰係数
            float atten =
			saturate(1.0f / (circleShadows[i].atten.x +
			circleShadows[i].atten.y * d +
			circleShadows[i].atten.z * d * d));
			//距離がマイナスなら0にする
            atten *= step(0, d);
			//仮想ライトの座標
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//オブジェクト表面からライトへのベクトル（単位ベクトル）
            float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//角度減衰
            float cos = dot(lightv, circleShadows[i].dir);
			//減衰開始各おから、減衰終了角度にかけて減衰
			//減衰開始角度の内側は1倍、減衰終了角度の外側は0倍の輝度
            float angleatten =
			smoothstep(circleShadows[i].factorAngleCos.y,
			circleShadows[i].factorAngleCos.x, cos);
			//角度減衰を乗算
            atten *= angleatten;
			//すべて減算する
            shadecolor.rgb -= atten;
        }
    }
		
   
    output.target0 = shadecolor * texcolor;
    output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);
	//シェーディングによる色で描画
    return output;
	
    //return float4(input.uv.xy, 1, 1);
    //return float4(tex.Sample(smp, input.uv));
}