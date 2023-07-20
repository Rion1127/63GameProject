cbuffer ConstBufferTime : register(b0) {
    int timer;
    float power; //ノイズがかかる幅
    float width; //揺らめき具合
}

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput {
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv座標
	float2 uv :TEXCOORD;
};

