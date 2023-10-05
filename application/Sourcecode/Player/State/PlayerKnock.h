#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

class Player;

class PlayerKnock :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerKnock(Player* player) :State<PlayerState>(PlayerState::Knock), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

