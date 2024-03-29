#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

/**
 * @file PlayerDogeRoll.h
 * @brief ステートマシン
 * @brief プレイヤーが回避（ドッジロール）している状態
 */

class Player;

class PlayerDogeRoll :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerDogeRoll(Player* player) :State<PlayerState>(PlayerState::DodgeRoll), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

