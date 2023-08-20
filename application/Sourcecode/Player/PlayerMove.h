#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

class Player;

class PlayerMove :
    public State<PlayerState>
{
private:
    Player* player_;

public:
    PlayerMove(Player* player) :State<PlayerState>(PlayerState::Move), player_(player) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    void Move();
};

