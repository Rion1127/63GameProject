#pragma once
#include "IEnemy.h"
class EnemyDummy :
    public IEnemy
{
private:
    float time_ = 0.f;
public:
    EnemyDummy(Vector3 pos);
private:
    void MoveUpdate() override;
};

