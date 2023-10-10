#include "MagicManager.h"
#include "Player.h"
#include "MagicFire.h"
#include "mSound.h"
#include "SoundVolume.h"

MagicManager::MagicManager()
{
	magicList_.insert(std::make_pair(MagicType::Fire, std::make_unique<MagicFire>(nullptr)));
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

	SoundManager::Play("FireSE", false, SoundVolume::GetValumeSE());
}

