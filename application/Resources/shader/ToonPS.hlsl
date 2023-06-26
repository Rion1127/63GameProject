#include "Toon.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(VSOutput input) : SV_TARGET
{
	
    PSOutput output;
	//�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
	
	
	//����x
    const float shininess = 20.0f;
	//���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//�����ˌ�
    float3 ambient = m_ambient * 0.3f;
	//�V�F�[�f�B���O
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
    float4 color = { 1, 1, 1, 1 };
	//���s����
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-dirLights[i].lightv) + 2 * input.normal * dotlightnormal;
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse * dirLights[i].lightColor;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightColor;
			//���ׂĉ��Z����
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightColor;
			
            float3 pos = { 0, 0, 0 };
			//�A�e
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
			
            float4 white = { 1, 1, 1, 1 };
            float4 blue = { 0.04, 0.43, 1, 1 };
			//�g�D�[���J���[
            color.rgb = (step(0.7f, intensity) == 1) ?
						white.rgb : white.rgb * 0.45f;
			
            color.rgb *= dirLights[i].lightColor.rgb;
			
            float4 toonSpecular;
			
            toonSpecular.rgb =
						(step(0.3f, specular.r) == 1) ?
						dirLights[i].lightColor.rgb : texcolor.rgb;
			//�e�N�X�`���J���[�ɑ��
            texcolor.rgb = toonSpecular.rgb;
			
          //////�������C�g//////
            half rim = 1.0f - abs(dot(eyedir, input.normal));
            float3 emission = 0;
					//���ϒl(rim)���(_RimRange)�Ƀg�D�[�����ɂ���
            emission =
						(step(rim, 0.7f) == 1) ?
						texcolor.rgb : blue.rgb;
					//�������C�g�̐F��������
            texcolor.rgb = emission.rgb;
            texcolor = float4(texcolor.rgb, 1.0f);

        }
    }
	//�_����
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
			//���C�g�̃x�N�g��
            float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
            float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
            lightv = normalize(lightv);
			//���������W��
            float atten = 1.0f / (pointLights[i].lightAtten.x + pointLights[i].lightAtten.y * d +
			pointLights[i].lightAtten.z * d * d);
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;

        }
    }
	//�X�|�b�g���C�g
    for (int i = 0; i < SPOTLIGHT_NUM; i++)
    {
        if (spotLights[i].active)
        {
			//���C�g�ւ̕����x�N�g��
            float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);
			//���������W��
            float atten =
			saturate(1.0f / (spotLights[i].lightatten.x +
			spotLights[i].lightatten.y * d +
			spotLights[i].lightatten.z * d * d));
			//�p�x����
            float cos = dot(lightv, spotLights[i].lightv);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓����͂P�{ �����I���p�x�̊O����0�{�̋P�x
            float angleatten =
			smoothstep(spotLights[i].lightActorAngleCos.y,
			spotLights[i].lightActorAngleCos.x, cos);
			//�p�x��������Z
            atten *= angleatten;
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
        }
    }
	//�ۉe
    for (int i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
			//�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
            float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			//���e�����ł̋���
            float d = dot(casterv, circleShadows[i].dir);
			//���������W��
            float atten =
			saturate(1.0f / (circleShadows[i].atten.x +
			circleShadows[i].atten.y * d +
			circleShadows[i].atten.z * d * d));
			//�������}�C�i�X�Ȃ�0�ɂ���
            atten *= step(0, d);
			//���z���C�g�̍��W
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
            float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//�p�x����
            float cos = dot(lightv, circleShadows[i].dir);
			//�����J�n�e������A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{�A�����I���p�x�̊O����0�{�̋P�x
            float angleatten =
			smoothstep(circleShadows[i].factorAngleCos.y,
			circleShadows[i].factorAngleCos.x, cos);
			//�p�x��������Z
            atten *= angleatten;
			//���ׂČ��Z����
            shadecolor.rgb -= atten;
        }
    }
		
    
    output.target0 = color * texcolor;
    output.target1 = float4(1 - (color * texcolor).rgb, 1);
	//�V�F�[�f�B���O�ɂ��F�ŕ`��
    return output;
	
    //return float4(input.uv.xy, 1, 1);
    //return float4(tex.Sample(smp, input.uv));
}