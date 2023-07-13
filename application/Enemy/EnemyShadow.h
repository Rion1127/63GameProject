#pragma once
#include "IEnemy.h"
#include "Timer.h"

class EnemyShadow :
    public IEnemy
{
private:
    enum class State {
        Idle,
        Move,
        HideMove,
        Attack,
        JumpAttack,
        None
    };
private:
    State state_;
    Timer actionTimer_;
public:
    EnemyShadow(Vector3 pos);
private:
    void MoveUpdate() override;
    void DrawSprite() override;

    void Idle(void);
    void Move();
    void HideMove();
    void Attack();
    void JumpAttack();

   
};
