#include "CollisionManager.h"
#include "ParticleManager.h"
#include "mSound.h"

void CollisionManager::Update()
{
	//床とプレイヤー
	PlayerToFloor();
	PlayerToWall();
	//床と敵
	EnemyToFloor();
	//ロックオンする敵
	EnemyLockOn();
	//プレイヤーと敵押し出し
	PlayerToEnemy();
	//プレイヤーの攻撃と敵
	PlayerAttackToEnemy();
	//敵攻撃とプレイヤー
	EnemyAttackToPlayer();
}

void CollisionManager::PlayerToFloor()
{
	Plane* floor = stage_->GetFloor();
	Sphere col = player_->GetCol();
	col.center += player_->GetAddVec();

	//床とプレイヤー
	if (Sphere2PlaneCol(col, *floor))
	{
		//地面にめり込まないよう処理
		while (true)
		{
			float dist = player_->GetCol().center.y - floor->distance;
			if (player_->GetCol().radius <= dist)
			{
				Vector3 pushBackVec = floor->normal * dist;
				player_->GetWorldTransform()->position_ -= pushBackVec;
			}
			player_->ColPosUpdate();

			Vector3 pos = { 10,10,10 };
			if (Sphere2PlaneCol(player_->GetCol(), *floor, &pos))
			{
				player_->GetGravity()->SetGrabity({ 0,0,0 });
				player_->FloorColision(pos);
				break;
			}
		}
	}
	else
	{
		player_->SetIsFloorCollision(false);
	}

}

void CollisionManager::PlayerToWall()
{
	auto* walls = stage_->GetPlaneCol();

	//壁とプレイヤー
	for (auto itr = walls->begin(); itr != walls->end(); ++itr) {
		Vector3 interPos;
		Sphere col = player_->GetCol();
		Vector3 addvec = player_->GetAddVec();
		col.center += addvec;
		if (Sphere2PlaneCol(col, *itr->get(), &interPos))
		{
			//めり込まないよう処理
			Vector3 colEdgePos =
				col.center +
				(col.radius * -itr->get()->normal);

			Vector3 pushBackVec = interPos - colEdgePos;
			player_->AddPos(pushBackVec);
		}
	}
}

void CollisionManager::EnemyToFloor()
{
	Plane* floor = stage_->GetFloor();
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		Sphere col = enemy->GetCol();
		col.center += enemy->GetAddVec();
		//床とプレイヤー
		if (Sphere2PlaneCol(col, *floor))
		{
			//地面にめり込まないよう処理
			while (true)
			{
				float dist = enemy->GetCol().center.y - floor->distance;
				if (enemy->GetCol().radius <= dist)
				{
					Vector3 pushBackVec = floor->normal * dist * 0.1f;
					enemy->GetWorldTransform()->position_ -= pushBackVec;
				}
				enemy->ColPosUpdate();


				Vector3 pos = { 10,10,10 };
				if (Sphere2PlaneCol(enemy->GetCol(), *floor, &pos))
				{
					enemy->GetGravity().SetGrabity({ 0,0,0 });
					enemy->FloorColision(pos);
					break;
				}
			}
		}
	}
}

void CollisionManager::EnemyLockOn()
{
	IEnemy* lockOnEnemy = nullptr;
	std::vector<IEnemy*> lockOnEnemys_;
	bool isHardLockOn = false;

	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		if (enemy->GetIsSoftLockOn() == false)
		{
			enemy->SetHardIsLockOn(false);
		}
		//一体でもハードロックをしていたらフラグをtrueにする
		if (enemy->GetIsHardLockOn())
		{
			isHardLockOn = true;
			break;
		}
		//ロックオン用の大きい当たり判定
		enemy->SetSoftIsLockOn(false);
		Sphere serchCol = player_->GetCol();
		serchCol.radius *= 20.f;
		if (BallCollision(serchCol, enemy->GetCol()))
		{
			//ロックオン圏内にいる敵を追加していく
			lockOnEnemys_.push_back(enemy.get());
		}
	}
	if (isHardLockOn == false)
	{
		//仮の大きい値を入れておく
		float dist2d = 5000.f;
		for (auto& enemy : lockOnEnemys_)
		{
			//スクリーン座標を取得して画面の中央に近い敵をロックオンする
			Vector2 ScreenPos = GetScreenPos(*enemy->GetWorldTransform(), *Camera::scurrent_);

			Vector2 halfWindowSize = WinAPI::GetWindowSize() / 2.f;

			Vector2 dist = halfWindowSize - ScreenPos;
			float length = dist.length();
			//比較して短かったら敵を代入する
			if (dist2d > length)
			{
				dist2d = length;
				lockOnEnemy = enemy;
			}
		}

		if (lockOnEnemy != nullptr)
		{
			lockOnEnemy->SetSoftIsLockOn(true);
		}
		player_->GetAttackManager()->SetLockOnEnemy(lockOnEnemy);
	}

	if (enemyManager_->GetLockOnEnemy() != nullptr)
	{
		//自分でロックオンする敵を設定
		if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_RIGHT_SHOULDER))
		{
			SoundManager::Play("lockOnSE", false, 0.5f);
			//ロックオンしていれば解除、していなければロックオン
			bool isLockOn = (enemyManager_->GetLockOnEnemy()->GetIsHardLockOn() == true) ?
				false : true;
			enemyManager_->GetLockOnEnemy()->SetHardIsLockOn(isLockOn);

			player_->GetAttackManager()->SetLockOnEnemy(enemyManager_->GetLockOnEnemy());
		}
	}
}

void CollisionManager::PlayerToEnemy()
{
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		if (BallCollision(player_->GetCol(), enemy->GetCol()))
		{
			Vector3 PtoEVec = player_->GetCol().center - enemy->GetCol().center;
			//ベクトルの長さを取得
			float length = PtoEVec.length();
			PtoEVec = PtoEVec.normalize();
			//Y成分を無効にして押し出す
			PtoEVec.y = 0;
			//二つの当たり判定の半径の長さを足す
			float backLength = player_->GetCol().radius + enemy->GetCol().radius;
			//ベクトルの長さを引いてめり込んでいる長さ分だけ押し戻す()
			backLength -= length;

			Vector3 pushBackVec = (PtoEVec * backLength);

			player_->SetAddPos(pushBackVec);
			enemy->AddVec(-pushBackVec);
		}
	}
}

void CollisionManager::PlayerAttackToEnemy()
{
	IAttack* attackCol = player_->GetAttackManager()->GetNowAttack();
	if (attackCol != nullptr)
	{
		for (auto& enemy : *enemyManager_->GetEnemy())
		{
			for (auto& col : *attackCol->GetAttackCol())
			{
				if (enemy->GetDamageCoolTime().GetIsEnd())
				{
					if (BallCollision(col->col_, enemy->GetCol()))
					{
						//プレイヤーの反対方向にノックバックする
						Vector3 knockVec = enemy->GetCol().center - player_->GetWorldTransform()->position_;
						knockVec.y = col->knockVecY;
						knockVec = knockVec.normalize();
						knockVec = knockVec * col->knockPower;
						//敵のノックバック抵抗力を掛ける
						knockVec = knockVec * enemy->GetKnockResist();
						enemy->Damage(knockVec, col->damage, col->damageCoolTime);
						enemy->SetIsNock(true);
						//HPゲージ反映
						enemyManager_->Damage();

						Vector3 addVec = { 0.15f,0.15f,0.15f };

						ParticleManager::GetInstance()->AddParticle("HitAttack", 3, 40, enemy->GetCol().center, addVec, 0.7f);
						SoundManager::Play("HitSE", false, 0.5f);
					}
				}
			}
		}
	}
}

void CollisionManager::EnemyAttackToPlayer()
{
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		IAttack* attackCol = enemy->GetAttack();
		if (attackCol != nullptr)
		{
			for (auto& col : *attackCol->GetAttackCol())
			{
				//プレイヤーのダメージクールタイムが終わっていれば
				if (player_->GetDamegeCoolTime()->GetIsEnd()) {
					
					//ガードの当たり判定
					if (player_->GetGuard()->GetCol().isActive) {
						if (BallCollision(col->col_, player_->GetGuard()->GetCol())) {
							//敵の反対方向にノックバックする
							Vector3 knockVec =
								player_->GetWorldTransform()->position_ - enemy->GetCol().center;
							knockVec.y = 0;
							knockVec = knockVec.normalize();
							knockVec = knockVec * 0.1f;

							player_->GuardHit(knockVec);

							SoundManager::Play("GuardHitSE",false,0.5f);
						}
					}

					//
					if (BallCollision(col->col_, player_->GetCol()))
					{
						//敵の反対方向にノックバックする
						Vector3 knockVec =
							player_->GetWorldTransform()->position_ - enemy->GetCol().center;
						knockVec.y = col->knockVecY;
						knockVec = knockVec.normalize();
						knockVec = knockVec * col->knockPower;
						
						player_->Damage(col->damage, knockVec);
						player_->SetState(PlayerState::Knock);

						Vector3 addVec = { 0.15f,0.15f,0.15f };
						ParticleManager::GetInstance()->
							AddParticle("HitAttack", 3, 40, player_->GetCol().center, addVec, 0.7f);
						SoundManager::Play("HitSE", false, 0.5f);
					}
				}
			}
		}
	}
}
