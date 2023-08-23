#pragma once
#include "IAttack.h"
#include "IBullet.h"
#include <list>
#include "MagicFire.h"

class Player;

enum class MagicType {
	Fire,
	NONE,
	END
};

class MagicManager
{
private:
	std::list<std::unique_ptr<IBullet>> magics_;

	Player* player_;
	IActor* enemy_;
public:
	MagicManager();
	void Update();

	void Draw();
public:
	void AddBrrizard();
	void ShotMagic(MagicType type = MagicType::Fire);
	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemy(IActor* enemy) { enemy_ = enemy; }
public:
	std::list<std::unique_ptr<IBullet>>* GetBullet() { return &magics_; }
};

