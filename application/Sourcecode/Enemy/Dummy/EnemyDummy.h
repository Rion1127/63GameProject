#pragma once
#include "IEnemy.h"
#include "Vector2.h"
#include "Sprite.h"

/**
 * @file EnemyDummy.h
 * @brief デバッグ用の地上の敵のクラス
 */

class EnemyDummy final :
    public IEnemy
{
private:
    Vector2 pos2D;
public:
    EnemyDummy(const Vector3& pos);
    void SetIsNock(bool flag) override {isKnock_ = flag; };
    void SetIsDown(bool flag) override { isKnock_ = flag; };
    void BulletShot(std::list<std::unique_ptr<IBullet>>* bullets) override { bullets; };
private:
    void MoveUpdate() override;
    void DrawSprite() override;
    void DamageUpdate() override {};
    
};

