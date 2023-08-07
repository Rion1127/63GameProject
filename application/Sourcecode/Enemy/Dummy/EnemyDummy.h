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
    void SetIsNock(bool flag) override {};
    void BulletShot(std::list<std::unique_ptr<IAttack>>* bullets) override {};
private:
    void MoveUpdate() override;
    void DrawSprite() override;
};

