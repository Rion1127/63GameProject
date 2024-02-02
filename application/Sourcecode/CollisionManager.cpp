#include "CollisionManager.h"
#include "ParticleManager.h"
#include "mSound.h"
#include "ParticleWallHit.h"
#include "ParticleHitAttack.h"
#include "ParticleLanding.h"
#include <imgui.h>
#include "ParticleManager.h"
#include "SoundVolume.h"
#include "ComboCounter.h"

/**
 * @file CollisionManager.cpp
 * @brief 当たり判定をまとめているクラス
 */

CollisionManager::CollisionManager()
{
	wallHitTimer_.SetLimitTime(40);
	wallHitTimer_.SetTime(wallHitTimer_.GetLimitTimer());

	isHitStop_ = true;
	hitStopTimer_ = 3;

	shakePower_ = 0.3f;
	shakeTimer_ = 15;
}

void CollisionManager::Update()
{
	//ヒットストップ中に更新しないようにする;
	hitstop_.HitStopUpdate();
	if (hitstop_.GetIsHitStopping())return;
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
	//敵同士の押し出し
	EnemyToEnemy();
	//プレイヤーの攻撃と敵
	PlayerAttackToEnemy();
	//敵攻撃とプレイヤー
	EnemyAttackToPlayer();
	//敵弾とプレイヤー
	EnemyBulletToPlayer();
	//敵弾と床
	EnemyBulletToFloor();
}

void CollisionManager::DrawImGui()
{
	uint16_t isHitstop = isHitStop_;
	ImGui::Begin("CollisionManager");

	if (ImGui::Button("HitStop"))
	{
		isHitstop++;
		isHitstop = isHitstop & 1;
	}
	std::string flagstring;

	if (isHitstop)flagstring = "True";
	else flagstring = "False";

	ImGui::SameLine();
	ImGui::Text(flagstring.c_str());


	ImGui::DragFloat("hitStopTime", &hitStopTimer_, 1.f, 0, 100.f);

	ImGui::DragFloat("shakePower", &shakePower_, 0.1f, 0.1f, 5.f);
	ImGui::DragFloat("shakeTimer", &shakeTimer_, 1.f, 1, 100.f);

	ImGui::End();

	isHitStop_ = isHitstop;
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
				emitter->pos = interPos + (itr->get()->normal * -0.01f);
				emitter->addVec = rot;
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
					Vector3 pushBackVec = (floor->normal * dist) * enemy->GetCol().radius;
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
		else
		{
			enemy->SetIsFloorCollision(false);
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
		int32_t index = 0;
		for (auto& enemy : *enemyManager_->GetEnemy())
		{
			//一体でもハードロックをしていたらフラグをtrueにする
			if (enemy->GetIsHardLockOn())
			{
				isHardLockOn = true;
				lockOnEnemyIndex_ = index;
			}
			//ロックオン用の大きい当たり判定
			enemy->SetSoftIsLockOn(false);
			Sphere serchCol = player_->GetCol();
			serchCol.radius = 20.f;
			if (BallCollision(serchCol, enemy->GetCol()))
			{
				//ロックオン圏内にいる敵を追加していく
				lockOnEnemys_.push_back(enemy.get());
			}
			index++;
		}
		//ソフトロックの場合は常に計算する
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
		else {
			IEnemy* otherLockOnEnemy = nullptr;
			if (Controller::GetTriggerButtons(PAD::INPUT_LEFT_SHOULDER)) {
				//ロックオン範囲内に敵が1体以下なら計算をしない
				if (lockOnEnemys_.size() < 1)return;
				//ロックオンしている敵が1体でその敵がハードロックされている場合計算をしない
				if (lockOnEnemys_.size() == 1 &&
					lockOnEnemys_[0]->GetIsHardLockOn())return;
				//次の敵にロックオンを映す
				lockOnEnemyIndex_++;
				//配列一番後ろの敵を選択していたら先頭のインデックスにする
				if (lockOnEnemyIndex_ >= lockOnEnemys_.size()) {
					lockOnEnemyIndex_ = 0;
				}
				index = 0;
				for (auto& enemy : lockOnEnemys_)
				{
					//条件にあった敵をロックオンする
					if (index == lockOnEnemyIndex_) {
						otherLockOnEnemy = enemy;
						break;
					}
					index++;
				}

				if (otherLockOnEnemy != nullptr) {
					SoundManager::Play("lockOnSE", false, SoundVolume::GetValumeSE());
					enemyManager_->GetLockOnEnemy()->SetHardIsLockOn(false);
					otherLockOnEnemy->SetHardIsLockOn(true);
					otherLockOnEnemy->SetSoftIsLockOn(true);
				}
				player_->SetLockOnEnemy(otherLockOnEnemy);
			}
		}
	}

	if (enemyManager_->GetLockOnEnemy() != nullptr)
	{
		//自分でロックオンする敵を設定
		if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT_SHOULDER))
		{
			SoundManager::Play("lockOnSE", false, SoundVolume::GetValumeSE());
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

			Vector3 pushBackVec = (PtoEVec * backLength) / 2.f;

			player_->AddaddVec(pushBackVec);
			enemy->AddaddVec(-pushBackVec);
		}
	}
}

void CollisionManager::EnemyToEnemy()
{
	for (auto& enemy1 : *enemyManager_->GetEnemy())
	{
		for (auto& enemy2 : *enemyManager_->GetEnemy())
		{
			if (BallCollision(enemy2->GetCol(), enemy1->GetCol()))
			{
				Vector3 PtoEVec = enemy2->GetCol().center - enemy1->GetCol().center;
				//ベクトルの長さを取得
				float length = PtoEVec.length();
				PtoEVec = PtoEVec.normalize();
				//Y成分を無効にして押し出す
				PtoEVec.y = 0;
				//二つの当たり判定の半径の長さを足す
				float backLength = enemy2->GetCol().radius + enemy1->GetCol().radius;
				//ベクトルの長さを引いてめり込んでいる長さ分だけ押し戻す()
				backLength -= length;

				Vector3 pushBackVec = (PtoEVec * backLength) / 2.f;

				enemy2->AddaddVec(pushBackVec);
				enemy1->AddaddVec(-pushBackVec);
			}
		}
	}
}

void CollisionManager::PlayerAttackToEnemy()
{
	BaseAttack* attackCol = player_->GetAttackManager()->GetNowAttack();
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		//近接攻撃
		if (enemy->GetDamageCoolTime().GetIsEnd())
		{
			if (attackCol != nullptr)
			{
				if (BallCollision(attackCol->GetCol(), enemy->GetCol()))
				{
					//プレイヤーの反対方向にノックバックする
					Vector3 knockVec = enemy->GetCol().center - player_->GetWorldTransform()->position_;
					knockVec.y = attackCol->GetKnockYVec();
					knockVec = knockVec.normalize() * attackCol->GetKnockVec();
					//敵のノックバック抵抗力を掛ける
					knockVec = knockVec * enemy->GetKnockResist();
					enemy->Damage(knockVec,
						(int32_t)attackCol->GetDamage(),
						attackCol->GetDamageCoolTime());

					//HPゲージ反映
					enemyManager_->Damage();
					//コンボを加算する
					ComboCounter::GetInstance()->AddCombo();

					//ヒットパーティクル生成
					std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
					hitEmitter_->pos = enemy->GetCol().center;
					ParticleManager::GetInstance()->
						AddParticle("HitAttack", hitEmitter_);
					SoundManager::Play("HitSE", false, SoundVolume::GetValumeSE());

					//ヒットストップのフラグがオフだった場合 or フィニッシュ技以外はヒットストップしない
					if (attackCol->GetNowSwingAttackType() == AttackType::Normal)
					{
						enemy->SetIsNock(true);
						hitStopTimer_ = 2;
						shakePower_ = 0.3f;
						shakeTimer_ = 15;
						gameCamera_->SetCameraShake(shakeTimer_, shakePower_);
					}
					else
					{
						//フィニッシュ業をくらったらダウンする
						enemy->SetIsDown(true);
						hitStopTimer_ = 4;
						shakePower_ = 0.8f;
						shakeTimer_ = 25;
						gameCamera_->SetCameraShake(shakeTimer_, shakePower_);
					}
					if (isHitStop_ == false)continue;
					if (player_->GetIsHitStopping()) continue;
					player_->SetHitStopTimer(hitStopTimer_);
					enemy->SetHitStopTimer(hitStopTimer_);
					ParticleManager::GetInstance()->SetHitStopTimer(hitStopTimer_);
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

							SoundManager::Play("GuardHitSE", false, SoundVolume::GetValumeSE());
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

						std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
						hitEmitter_->pos = enemy->GetCol().center;
						ParticleManager::GetInstance()->
							AddParticle("HitAttack", hitEmitter_);
						SoundManager::Play("HitSE", false, SoundVolume::GetValumeSE());
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

				SoundManager::Play("GuardHitSE", false, SoundVolume::GetValumeSE());
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
			hitEmitter_->pos = player_->GetCol().center;
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
