#pragma once
#include "IEnemy.h"
#include "Vector2.h"
#include "Sprite.h"
class EnemyDummy final :
    public IEnemy 
{
private:
    Vector2 pos2D;
public:
    EnemyDummy(Vector3 pos);
private:
    void MoveUpdate() override;
    void DrawSprite() override;

};

