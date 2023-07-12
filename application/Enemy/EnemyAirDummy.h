#pragma once
#include "IEnemy.h"
class EnemyAirDummy final :
    public IEnemy
{
private:
public:
    EnemyAirDummy(Vector3 pos);
private:
    void MoveUpdate() override;
    void DrawSprite() override;
};

