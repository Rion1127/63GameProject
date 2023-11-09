#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

/**
 * @file PlayerAirAttack.h
 * @brief ステートマシン
 * @brief プレイヤーが攻撃（空中）している状態
 */

class Player;

class PlayerAirAttack :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerAirAttack(Player* player) :State<PlayerState>(PlayerState::AirAttack), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

