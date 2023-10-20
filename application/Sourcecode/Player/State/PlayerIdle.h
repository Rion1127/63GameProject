#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

/**
 * @file PlayerIdle.h
 * @brief ステートマシン
 * @brief プレイヤーが何もしていない状態
 */

class Player;

class PlayerIdle :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerIdle(Player* player) :State<PlayerState>(PlayerState::Idle), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

