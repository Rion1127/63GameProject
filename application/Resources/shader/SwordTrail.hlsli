
cbuffer ConstantBuffer : register(b0)
{
    matrix worldMat;
    matrix viewProjMat; //�r���[�v���W�F�N�V�����s��
    float3 cameraPos;
};

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv���W
	float2 uv :TEXCOORD;
};