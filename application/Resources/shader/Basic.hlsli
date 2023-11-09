//3D変換行列
cbuffer ConstBufferDataTransform : register(b0) {
	matrix mat;
    matrix viewProj;
    float3 cameraPos;
}

cbuffer Material : register(b1)
{
    float3 m_ambient : packoffset(c0); // アンビエント係数
    float3 m_diffuse : packoffset(c1); // ディフューズ係数
    float3 m_specular : packoffset(c2); // スペキュラー係数
    float m_alpha : packoffset(c2.w); // アルファ
}

//平行光源
struct DirLight
{
    float3 lightv; //ライトへの方向の単位ベクトル
    float3 lightColor;  //ライトの色
    uint active;
};

//点光源
struct PointLight
{
    float3 lightpos;    //ライト座標
    float3 lightColor;  //ライトの色
    float3 lightAtten;  //ライト距離減衰係数
    uint active;
};

//スポットライト
struct SpotLight
{
    float3 lightv;      //ライトの光線方向の逆ベクトル
    float3 lightpos;    //ライトの座標
    float3 lightcolor;  //ライトの色
    float3 lightatten;  //ライト距離減衰係数
    float2 lightActorAngleCos;  //ライト減衰角度のコサイン
    uint active;
};

//丸影
struct CircleShadow
{
    float3 dir;                 //東映方向の逆ベクトル
    float3 casterPos;           //キャスター座標
    float distanceCasterLight;  //キャスターとライトの距離
    float3 atten;               //距離減衰係数
    float2 factorAngleCos;      //減衰角度のコサイン
    uint active;
};

static const int DIRLIGHT_NUM = 3;
static const int POINTLIGHT_NUM = 3;
static const int SPOTLIGHT_NUM = 3;
static const int CIRCLESHADOW_NUM = 30;

cbuffer Light : register(b2)
{
    float3 ambientColor; //ライトへの方向の単位ベクトル
    DirLight dirLights[DIRLIGHT_NUM];       //平行光源
    PointLight pointLights[POINTLIGHT_NUM]; //点光源
    SpotLight spotLights[SPOTLIGHT_NUM];    //スポットライト
    CircleShadow circleShadows[CIRCLESHADOW_NUM];   //丸影
}




//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION; //システム用頂点座標
    //float4 color : COLOR;//色
    float4 worldpos : POSITION;//ワールド座標
    float3 normal : NORMAL; //法線ベクトル
    float2 uv : TEXCOORD; //uv座標
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};