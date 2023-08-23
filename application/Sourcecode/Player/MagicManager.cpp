#include "MagicManager.h"
#include "Player.h"

MagicManager::MagicManager()
{
}

void MagicManager::Update()
{
	std::list<std::unique_ptr<IBullet>>::iterator itr;
	for (itr = magics_.begin(); itr != magics_.end();) {

		if ((*itr)->GetIsDead()) {
			itr = magics_.erase(itr);
			continue;
		}

		(*itr)->Update();

		itr++;
	}
}

void MagicManager::Draw()
{
	for (auto& bullet : magics_) {
		bullet->DrawCol();
	}
}

void MagicManager::AddBrrizard()
{
	std::unique_ptr<IBullet> magic = std::make_unique<MagicFire>(player_);

	magic->SetLockOnActor(enemy_);
	magic->Init();
	magics_.emplace_back(std::move(magic));

	player_->GoToState(PlayerState::Magic);
}

void MagicManager::ShotMagic(MagicType type)
{
	std::unique_ptr<IBullet> magic;

	if (type == MagicType::Fire) {
		magic = std::make_unique<MagicFire>(player_);
	}

	player_->GoToState(PlayerState::Magic);
	player_->SubMP(magic->GetCostMP());

	magic->SetLockOnActor(enemy_);
	magic->Init();
	magics_.emplace_back(std::move(magic));
}

