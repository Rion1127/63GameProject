#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"
#include "Timer.h"

/**
 * @file PlayerMove.h
 * @brief ステートマシン
 * @brief プレイヤーの移動している状態
 */

class Player;

class PlayerMove :
    public State<PlayerState>
{
private:
    Player* player_;

    TimerFloat shakeTimer_;
    TimerFloat dashParticleTimer_;
public:
    PlayerMove(Player* player) :State<PlayerState>(PlayerState::Move), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

