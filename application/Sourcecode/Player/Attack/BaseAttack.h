#pragma once
#include "AttackEditor.h"
#include "Spline.h"
#include <vector>

class BaseAttack
{
public:
	struct AttackInput {
		AttackType type_;		//�ʏ�U�����t�B�j�b�V���Z��
		std::vector<AttackEditor::AttackInfo> attackinfo;	//�U���̏ڍ�
	};
private:
	float DamageCoolTime_;		//���G����
	AttackInput attackinput_;	//�U���̏ڍ�
	Spline spline_;
public:
	BaseAttack(const AttackInput& input);

	void Update();

	void Draw();
};

