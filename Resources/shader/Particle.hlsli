//マテリアル
cbuffer Transform : register(b0)
{
    matrix mat;
    matrix matBillboard; //ビルボード行列
}

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
    float4 pos : POSITION; //頂点座標
    float scale : TEXCOORD;
    float4 color : COLOR;
};


//ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float2 uv : TEXCOORD; //uv座標
    float4 color : COLOR;
};