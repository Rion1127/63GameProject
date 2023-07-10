//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b0) {
	matrix mat;
    matrix viewProj;
    float3 cameraPos;
}

cbuffer Material : register(b1)
{
    float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); // �A���t�@
}

//���s����
struct DirLight
{
    float3 lightv; //���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightColor; //���C�g�̐F
    uint active;
};

//�_����
struct PointLight
{
    float3 lightpos; //���C�g���W
    float3 lightColor; //���C�g�̐F
    float3 lightAtten; //���C�g���������W��
    uint active;
};

//�X�|�b�g���C�g
struct SpotLight
{
    float3 lightv; //���C�g�̌��������̋t�x�N�g��
    float3 lightpos; //���C�g�̍��W
    float3 lightcolor; //���C�g�̐F
    float3 lightatten; //���C�g���������W��
    float2 lightActorAngleCos; //���C�g�����p�x�̃R�T�C��
    uint active;
};

//�ۉe
struct CircleShadow
{
    float3 dir; //���e�����̋t�x�N�g��
    float3 casterPos; //�L���X�^�[���W
    float distanceCasterLight; //�L���X�^�[�ƃ��C�g�̋���
    float3 atten; //���������W��
    float2 factorAngleCos; //�����p�x�̃R�T�C��
    uint active;
};

static const int DIRLIGHT_NUM = 3;
static const int POINTLIGHT_NUM = 3;
static const int SPOTLIGHT_NUM = 3;
static const int CIRCLESHADOW_NUM = 1;

cbuffer Light : register(b2)
{
    float3 ambientColor; //���C�g�ւ̕����̒P�ʃx�N�g��
    DirLight dirLights[DIRLIGHT_NUM]; //���s����
    PointLight pointLights[POINTLIGHT_NUM]; //�_����
    SpotLight spotLights[SPOTLIGHT_NUM]; //�X�|�b�g���C�g
    CircleShadow circleShadows[CIRCLESHADOW_NUM]; //�ۉe
}

static const int BONE_MATRIX_NUM = 32;
//3D�ϊ��s��
cbuffer bMatrix : register(b3)
{
    matrix bmatrix[BONE_MATRIX_NUM];
}

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv���W
   
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 bIndex : BONEINDICES;
    float4 bWeight : BONEWEIGHTS;
};

//struct PSOutput
//{
//    float4 target0 : SV_TARGET0;
//    float4 target1 : SV_TARGET1;
//};



struct SkinOutput
{
    float4 pos;
    float3 normal;
};