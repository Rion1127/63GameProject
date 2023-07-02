#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, -20))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 20))));

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
		lockOnObj_->GetTransform()->SetPosition(lockOnEnemy_->GetWorldTransform()->position_);
		lockOnObj_->GetTransform()->AddPosition({ 0,2,0 });
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
