#pragma once
#include "IEnemy.h"
class EnemyDummy :
    public IEnemy
{
private:
public:
    EnemyDummy(Vector3 pos);
private:
    void MoveUpdate() override;
};

