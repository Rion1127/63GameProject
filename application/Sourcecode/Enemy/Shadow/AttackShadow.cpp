#include "AttackShadow.h"

/**
 * @file AttackShadow.cpp
 * @brief シャドウの通常攻撃（前方向）
 */

AttackShadow::AttackShadow(IActor* selfActor) :
	IAttack(selfActor, 1, 40, 5, 41)
{
}

void AttackShadow::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		frontDist_ = 0;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		else {
			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(selfActor_->GetWorldTransform()->rotation_.y),
				0,
				cosf(selfActor_->GetWorldTransform()->rotation_.y),
			};
		}
		frontVec = frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
		attackCol_.at(0)->col_.center = {0,10,0};
		attackCol_.at(0)->col_.radius = 0.6f;
		attackCol_.at(0)->damage = 10;
		//ノックバック力
		attackCol_.at(0)->knockPower = { 0.1f,0.3f,0.1f };
		attackCol_.at(0)->knockVecY = 0.5f;
	}

	attackVec_ = frontVec;

	SplineInit();

	spline_.SetParent(selfActor_->GetDisplayWorldTransform());
}

void AttackShadow::MoveUpdate()
{
	spline_.Update(GameSpeed::GetEnemySpeed());
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	Vector3 speed = attackVec_ * 0.5f;
	float timerate = 1.f - (float)(attackInfo_.nowTime / 1.5f) / attackInfo_.maxTime;
	timerate = Clamp(timerate,0.f,1.f);
	speed *= timerate;

	selfActor_->AddaddVec(speed);
	Vector3 attackVec = attackVec_ * (selfActor_->GetWorldTransform()->scale_.x * 1.f);
	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	attackCol_.at(0)->col_.center.y += selfActor_->GetWorldTransform()->scale_.y;

	
}


void AttackShadow::SplineInit()
{
	spline_.SetLimitTime(attackInfo_.maxTime - 20);

	std::vector<Vector3>attackVec;
	Vector3 up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 2.f);
	Vector3 playerUpPos = up;
	attackVec.push_back(playerUpPos);
	attackVec.push_back(playerUpPos);

	up = {
		0.f,
		0.f + selfActor_->GetWorldTransform()->scale_.y * 2.f,
		0.5f,
	};
	Vector3 playermiddlePos = up;
	attackVec.push_back(playermiddlePos);

	Vector3 playerFrontPos(0,-1.0f,2.5f);
	//playerFrontPos.y += 0.2f;
	attackVec.push_back(playerFrontPos);
	attackVec.push_back(playerFrontPos);

	spline_.SetPositions(attackVec);
}