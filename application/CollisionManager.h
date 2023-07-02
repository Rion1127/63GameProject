#pragma once
#include "Player.h"
#include "Floor.h"
#include "EnemyManager.h"
class CollisionManager
{
private:
	Player* player_;
	Floor* floor_;
	EnemyManager* enemyManager_;
public:
	void Update();

public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetFloor(Floor* floor) { floor_ = floor; }
	void SetEnemys(EnemyManager* enemy) { enemyManager_ = enemy; }
private:
	//�v���C���[�Ə�
	void PlayerToFloor();
	//�G�Ə�
	void EnemyToFloor();
	//�G���b�N�I��
	void EnemyLockOn();
	//�v���C���[�ƓG�����o��
	void PlayerToEnemy();
	//�v���C���[�U���ƓG
	void PlayerAttackToEnemy();
};

