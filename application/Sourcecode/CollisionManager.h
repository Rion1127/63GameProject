#pragma once
#include "Player.h"
#include "Stage.h"
#include "EnemyManager.h"
class CollisionManager
{
private:
	Player* player_;
	Stage* stage_;
	EnemyManager* enemyManager_;
public:
	void Update();

public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetFloor(Stage* floor) { stage_ = floor; }
	void SetEnemys(EnemyManager* enemy) { enemyManager_ = enemy; }
private:
	//�v���C���[�Ə�
	void PlayerToFloor();
	//�v���C���[�ƕ�
	void PlayerToWall();
	//�G�Ə�
	void EnemyToFloor();
	//�G�Ə�
	void EnemyToWall();
	//�G���b�N�I��
	void EnemyLockOn();
	//�v���C���[�ƓG�����o��
	void PlayerToEnemy();
	//�v���C���[�U���ƓG
	void PlayerAttackToEnemy();
	//�G�U���ƃv���C���[
	void EnemyAttackToPlayer();
};

