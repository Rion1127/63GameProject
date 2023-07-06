#pragma once
#include "IEnemy.h"
class EnemyAirDummy :
    public IEnemy
{
private:
public:
    EnemyAirDummy(Vector3 pos);
private:
    void MoveUpdate() override;
};

