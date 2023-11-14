#pragma once
#include "IEnemy.h"

/**
 * @file EnemyAirDummy.h
 * @brief デバッグ用の空中の敵のクラス
 */

class EnemyAirDummy final :
    public IEnemy
{
private:
public:
    EnemyAirDummy(const Vector3& pos);
    void SetIsNock(bool flag) override { isKnock_ = flag; };
    void SetIsDown(bool flag) override { isKnock_ = flag; };
    void BulletShot(std::list<std::unique_ptr<IBullet>>* bullets) override { bullets; };
private:
    void MoveUpdate() override;
    void DrawSprite() override;
    void DamageUpdate() override {};
};

