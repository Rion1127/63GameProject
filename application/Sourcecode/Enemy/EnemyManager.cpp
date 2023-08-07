#include "EnemyManager.h"
#include "EnemyShadow.h"
#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
#include "ParticleManager.h"
#include "mSound.h"


EnemyManager::EnemyManager()
{
	lockOnobjTimer_.SetLimitTime(360);
	lockOnobjTimer_.SetIsLoop(true);

	for (auto& lockOnSprite : lockOnSprite_)
	{
		lockOnSprite = std::move(std::make_unique<Sprite>());
		lockOnSprite->Ini("");
		lockOnSprite->SetScale({ 0.5f,0.5f });
	}
	lockOnSprite_[0]->SetTexture(TextureManager::GetInstance()->GetTexture("LockOn1"));
	lockOnSprite_[1]->SetTexture(TextureManager::GetInstance()->GetTexture("LockOn2"));
}

void EnemyManager::PreUpdate()
{
	lockOnEnemy_ = nullptr;
#ifdef _DEBUG

	ImGui::Begin("EnemyManager");
	if (ImGui::Button("POP_EnemyShadow"))
	{
		enemys_.emplace_back(std::move(std::make_unique<EnemyShadow>(Vector3(0, 3, 0))));
	}
	ImGui::End();
#endif // _DEBUG

	
	std::list<std::unique_ptr<IEnemy>>::iterator itr;
	for (itr = enemys_.begin(); itr != enemys_.end();)
	{
		//敵が死んだら要素を消して次のイテレータへ移る
		if (itr->get()->GetIsDead())
		{
			Vector3 pos = itr->get()->GetWorldTransform()->position_;
			ParticleManager::GetInstance()->
				AddParticle("EnemyDead", 32, 80, pos, { 0.8f,0.8f, 0.8f }, 1.0f);
			SoundManager::Play("DeathSE", false, 1.0f, 0.6f);
			itr = enemys_.erase(itr);

			lockOnEnemy_ = nullptr;
			player_->GetAttackManager()->SetLockOnEnemy(lockOnEnemy_);
			continue;
		}
		else
		{
			itr->get()->PreUpdate();
			//ロックオンしている敵のアドレスを代入
			if (itr->get()->GetIsSoftLockOn() ||
				itr->get()->GetIsHardLockOn())
			{
				lockOnEnemy_ = itr->get();
			}
		}

		if (itr->get()->GetIsBulletShot()) {
			itr->get()->BulletShot(&bullets_);
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

	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	//hpゲージにロックオン中の敵をセット
	hpGauge_.SetLockOnEnemy(lockOnEnemy_);

	hpGauge_.Update();
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

void EnemyManager::Damage()
{
	hpGauge_.Damage();
}

void EnemyManager::Reset()
{
	enemys_.clear();
	lockOnEnemy_ = nullptr;
}

void EnemyManager::LockOnSpriteUpdate()
{
	bool isHardLockOn = lockOnEnemy_->GetIsHardLockOn();
	lockOnobjTimer_.AddTime(1);

	lockOnobjRot++;

	if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_RIGHT_SHOULDER))
	{
		addRot = 10.f;
	}
	addRot -= 0.2f;
	addRot = Max(0.f, addRot);

	lockOnobjRot += addRot;

	lockOnSprite_[0]->SetRot(Radian(-lockOnobjRot));
	lockOnSprite_[1]->SetRot(Radian(lockOnobjRot));
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
			lockOnWT.position_.y + lockOnWT.scale_.y,
			lockOnWT.position_.z,
		};
		lockOnWT.Update();
		Vector2 pos = GetScreenPos(lockOnWT, *Camera::scurrent_);
		lockOnSprite_[i]->SetPos(pos);
		lockOnSprite_[i]->Update();
	}
}
