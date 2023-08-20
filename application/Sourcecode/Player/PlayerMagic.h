#pragma once
#include "PlayerInfo.h"
#include "StateMachine.h"
#include "Player.h"

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
