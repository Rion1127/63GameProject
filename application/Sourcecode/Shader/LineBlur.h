#pragma once
#include "IPostEffect.h"

class LineBlur final:
    public IPostEffect
{
private:
	struct ConstBuffRad {
		float radian;
	};

	float radian_;
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�萔�o�b�t�@�p�f�[�^�\����

	ConstBuffRad* constMap_;
protected:
	//�V�F�[�_�[�֒l��n��
	void TransferBuff() override;
	void SendToShader() override;
public:
	LineBlur();
	void SetRadian(float rad) { radian_ = rad; }
};

