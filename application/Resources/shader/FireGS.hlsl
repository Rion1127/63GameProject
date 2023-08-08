#include "Fire.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //左下
    float4(-0.5f, +0.5f, 0, 0), //左上
    float4(+0.5f, -0.5f, 0, 0), //右下
    float4(+0.5f, +0.5f, 0, 0), //右上
};

//左上が0,0　右下が1,1
static const float2 uv_array[vnum] =
{
    float2(0.0f, 1.0f), //左下
    float2(0.0f, 0.0f), //左上
    float2(1.0f, 1.0f), //右下
    float2(1.0f, 0.0f), //右上
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //出力用頂点データ

    for (uint i = 0; i < vnum; i++)
    {
        //中心からのオフセットをスケーリング
        float4 offset =
        {
            (offset_array[i].x - input[0].ancorPoint.x) * input[0].scale,
            (offset_array[i].y - input[0].ancorPoint.y) * input[0].scale,
            offset_array[i].z * input[0].scale,
            offset_array[i].w * input[0].scale,
        };

        // X軸回転行列
        matrix rotXMat =
        {
            1, 0, 0, 0,
            0, cos(input[0].rot.x), sin(input[0].rot.x), 0,
            0, -sin(input[0].rot.x), cos(input[0].rot.x), 0,
            0, 0, 0, 1,
        };
        // Y軸回転行列
        matrix rotYMat =
        {
            cos(input[0].rot.y), 0, -sin(input[0].rot.y), 0,
            0, 1, 0, 0,
            sin(input[0].rot.y), 0, cos(input[0].rot.y), 0,
            0, 0, 0, 1
        };

        // Z軸回転行列
        matrix rotZMat =
        {
            cos(input[0].rot.z), sin(input[0].rot.z), 0, 0,
            -sin(input[0].rot.z), cos(input[0].rot.z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        offset = mul(rotZMat, offset);
        offset = mul(rotXMat, offset);
        offset = mul(rotYMat, offset);

        offset = mul(worldMat, offset);

        element.svpos = input[0].pos + offset;

        //ビュー、射影変換
        element.svpos = mul(viewProjMat, element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        output.Append(element);
    }
}