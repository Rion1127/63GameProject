#pragma once
#include "IAttack.h"
#include "IBullet.h"
#include <list>
#include <unordered_map>
#include "MagicFire.h"

/**
 * @file MagicManager.h
 * @brief プレイヤーの魔法を管理している
 */

class Player;

enum class MagicType {
	Fire,
	NONE,
	END
};

class MagicManager
{
private:
	std::unordered_map<MagicType, std::unique_ptr<IBullet>> magicList_;
	std::list<std::unique_ptr<IBullet>> magics_;

	Player* player_;
	IActor* enemy_;
public:
	MagicManager();
	void Update();

	void Draw();
public:
	void ShotMagic(MagicType type = MagicType::Fire);
	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemy(IActor* enemy) { enemy_ = enemy; }
public:
	std::list<std::unique_ptr<IBullet>>* GetBullet() { return &magics_; }
	auto* GetMagicList() { return &magicList_; }
};

