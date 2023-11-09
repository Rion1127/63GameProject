#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

/**
 * @file PlayerMagic.h
 * @brief ステートマシン
 * @brief プレイヤーが攻撃（魔法）している状態
 */

class Player;

class PlayerMagic :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerMagic(Player* player) :State<PlayerState>(PlayerState::Magic), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};
