#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	lockOnobjTimer_.SetLimitTime(360);
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, -20))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 20))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(10, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-10, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, -10))));

	lockOnObj_ = std::move(std::make_unique<Object3d>());
	lockOnObj_->SetModel(Model::CreateOBJ_uniptr("LockOn", true));
	lockOnObj_->SetAmbient("LockOn", { 0,0.2f,0.8f });
}

void EnemyManager::PreUpdate()
{
	lockOnEnemy_ = nullptr;
	for (auto& enemy : enemys_) {
		enemy->PreUpdate();
		//ロックオンしている敵のアドレスを代入
		if (enemy->GetIsLockOn()) {
			lockOnEnemy_ = enemy.get();
		}
	}
	if (lockOnEnemy_ != nullptr) {
		lockOnobjTimer_.AddTime(1);
		lockOnObj_->GetTransform()->SetPosition(lockOnEnemy_->GetWorldTransform()->position_);
		//ロックオンオブジェを上下に揺らす
		float posy = 4 + sinf((float)lockOnobjTimer_.GetTimer() / 15.f) * 0.4f;
		lockOnObj_->GetTransform()->AddPosition({ 0,posy,0});
		lockOnObj_->GetTransform()->AddRotation({ 0,0.03f,0 });
		lockOnObj_->Update();
	}
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
	if (lockOnEnemy_ != nullptr) {
		lockOnObj_->Draw();
	}
}
