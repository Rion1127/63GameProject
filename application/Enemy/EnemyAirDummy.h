#pragma once
#include "IEnemy.h"
class EnemyAirDummy final :
    public IEnemy
{
private:
public:
    EnemyAirDummy(Vector3 pos);
    void SetIsNock(bool flag) override {};
private:
    void MoveUpdate() override;
    void DrawSprite() override;
};

