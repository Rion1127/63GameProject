#include "BaseAttack.h"
#include "GameSpeed.h"
#include "RRandom.h"
#include "mSound.h"
#include "SoundVolume.h"

/**
 * @file BaseAttack.cpp
 * @brief プレイヤーの基本攻撃の動作をするクラス
 */

BaseAttack::BaseAttack(const AttackData& input, IActor* selfActor, IActor* lockOnActor) :
	attackdata_(input),
	selfActor_(selfActor),
	lockOnActor_(lockOnActor)
{
	spline_.SetParent(selfActor_->GetWorldTransform());
	index_ = 0;

	resultQuaternion_ = selfActor_->GetWorldTransform()->quaternion_;

	CalculateRotToLockOnActor();

	SetNextAttack();

	//攻撃に掛かる時間を計算する
	float attackAllTime = 0;
	for (auto& info : attackdata_.attackinfo) {
		attackAllTime += info.attackFrame + info.gapFrame;
	}
	attackAllTime_.SetLimitTime(attackAllTime);

	spline_.Update();
	swordPos_ = spline_.GetNowPoint();

	//剣の座標をに当たり判定代入
	col_.center = swordPos_;
	col_.radius = 1.0f;
	float& attackTime = attackdata_.attackinfo[index_].attackFrame;
	damageCoolTime_ = (attackTime - spline_.GetTimer().GetTimer());

	nextAttackFrame_ += attackdata_.attackinfo[index_].attackAllFrame;

	playerAxisY_ = selfActor_->GetAxisY();
	

	float time = (attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame - attackAllTime_.GetTimer());
	slerpSpeed_ = 1.f / time;

	for (auto& info : attackdata_.attackinfo)
	{
		for (auto& q : info.quaternion)
		{
			q.q = selfActor_->GetAxisY() * q.q;
		}
	}
	quaternionTimer_ = 0;
}

void BaseAttack::SetNextAttack()
{
	spline_.AllClear();
	spline_.SetIsStart(true);
	spline_.SetTimerType_(attackdata_.attackinfo[index_].timerType);
	spline_.SetEasingType_(attackdata_.attackinfo[index_].easingType);
	spline_.SetEasingTypeInOut_(attackdata_.attackinfo[index_].inOutType);
	spline_.SetMaxTime(attackdata_.attackinfo[index_].attackFrame);
	SplinePosUpdate();

	auto& addVec = attackdata_.attackinfo[index_].playerMoveVec;

	addVec = RotateVector(addVec, selfActor_->GetAxisY());

	selfActor_->SetGravity(attackdata_.attackinfo[index_].gravity);
	quaternionIndex_ = 0;
	oneSwingTimer_ = attackAllTime_.GetTimer();
}

void BaseAttack::Update()
{
	spline_.Update(GameSpeed::GetPlayerSpeed());
	attackAllTime_.AddTime(GameSpeed::GetPlayerSpeed());
	//攻撃が終了したら
	if (attackAllTime_.GetTimer() > nextAttackFrame_) {
		//残りの攻撃がある場合は続ける
		if (index_ < attackdata_.attackinfo.size() - 1) {
			index_++;
			nextAttackFrame_ += attackdata_.attackinfo[index_].attackAllFrame;
			SetNextAttack();

			float picth = RRandom::RandF(0.7f, 1.5f);
			SoundManager::Play("SwingSE", false, SoundVolume::GetValumeSE(), picth);
		}
	}
	//攻撃中フラグ
	if (attackAllTime_.GetIsEnd()) {
		isAttaking_ = false;
	}
	else {
		isAttaking_ = true;
	}
	//ダメージクールタイム計算
	float& attackTime = attackdata_.attackinfo[index_].attackFrame;
	damageCoolTime_ = (attackTime - spline_.GetTimer().GetTimer());

	//プレイヤーの移動
	PlayerMove();
	QuaternionUpdate();

	swordPos_ = spline_.GetNowPoint();
	//当たり判定更新
	ColUpdate();
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
		frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetAxisY());
	}
	return frontVec.normalize();
}

void BaseAttack::CalculateRotToLockOnActor()
{
	Vector2 vec = { CalculateFrontVec().x,CalculateFrontVec().z };
	float rot = Vec2Angle(vec);
	Vector3 vecY = { 0, 1, 0 };
	auto axisY = MakeAxisAngle(vecY, Radian(rot));
	selfActor_->SetAxisY(axisY);
	Vector3 frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetAxisY());
	frontVec.y = 0;
	Vector2 angleVec2 = {
		frontVec.x,
		frontVec.z
	};
	selfActor_->SetObjAngle(Vec2Angle(angleVec2));
	selfActor_->GetWorldTransform()->quaternion_ = axisY;
}

void BaseAttack::SplinePosUpdate()
{
	spline_.DeleteAllPoint();
	auto& attackpoint = attackdata_.attackinfo[index_].splinePos;
	for (int32_t i = 0; i < attackpoint.size(); i++)
	{
		Vector3 splinePos = attackpoint[i];
		splinePos.y += selfActor_->GetWorldTransform()->scale_.y;
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

void BaseAttack::PlayerMove()
{
	selfActor_->AddaddVec(attackdata_.attackinfo[index_].playerMoveVec);
	if (attackdata_.attackinfo[index_].playerMoveVec.length() != 0) {
		MoveTo(Vector3(0, 0, 0), attackdata_.attackinfo[index_].deceleration, attackdata_.attackinfo[index_].playerMoveVec);
	}
}

void BaseAttack::ColUpdate()
{
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

void BaseAttack::QuaternionUpdate()
{
	auto& frame = attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame;
	auto timer = attackAllTime_.GetTimer() - oneSwingTimer_;
	if (timer > frame)
	{
		if (quaternionIndex_ < attackdata_.attackinfo[index_].quaternion.size() - 1)
		{
			quaternionTimer_ += attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame;
			quaternionIndex_++;

			float time = (attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame - attackAllTime_.GetTimer());
			slerpSpeed_ = 1.f / time;
		}
	}
	auto& currentQuaternion = attackdata_.attackinfo[index_].quaternion[quaternionIndex_].q;

	resultQuaternion_ = resultQuaternion_.Slerp(currentQuaternion, 0.3f);

	selfActor_->GetDisplayWorldTransform()->SetQuaternion(resultQuaternion_);
}
