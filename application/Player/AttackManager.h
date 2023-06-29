#pragma once
#include "IAttack.h"
#include <vector>
#include <map>
#include "mInput.h"
#include "FirstAttack.h"
#include "SecondAttack.h"
class AttackManager
{
private:
	Controller* controller_ = nullptr;
	//�U���̃f�[�^�E�v�[��
	std::map<std::string,std::unique_ptr<IAttack>> attacks_;

	std::unique_ptr<IAttack> nowAttack_;	//���ݏ������Ă���U��
	std::unique_ptr<IAttack> nextAttack_;	//���ɏ�������U��

	const size_t MAX_COMBO = 3;
	size_t comboNum;

	size_t timer_ = 0;
public:
	AttackManager();

	void Update();

	void DrawDebug();
};

