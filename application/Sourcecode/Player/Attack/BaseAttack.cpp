#include "BaseAttack.h"
#include "GameSpeed.h"

/**
 * @file BaseAttack.cpp
 * @brief プレイヤーの基本攻撃の動作をするクラス
 */

BaseAttack::BaseAttack(const AttackInput& input, IActor* selfActor, IActor* lockOnActor) :
	attackinput_(input),
	selfActor_(selfActor),
	lockOnActor_(lockOnActor)
{
	spline_.SetParent(selfActor_->GetWorldTransform());
	index_ = 0;

	CalculateRotToLockOnActor();

	SetNextAttack();

	//攻撃に掛かる時間を計算する
	float attackAllTime = 0;
	for (auto& info : attackinput_.attackinfo) {
		attackAllTime += info.attackFrame + info.gapFrame;
	}
	attackAllTime_.SetLimitTime(attackAllTime);

	spline_.Update();
	swordPos_ = spline_.GetNowPoint();

	selfActor_->SetGravity(attackinput_.attackinfo[index_].gravity);

	//剣の座標をに当たり判定代入
	col_.center = swordPos_;
	col_.radius = 1.0f;
	float& attackTime = attackinput_.attackinfo[index_].attackFrame;
	damageCoolTime_ = (attackTime - spline_.GetTimer().GetTimer());
}

void BaseAttack::SetNextAttack()
{
	spline_.AllClear();
	spline_.SetIsStart(true);
	spline_.SetTimerType_(attackinput_.attackinfo[index_].timerType);
	spline_.SetEasingType_(attackinput_.attackinfo[index_].easingType);
	spline_.SetEasingTypeInOut_(attackinput_.attackinfo[index_].inOutType);
	spline_.SetMaxTime(attackinput_.attackinfo[index_].attackFrame);
	SplinePosUpdate();
}

void BaseAttack::Update()
{
	//SplinePosUpdate();

	spline_.Update(GameSpeed::GetPlayerSpeed());
	attackAllTime_.AddTime(GameSpeed::GetPlayerSpeed());
	//攻撃が終了したら
	if (attackAllTime_.GetTimer() > attackinput_.attackinfo[index_].attackAllFrame) {
		//残りの攻撃がある場合は続ける
		if (index_ < attackinput_.attackinfo.size() - 1) {
			index_++;
			SetNextAttack();
		}
	}

	if (attackAllTime_.GetIsEnd()) {
		isAttaking_ = false;
	}
	else {
		isAttaking_ = true;
	}
	//ダメージクールタイム計算
	float& attackTime = attackinput_.attackinfo[index_].attackFrame;
	damageCoolTime_ = (attackTime - spline_.GetTimer().GetTimer());


	swordPos_ = spline_.GetNowPoint();
	//剣の座標をに当たり判定代入
	col_.center = swordPos_;
	//当たり判定は剣を振っている間だけ
	if (damageCoolTime_ == 0) {
		col_.isActive = false;
	}
	else {
		col_.isActive = true;
	}
}

void BaseAttack::Draw()
{
	obj_->Draw();
}

Vector3 BaseAttack::CalculateFrontVec()
{
	Vector3 frontVec = { 0,0,0 };
	if (lockOnActor_ != nullptr)
	{
		Vector3& lockOnPos = lockOnActor_->GetWorldTransform()->position_;
		//ロックオンしている敵へのベクトルをとる
		frontVec = {
			lockOnPos.x - selfActor_->GetWorldTransform()->position_.x,
			0,
			lockOnPos.z - selfActor_->GetWorldTransform()->position_.z,
		};
	}
	else
	{
		frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetWorldTransform()->quaternion_);
	}
	return frontVec.normalize();
}

void BaseAttack::CalculateRotToLockOnActor()
{
	Vector2 vec = { CalculateFrontVec().x,CalculateFrontVec().z };
	float rot = Vec2Angle(vec);
	Vector3 vecY = { 0, 1, 0 };
	auto axisY = MakeAxisAngle(vecY, Radian(rot));
	selfActor_->GetWorldTransform()->SetQuaternion(axisY);
	Vector3 frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetWorldTransform()->quaternion_);
	frontVec.y = 0;
	Vector2 angleVec2 = {
		frontVec.x,
		frontVec.z
	};
	selfActor_->SetObjAngle(Vec2Angle(angleVec2));
}

void BaseAttack::SplinePosUpdate()
{
	spline_.DeleteAllPoint();
	auto& attackpoint = attackinput_.attackinfo[index_].splinePos;
	for (int32_t i = 0; i < attackpoint.size(); i++)
	{
		Vector3 splinePos = attackpoint[i];
		if (i == 0)
		{
			spline_.AddPosition(splinePos, PosState::Start);
		}
		else if (i == attackpoint.size() - 1)
		{
			spline_.AddPosition(splinePos, PosState::End);
		}
		else
		{
			spline_.AddPosition(splinePos, PosState::Middle);
		}
	}
}
