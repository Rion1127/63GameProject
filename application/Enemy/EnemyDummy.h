#pragma once
#include "IEnemy.h"
class EnemyDummy :
    public IEnemy
{
public:
    EnemyDummy() : IEnemy() {};
private:
    void MoveUpdate() override;
};

