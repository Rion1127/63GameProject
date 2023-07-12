#include "CollisionManager.h"
#include "ParticleManager.h"
#include "mSound.h"

void CollisionManager::Update()
{
	//床とプレイヤー
	PlayerToFloor();
	//床と敵
	EnemyToFloor();
	//ロックオンする敵
	EnemyLockOn();
	//プレイヤーと敵押し出し
	PlayerToEnemy();
	//プレイヤーの攻撃と敵
	PlayerAttackToEnemy();
}

void CollisionManager::PlayerToFloor()
{
	//床とプレイヤー
	if (Sphere2PlaneCol(player_->GetCol(), floor_->GetPlaneCol()))
	{
		//地面にめり込まないように押し出し処理
		while (true)
		{
			float checkValue = 0.01f;
			Sphere col = player_->GetCol();
			col.center.y -= checkValue;
			col.radius = player_->GetCol().radius;
			//地面に当たっていたら上に押し出していく
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
	else
	{
		player_->SetIsFloorCollision(false);
	}
}

void CollisionManager::EnemyToFloor()
{
	for (auto& enemy : *enemyManager_->GetEnemy()) {
		//床と敵
		if (Sphere2PlaneCol(enemy->GetCol(), floor_->GetPlaneCol()))
		{
			//地面にめり込まないように押し出し処理
			while (true)
			{
				float checkValue = 1.5f;
				Sphere col = enemy->GetCol();
				col.center.y -= checkValue;
				col.radius = enemy->GetCol().radius;
				//地面に当たっていたら上に押し出していく
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
	std::vector<IEnemy*> lockOnEnemys_;
	float dist = 1000.f;
	
	for (auto& enemy : *enemyManager_->GetEnemy()) {
		//ロックオン用の大きい当たり判定
		enemy->SetIsLockOn(false);
		Sphere serchCol = player_->GetCol();
		serchCol.radius *= 15.f;
		if (BallCollision(serchCol, enemy->GetCol()))
		{
			lockOnEnemys_.push_back(enemy.get());
			Vector3 PtoEVec = serchCol.center - enemy->GetCol().center;
			float length = PtoEVec.length();
			//ベクトルの長さを測ってdistよりも小さければ
			if (dist > length) {
				dist = length;
				
				
			}
		}
	}
	float dist2d = 1000.f;
	for (auto& enemy : lockOnEnemys_)
	{
		Vector2 ScreenPos = TransformToVec2(*enemy->GetWorldTransform(), *Camera::scurrent_);

		Vector2 halfWindowSize = WinAPI::GetWindowSize() / 2.f;

		Vector2 dist = halfWindowSize - ScreenPos;
		float length = dist.length();
		if (dist2d > length)
		{
			dist2d = length;
			lockOnEnemy = enemy;
		}
	}

	if (lockOnEnemy != nullptr) {
		lockOnEnemy->SetIsLockOn(true);
	}
	enemyManager_->SetLockOnEnemy(lockOnEnemy);
	//AttackManagerへlockOnEnemyのアドレスを渡す
	player_->SetLockOnEnemy(lockOnEnemy);

}

void CollisionManager::PlayerToEnemy()
{
	for (auto& enemy : *enemyManager_->GetEnemy()) {
		if (BallCollision(player_->GetCol(), enemy->GetCol()))
		{
			Vector3 PtoEVec = player_->GetCol().center - enemy->GetCol().center;
			//ベクトルの長さを取得
			float length = PtoEVec.length();
			PtoEVec.normalize();
			//Y成分を無効にして押し出す
			PtoEVec.y = 0;
			//二つの当たり判定の半径の長さを足す
			float backLength = player_->GetCol().radius + enemy->GetCol().radius;
			//ベクトルの長さを引いてめり込んでいる長さ分だけ押し戻す()
			backLength -= length;

			player_->SetAddPos(PtoEVec * backLength);
		}
	}
}

void CollisionManager::PlayerAttackToEnemy()
{
	IAttack* attackCol = player_->GetAttackManager()->GetNowAttack();
	if (attackCol != nullptr) {
		for (auto& enemy : *enemyManager_->GetEnemy()) {
			for (auto& col : *attackCol->GetAttackCol()) {
				if (enemy->GetDamageCoolTime().GetIsEnd()) {
					if (BallCollision(col->col_, enemy->GetCol()))
					{
						//プレイヤーの反対方向にノックバックする
						Vector3 knockVec = enemy->GetCol().center - player_->GetWorldTransform()->position_;
						knockVec.y = col->knockVecY;
						knockVec.normalize();
						knockVec = knockVec * col->knockPower;
						//敵のノックバック抵抗力を掛ける
						knockVec = knockVec * enemy->GetKnockResist();
						enemy->HitPlayerAttack(knockVec, col->damage, col->damageCoolTime);

						Vector3 addVec = { 0.15f,0.15f,0.15f };
	
						ParticleManager::GetInstance()->AddParticle("HitAttack", 3, 40, enemy->GetCol().center, addVec, 0.7f);
						SoundManager::Play("HitSE", false, 0.2f);
					}
				}
			}
		}
	}
}
