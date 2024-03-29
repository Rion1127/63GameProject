#include "EnemyManager.h"
#include "EnemyShadow.h"
#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
#include "ParticleManager.h"
#include "mSound.h"
#include "ParticleEnemyDead.h"
#include "EnemyRedNocturne.h"
#include "IBullet.h"
#include "SoundVolume.h"
#include "Easing.h"

/**
 * @file EnemyManager.cpp
 * @brief 全ての敵を管理するクラス
 */

EnemyManager::EnemyManager()
{
	lockOnobjTimer_.SetLimitTime(20);
	
	for (auto& lockOnSprite : lockOnSprite_)
	{
		lockOnSprite = std::move(std::make_unique<Sprite>());
		lockOnSprite->Ini("");
		lockOnSprite->SetScale({ 0.3f,0.3f });
	}
	lockOnSprite_[0]->SetTexture(TextureManager::GetInstance()->GetTexture("LockOn1"));
	lockOnSprite_[1]->SetTexture(TextureManager::GetInstance()->GetTexture("LockOn2"));
	rockOnScaleTimer_.SetLimitTime(80);
}

void EnemyManager::PreColUpdate()
{
	lockOnEnemy_ = nullptr;
#ifdef _DEBUG

#endif // _DEBUG

	hitStop_.HitStopUpdate();
	if (hitStop_.GetIsHitStopping())return;

	std::list<std::unique_ptr<IEnemy>>::iterator itr;
	for (itr = enemys_.begin(); itr != enemys_.end();)
	{
		//敵が死んだら要素を消して次のイテレータへ移る
		if ((*itr)->GetIsDead())
		{
			Vector3 pos = (*itr)->GetWorldTransform()->position_;

			itr = enemys_.erase(itr);

			lockOnEnemy_ = nullptr;
			player_->SetLockOnEnemy(lockOnEnemy_);
			player_->GetAttackManager()->SetLockOnEnemy(lockOnEnemy_);
			std::shared_ptr<OneceEmitter> deadEmitter_ = std::make_shared<OneceEmitter>();
			deadEmitter_->pos = pos;
			ParticleManager::GetInstance()->
				AddParticle("EnemyDead", deadEmitter_);
			SoundManager::Play("DeathSE", false, SoundVolume::GetValumeSE(), 0.6f);
			continue;
		}
		else
		{
			(*itr)->PreUpdate();
			//ロックオンしている敵のアドレスを代入
			if ((*itr)->GetIsSoftLockOn() ||
				(*itr)->GetIsHardLockOn())
			{
				lockOnEnemy_ = itr->get();
			}
		}

		if ((*itr)->GetIsBulletShot()) {
			(*itr)->BulletShot(&bullets_);
		}

		itr++;
	}

	if (lockOnEnemy_ != nullptr)
	{
		//ロックオンスプライト更新
		LockOnSpriteUpdate();
	}
	else
	{
		//ロックオンできる敵がいなければUIを消す
		for (auto& lockOnSprite : lockOnSprite_)
		{
			lockOnSprite->SetInvisivle(true);
		}
	}

	BulletUpdate();

	//hpゲージにロックオン中の敵をセット
	hpGauge_.SetLockOnEnemy(lockOnEnemy_);
	hpGauge_.Update();
	rockOnParticle_.Update();
}

void EnemyManager::PostUpdate()
{
	for (auto& enemy : enemys_)
	{
		enemy->PostUpdate();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}

	for (auto& bullet : bullets_) {
		bullet->DrawCol();
	}
}

void EnemyManager::SpriteDraw()
{
	for (auto& lockOnSprite : lockOnSprite_)
	{
		lockOnSprite->Draw();
	}
	hpGauge_.Draw();
}

void EnemyManager::SpriteBackDraw()
{
	rockOnParticle_.Draw();
}

void EnemyManager::Damage()
{
	hpGauge_.Damage();
}

void EnemyManager::BulletUpdate()
{
	std::list<std::unique_ptr<IBullet>>::iterator itr;
	for (itr = bullets_.begin(); itr != bullets_.end();) {

		if ((*itr)->GetIsDead()) {
			itr = bullets_.erase(itr);
			continue;
		}

		(*itr)->Update();

		itr++;
	}
}

void EnemyManager::Reset()
{
	enemys_.clear();
	lockOnEnemy_ = nullptr;
}

void EnemyManager::DebugUpdate()
{

	ImGui::Begin("EnemyManager");

	Vector3 popPos;

	static float dragPos[3] = {};

	ImGui::DragFloat3("popPos", dragPos, 0.1f, -30, 30);

	popPos = { dragPos[0],dragPos[1],dragPos[2] };

	if (ImGui::Button("POP_Dummy"))
	{
		enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(popPos)));
	}
	if (ImGui::Button("AirDummy"))
	{
		enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(0, 3, 0))));
	}
	if (ImGui::Button("POP_EnemyShadow"))
	{
		enemys_.emplace_back(std::move(std::make_unique<EnemyShadow>(Vector3(0, 3, 0), Vector3(0, Radian(180), 0))));
	}
	if (ImGui::Button("POP_RedNocturne"))
	{
		enemys_.emplace_back(std::move(std::make_unique<EnemyRedNocturne>(Vector3(0, 4, 0), Vector3(0, Radian(180), 0))));
	}

	if (ImGui::Button("Reset"))
	{
		enemys_.clear();
		lockOnEnemy_ = nullptr;
		player_->SetLockOnEnemy(lockOnEnemy_);
		player_->GetAttackManager()->SetLockOnEnemy(lockOnEnemy_);
	}
	ImGui::End();
}

void EnemyManager::LockOnSpriteUpdate()
{
	bool isHardLockOn = lockOnEnemy_->GetIsHardLockOn();
	lockOnobjTimer_.AddTime(1);

	if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT_SHOULDER))
	{
		lockOnobjTimer_.Reset();
		addRot = 10.f;
		rockOnScale_ = { 2.f,2.f };
		if (isHardLockOn == false)
		{
			rockOnParticle_.Reset();
			rockOnParticle_.SetIsStart(true);
		}
		else {
			rockOnParticle_.Reset();
		}
	}
	addRot -= 0.2f;
	addRot = Max(0.f, addRot);
	lockOnobjRot += addRot;
	lockOnobjRot++;

	rockOnScale_ = {
		Easing::Circ::easeIn(0.6f,0.3f,lockOnobjTimer_.GetTimeRate()),
		Easing::Circ::easeIn(0.6f,0.3f,lockOnobjTimer_.GetTimeRate())
	};

	lockOnSprite_[0]->SetRot(Radian(-lockOnobjRot));
	lockOnSprite_[1]->SetRot(Radian(lockOnobjRot));
	lockOnSprite_[0]->SetScale(rockOnScale_);
	lockOnSprite_[1]->SetScale(rockOnScale_);
	for (size_t i = 0; i < 2; i++)
	{
		//ハードロック
		if (isHardLockOn)
		{
			lockOnSprite_[i]->SetInvisivle(false);
			lockOnSprite_[i]->SetColor(Color(40, 30, 240, 255));
		}
		//ソフトロック
		else
		{
			lockOnSprite_[0]->SetInvisivle(true);
			lockOnSprite_[1]->SetInvisivle(false);
			lockOnSprite_[i]->SetColor(Color(240, 175, 30, 255));
		}

		WorldTransform lockOnWT = *lockOnEnemy_->GetWorldTransform();
		lockOnWT.position_ = {
			lockOnWT.position_.x,
			lockOnWT.position_.y,
			lockOnWT.position_.z,
		};

		
		lockOnWT.position_.y += lockOnEnemy_->GetWorldTransform()->scale_.y;
		

		lockOnWT.Update();
		Vector2 pos = GetScreenPos(lockOnWT, *Camera::scurrent_);
		lockOnSprite_[i]->SetPos(pos);
		lockOnSprite_[i]->Update();
	}

	if (isHardLockOn)
	{
		if (Controller::GetTriggerButtons(PAD::INPUT_LEFT_SHOULDER))
		{
			rockOnParticle_.Reset();
			rockOnParticle_.SetIsStart(true);
			lockOnobjTimer_.Reset();
		}
	}
	if (rockOnParticle_.GetIsStaert()) {
		//スクリーン座標を取得して画面の中央に近い敵をロックオンする
		WorldTransform wt = *player_->GetWorldTransform();
		wt.position_.y += wt.scale_.y;
		wt.Update();
		Vector2 ScreenPos = GetScreenPos(wt, *Camera::scurrent_);
		rockOnParticle_.SetStartPos(ScreenPos);
		rockOnParticle_.SetEndPos(lockOnSprite_[0]->GetPos());
	}
}
