cbuffer ConstantBuffer : register(b0)
{
    matrix worldMat;
    matrix viewProjMat; //ビュープロジェクション行列
    float3 cameraPos;
};

struct VSInput
{
    float4 pos : POSITION; // システム用頂点座標
    float scale : SCALE; // スケール
    float3 rot : ROTATION; // 角度
    float2 ancorPoint : ANCORPOINT;
    float4 color : COLOR; //色
};

struct VSOutput
{
    float4 pos : SV_POSITION; // システム用頂点座標
    float scale : SCALE; // スケール
    float3 rot : ROTATION; // 角度
    float2 ancorPoint : ANCORPOINT;
    float4 color : COLOR; //色
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv値
    float4 color : COLOR; //色
};
