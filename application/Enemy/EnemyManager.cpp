#include "EnemyManager.h"

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
