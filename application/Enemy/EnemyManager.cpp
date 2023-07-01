#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>()));
	
}

void EnemyManager::PreUpdate()
{
	for (auto& enemy : enemys_) {
		enemy->PreUpdate();
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
}
