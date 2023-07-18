#include "EnemyManager.h"
#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
#include "EnemyShadow.h"

EnemyManager::EnemyManager()
{
	lockOnobjTimer_.SetLimitTime(360);
	lockOnobjTimer_.SetIsLoop(true);
	/*enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, -20))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 20))));*/
	/*enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 1, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(10, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-10, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 10))));

	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(5, 3, -10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(10, 3, -10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(-5, 3, -10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(-10, 3, -10))));*/
	enemys_.emplace_back(std::move(std::make_unique<EnemyShadow>(Vector3(0, 3, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(2, 3, 0))));

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
	
	std::list<std::unique_ptr<IEnemy>>::iterator itr;
	for (itr = enemys_.begin(); itr != enemys_.end();)
	{
		//敵が死んだら要素を消して次のイテレータへ移る
		if (itr->get()->GetIsDead())
		{
			itr = enemys_.erase(itr);
			//消した配列の次が終わりだったら
			if (itr == enemys_.end()) continue;

			lockOnEnemy_ = nullptr;
			continue;
		}
		itr->get()->PreUpdate();
		//ロックオンしている敵のアドレスを代入
		if (itr->get()->GetIsSoftLockOn() ||
			itr->get()->GetIsHardLockOn()) {
			lockOnEnemy_ = itr->get();
		}

		itr++;
	}
	
	if (lockOnEnemy_ != nullptr) {
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
	//hpゲージにロックオン中の敵をセット
	hpGauge_.SetLockOnEnemy(lockOnEnemy_);
	hpGauge_.Update();
}

void EnemyManager::PostUpdate()
{
	for (auto& enemy : enemys_) {
		enemy->PostUpdate();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
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
			lockOnSprite_[i]->SetColor(Color(240,175,30,255));
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