#pragma once
#include "IEnemy.h"
class EnemyDummy :
    public IEnemy
{
public:

private:
    void MoveUpdate() override;
};

