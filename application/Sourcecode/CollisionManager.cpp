#include "CollisionManager.h"
#include "ParticleManager.h"
#include "mSound.h"
#include "ParticleWallHit.h"
#include "ParticleHitAttack.h"

CollisionManager::CollisionManager()
{
	wallHitTimer_.SetLimitTime(40);
	wallHitTimer_.SetTime(wallHitTimer_.GetLimitTimer());
}

void CollisionManager::Update()
{
	//床とプレイヤー
	PlayerToFloor();
	PlayerToWall();
	//床と敵
	EnemyToFloor();
	//敵と壁
	EnemyToWall();
	//ロックオンする敵
	EnemyLockOn();
	//プレイヤーと敵押し出し
	PlayerToEnemy();
	//プレイヤーの攻撃と敵
	PlayerAttackToEnemy();
	//敵攻撃とプレイヤー
	EnemyAttackToPlayer();
	//敵弾とプレイヤー
	EnemyBulletToPlayer();
	//敵弾と床
	EnemyBulletToFloor();
}

void CollisionManager::PlayerToFloor()
{
	Plane* floor = stage_->GetFloor();
	Sphere col = player_->GetCol();
	col.center += player_->GetGravity()->GetGravityValue();

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
			player_->AddaddVec(pushBackVec);

			//パーティクル
			wallHitTimer_.AddTime(1);
			if (wallHitTimer_.GetIsEnd()) {
				Vector3 rot{};
				if (itr->get()->normal.x != 0) {
					rot = { 0,Radian(90),0 };
				}
				std::shared_ptr<OneceEmitter> emitter = std::make_shared<OneceEmitter>();
				emitter->particle = std::make_unique<ParticleWallHit>();
				emitter->addNum = 1;
				emitter->time = 60;
				emitter->pos = interPos + (itr->get()->normal * -0.01f);
				emitter->addVec = rot;
				emitter->scale = 1.0f;

				ParticleManager::GetInstance()->
					AddParticle("WallHit", emitter);
				wallHitTimer_.Reset();
			}
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

void CollisionManager::EnemyToWall()
{
	auto* walls = stage_->GetPlaneCol();

	//壁とプレイヤー
	for (auto itr = walls->begin(); itr != walls->end(); ++itr) {
		for (auto& enemy : *enemyManager_->GetEnemy())
		{
			Vector3 interPos;
			Sphere col = enemy->GetCol();
			Vector3 addvec = enemy->GetAddVec();
			col.center += addvec;
			if (Sphere2PlaneCol(col, *itr->get(), &interPos))
			{
				//めり込まないよう処理
				Vector3 colEdgePos =
					col.center +
					(col.radius * -itr->get()->normal);

				Vector3 pushBackVec = interPos - colEdgePos;
				enemy->AddaddVec(pushBackVec);
			}
		}
	}
}

void CollisionManager::EnemyLockOn()
{
	IEnemy* lockOnEnemy = nullptr;
	std::vector<IEnemy*> lockOnEnemys_;
	bool isHardLockOn = false;
	//攻撃をしていない、又は、ロックオンしている敵がいない時にロックオン対象を探す
	if (player_->GetAttackManager()->GetIsAttacking() == false ||
		enemyManager_->GetLockOnEnemy() == nullptr) {
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
			player_->SetLockOnEnemy(lockOnEnemy);
		}
	}

	if (enemyManager_->GetLockOnEnemy() != nullptr)
	{
		//自分でロックオンする敵を設定
		if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT_SHOULDER))
		{
			SoundManager::Play("lockOnSE", false, 0.5f);
			//ロックオンしていれば解除、していなければロックオン
			bool isLockOn = (enemyManager_->GetLockOnEnemy()->GetIsHardLockOn() == true) ?
				false : true;
			enemyManager_->GetLockOnEnemy()->SetHardIsLockOn(isLockOn);
		}
	}
	player_->SetLockOnEnemy(enemyManager_->GetLockOnEnemy());
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
			enemy->AddaddVec(-pushBackVec);
		}
	}
}

void CollisionManager::PlayerAttackToEnemy()
{
	IAttack* attackCol = player_->GetAttackManager()->GetNowAttack();
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		if (attackCol != nullptr)
		{
			//近接攻撃
			for (auto& col : *attackCol->GetAttackCol())
			{
				if (enemy->GetDamageCoolTime().GetIsEnd())
				{
					if (BallCollision(col->col_, enemy->GetCol()))
					{
						col->isCollision_ = true;
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

						Vector3 addVec = { 0.05f,0.05f,0.05f };

						std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
						hitEmitter_->particle = std::make_unique<ParticleHitAttack>();
						hitEmitter_->addNum = 3;
						hitEmitter_->time = 40;
						hitEmitter_->pos = enemy->GetCol().center;
						hitEmitter_->addVec = addVec;
						hitEmitter_->scale = 1.0f;
						ParticleManager::GetInstance()->
							AddParticle("HitAttack", hitEmitter_);
						SoundManager::Play("HitSE", false, 0.4f);
					}
				}
			}
		}

		auto& bullets = *player_->GetMagicManager()->GetBullet();

		if (bullets.size() == 0)continue;
		//魔法攻撃
		for (auto& bullet : bullets)
		{

			if (enemy->GetDamageCoolTime().GetIsEnd())
			{
				if (BallCollision(bullet->GetAttackCol()->get()->col_, enemy->GetCol()))
				{
					//プレイヤーの反対方向にノックバックする
					Vector3 knockVec = enemy->GetCol().center - player_->GetWorldTransform()->position_;
					knockVec.y = bullet->GetAttackCol()->get()->knockVecY;
					knockVec = knockVec.normalize();
					knockVec = knockVec * bullet->GetAttackCol()->get()->knockPower;
					//敵のノックバック抵抗力を掛ける
					knockVec = knockVec * enemy->GetKnockResist();
					enemy->Damage(knockVec, bullet->GetAttackCol()->get()->damage, bullet->GetAttackCol()->get()->damageCoolTime);
					enemy->SetIsNock(true);
					//HPゲージ反映
					enemyManager_->Damage();

					bullet->SetIsDead(true);

					Vector3 addVec = { 0.05f,0.05f,0.05f };
					std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
					hitEmitter_->particle = std::make_unique<ParticleHitAttack>();
					hitEmitter_->addNum = 3;
					hitEmitter_->time = 40;
					hitEmitter_->pos = enemy->GetCol().center;
					hitEmitter_->addVec = addVec;
					hitEmitter_->scale = 1.0f;
					ParticleManager::GetInstance()->
						AddParticle("HitAttack", hitEmitter_);
					SoundManager::Play("HitSE", false, 0.5f);
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

							SoundManager::Play("GuardHitSE", false, 0.5f);
						}
					}

					if (BallCollision(col->col_, player_->GetDamageCol()))
					{
						//敵の反対方向にノックバックする
						Vector3 knockVec =
							player_->GetWorldTransform()->position_ - enemy->GetCol().center;
						knockVec.y = col->knockVecY;
						knockVec = knockVec.normalize();
						knockVec = knockVec * col->knockPower;

						player_->Damage(col->damage, knockVec);

						Vector3 addVec = { 0.15f,0.15f,0.15f };

						std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
						hitEmitter_->particle = std::make_unique<ParticleHitAttack>();
						hitEmitter_->addNum = 3;
						hitEmitter_->time = 40;
						hitEmitter_->pos = enemy->GetCol().center;
						hitEmitter_->addVec = addVec;
						hitEmitter_->scale = 1.0f;
						ParticleManager::GetInstance()->
							AddParticle("HitAttack", hitEmitter_);
						SoundManager::Play("HitSE", false, 0.5f);
					}
				}
			}
		}
	}
}

void CollisionManager::EnemyBulletToPlayer()
{
	for (auto& bullet : *enemyManager_->GetBullet())
	{
		//ガードの当たり判定
		if (player_->GetGuard()->GetCol().isActive) {
			if (BallCollision(bullet->GetAttackCol()->get()->col_, player_->GetGuard()->GetCol())) {
				//プレイヤーのダメージクールタイムが終わっていれば
				if (player_->GetDamegeCoolTime()->GetIsEnd()) {
					//敵の反対方向にノックバックする
					Vector3 knockVec =
						player_->GetWorldTransform()->position_ - bullet->GetAttackCol()->get()->col_.center;
					knockVec.y = 0;
					knockVec = knockVec.normalize();
					knockVec = knockVec * 0.1f;

					player_->GuardHit(knockVec);
				}
				bullet->SetIsDead(true);

				SoundManager::Play("GuardHitSE", false, 0.5f);
			}
		}

		if (BallCollision(bullet->GetAttackCol()->get()->col_, player_->GetDamageCol()))
		{
			//プレイヤーのダメージクールタイムが終わっていれば
			if (player_->GetDamegeCoolTime()->GetIsEnd()) {
				//敵の反対方向にノックバックする
				Vector3 knockVec =
					player_->GetWorldTransform()->position_ - bullet->GetAttackCol()->get()->col_.center;
				knockVec.y = bullet->GetAttackCol()->get()->knockVecY;
				knockVec = knockVec.normalize();
				knockVec = knockVec * bullet->GetAttackCol()->get()->knockPower;

				player_->Damage(bullet->GetAttackCol()->get()->damage, knockVec);
				player_->SetState(PlayerState::Knock);
			}
			bullet->SetIsDead(true);

			std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
			hitEmitter_->particle = std::make_unique<ParticleHitAttack>();
			hitEmitter_->addNum = 3;
			hitEmitter_->time = 40;
			hitEmitter_->pos = player_->GetCol().center;
			hitEmitter_->addVec = { 0.15f,0.15f,0.15f };;
			hitEmitter_->scale = 1.0f;
			ParticleManager::GetInstance()->
				AddParticle("HitAttack", hitEmitter_);
		}
	}
}

void CollisionManager::EnemyBulletToFloor()
{
	Plane* floor = stage_->GetFloor();
	for (auto& bullet : *enemyManager_->GetBullet())
	{
		Sphere col = bullet->GetAttackCol()->get()->col_;
		//床とプレイヤー
		if (Sphere2PlaneCol(col, *floor))
		{
			bullet->SetIsDead(true);
		}
	}
}
