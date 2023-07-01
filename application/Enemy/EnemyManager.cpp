#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>()));
	
}

void EnemyManager::Update()
{
	for (auto& enemy : enemys_) {
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}
}
