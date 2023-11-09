#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

/**
 * @file PlayerAttack.h
 * @brief ステートマシン
 * @brief プレイヤーが攻撃（地上）している状態
 */

class Player;

class PlayerAttack :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerAttack(Player* player) :State<PlayerState>(PlayerState::Attack), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

