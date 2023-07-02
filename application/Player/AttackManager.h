#pragma once
#include "IAttack.h"
#include "IEnemy.h"
#include <vector>
#include <map>
#include "mInput.h"
#include "Attack1.h"
#include "Attack2.h"
#include "Attack3.h"

#include "AttackAir1.h"
#include "AttackAir2.h"
#include "AttackAir3.h"

class AttackManager
{
private:
	Controller* controller_ = nullptr;
	//�U���̃f�[�^�E�v�[��
	std::vector<std::unique_ptr<IAttack>> attacks_;

	std::unique_ptr<IAttack> nowAttack_;	//���ݏ������Ă���U��
	std::unique_ptr<IAttack> nextAttack_;	//���ɏ�������U��

	const size_t MAX_COMBO = 3;
	size_t comboNum;

	size_t timer_ = 0;

	bool isAttacking;
	IEnemy* lockOnEnemy_;
public:
	AttackManager();

	void Update();

	void DrawDebug();
public:
	bool GetIsAttacking() { return isAttacking; }
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
public:
	IAttack* GetNowAttack() { return nowAttack_.get(); }
};

