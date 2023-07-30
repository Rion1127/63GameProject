#include "Smoke.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //����
    float4(-0.5f, +0.5f, 0, 0), //����
    float4(+0.5f, -0.5f, 0, 0), //�E��
    float4(+0.5f, +0.5f, 0, 0), //�E��
};

//���オ0,0�@�E����1,1
static const float2 uv_array[vnum] =
{
    float2(0.0f, 1.0f), //����
    float2(0.0f, 0.0f), //����
    float2(1.0f, 1.0f), //�E��
    float2(1.0f, 0.0f), //�E��
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //�o�͗p���_�f�[�^

    for (uint i = 0; i < vnum; i++)
    {
        //���S����̃I�t�Z�b�g���X�P�[�����O
        float4 offset =
        {
            (offset_array[i].x - input[0].ancorPoint.x) * input[0].scale,
            (offset_array[i].y - input[0].ancorPoint.y) * input[0].scale,
            offset_array[i].z * input[0].scale,
            offset_array[i].w * input[0].scale,
        };

        offset = mul(worldMat, offset);

        element.svpos = input[0].pos + offset;

        //�r���[�A�ˉe�ϊ�
        element.svpos = mul(viewProjMat, element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        output.Append(element);
    }
}

