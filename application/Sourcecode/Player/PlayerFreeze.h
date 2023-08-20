#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

class Player;

class PlayerFreeze :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerFreeze(Player* player) :State<PlayerState>(PlayerState::Freeze), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};
