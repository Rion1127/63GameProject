
cbuffer ConstantBuffer : register(b0)
{
    matrix worldMat;
    matrix viewProjMat; //ビュープロジェクション行列
    float3 cameraPos;
};

cbuffer ConstantBufferColor : register(b1)
{
    float4 color;
};

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION;
};