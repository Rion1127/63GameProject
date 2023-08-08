#include "Fire.hlsli"

VSOutput main(VSInput vsInput)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.pos = vsInput.pos;
    output.scale = vsInput.scale;
    output.rot = vsInput.rot;
    output.ancorPoint = vsInput.ancorPoint;
    output.color = vsInput.color;
    return output;
}