#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

class Player;

class PlayerGuard :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerGuard(Player* player) :State<PlayerState>(PlayerState::Guard), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

