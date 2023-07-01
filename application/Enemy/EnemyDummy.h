#pragma once
#include "IEnemy.h"
class EnemyDummy :
    public IEnemy
{
private:
    float time_ = 0.f;
public:
    EnemyDummy();
private:
    void MoveUpdate() override;
};

