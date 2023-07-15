#pragma once
#include "IEnemy.h"
#include "Timer.h"

class EnemyShadow :
    public IEnemy
{
private:
    enum class State {
        Idle,
        Following,
        Wander,
        HideMove,
        Attack,
        JumpAttack,
        KnockBack,
        None
    };
private:
    State state_;
    Timer actionTimer_;
    std::map<int32_t,State> priority_;
    float followLength;
    float moveSpeed;
private:
    //デバッグ
    std::string stateName_;
public:
    EnemyShadow(Vector3 pos);
    void SetIsNock(bool flag) override;
private:
    void MoveUpdate() override;
    void DrawSprite() override;

    void Idle();
    void Following();
    void Wander();
    void HideMove();
    void Attack();
    void JumpAttack();
    void KnockBack();

    void PriorityUpdate();
};
