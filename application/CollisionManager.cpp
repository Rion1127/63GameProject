#include "CollisionManager.h"

void CollisionManager::Update()
{
	//���ƃv���C���[
	PlayerToFloor();
	//���ƓG
	EnemyToFloor();
	//���b�N�I������G
	EnemyLockOn();
	//�v���C���[�ƓG�����o��
	PlayerToEnemy();
}

void CollisionManager::PlayerToFloor()
{
	//���ƃv���C���[
	if (Sphere2PlaneCol(player_->GetCol(), floor_->GetPlaneCol()))
	{
		//�n�ʂɂ߂荞�܂Ȃ��悤�ɉ����o������
		while (true)
		{
			float checkValue = 0.01f;
			Sphere col = player_->GetCol();
			col.center.y -= checkValue;
			col.radius = player_->GetCol().radius;
			//�n�ʂɓ������Ă������ɉ����o���Ă���
			if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
			{
				player_->floorColision();
				break;
			}

			Vector3 playerPos = player_->GetWorldTransform()->position_;
			float upDist = 0.01f;
			playerPos.y += upDist;
			player_->SetAddPos({ 0,upDist,0 });
			player_->ColPosUpdate();
		}

	}
}

void CollisionManager::EnemyToFloor()
{
	for (auto& enemy : *enemyManager_->GetEnemy()) {
		//���ƃv���C���[
		if (Sphere2PlaneCol(enemy->GetCol(), floor_->GetPlaneCol()))
		{
			//�n�ʂɂ߂荞�܂Ȃ��悤�ɉ����o������
			while (true)
			{
				float checkValue = 0.01f;
				Sphere col = enemy->GetCol();
				col.center.y -= checkValue;
				col.radius = enemy->GetCol().radius;
				//�n�ʂɓ������Ă������ɉ����o���Ă���
				if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
				{
					enemy->FloorColision();
					break;
				}

				Vector3* enemyPos = &enemy->GetWorldTransform()->position_;
				float upDist = 0.01f;
				enemyPos->y += upDist;

				enemy->ColPosUpdate();
			}
		}
	}
}

void CollisionManager::EnemyLockOn()
{
	IEnemy* lockOnEnemy = nullptr;
	float dist = 1000.f;
	for (auto& enemy : *enemyManager_->GetEnemy()) {
		//���b�N�I���p�̑傫�������蔻��
		enemy->SetIsLockOn(false);
		Sphere serchCol = player_->GetCol();
		serchCol.radius *= 20.f;
		if (BallCollision(serchCol, enemy->GetCol()))
		{
			Vector3 PtoEVec = serchCol.center - enemy->GetCol().center;
			float length = PtoEVec.length();
			//�x�N�g���̒����𑪂���dist�������������
			if (dist > length) {
				dist = length;
				lockOnEnemy = enemy.get();
			}
		}
	}

	if (lockOnEnemy != nullptr) {
		lockOnEnemy->SetIsLockOn(true);
	}
	enemyManager_->SetLockOnEnemy(lockOnEnemy);
	//AttackManager��lockOnEnemy�̃A�h���X��n��
	player_->SetLockOnEnemy(lockOnEnemy);

}

void CollisionManager::PlayerToEnemy()
{
	for (auto& enemy : *enemyManager_->GetEnemy()) {
		if (BallCollision(player_->GetCol(), enemy->GetCol()))
		{
			Vector3 PtoEVec = player_->GetCol().center - enemy->GetCol().center;
			float length = PtoEVec.length();
			PtoEVec.normalize();
			//Y�����𖳌��ɂ��ĉ����o��
			PtoEVec.y = 0;
			float backLength = player_->GetCol().radius + enemy->GetCol().radius;
			backLength -= length * 0.97f;
			
			player_->AddPos(PtoEVec * backLength);
		}
	}
}
