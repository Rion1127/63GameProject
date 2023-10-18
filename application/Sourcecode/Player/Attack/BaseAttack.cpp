#include "BaseAttack.h"

BaseAttack::BaseAttack(const AttackInput& input, IActor* selfActor, IActor* lockOnActor) :
	attackinput_(input),
	selfActor_(selfActor),
	lockOnActor_(lockOnActor)
{

	index_ = 0;

	CalculateRotToLockOnActor();

	SetSpline();
	/*obj_->SetModel(Model::CreateOBJ_uniptr("sphere", false));
	obj_->SetAmbient("sphere", { 1.0f, 0, 0 });
	obj_->SetIsVisible(true);*/
}

void BaseAttack::SetSpline()
{
	spline_.AllClear();
	spline_.SetIsStart(true);
	spline_.SetTimerType_(attackinput_.attackinfo[index_].timerType);
	spline_.SetEasingType_(attackinput_.attackinfo[index_].easingType);
	spline_.SetEasingTypeInOut_(attackinput_.attackinfo[index_].inOutType);
	spline_.SetMaxTime(attackinput_.attackinfo[index_].attackFrame);
	auto& attackpoint = attackinput_.attackinfo[index_].splinePos;
	//プレイヤートランスフォームと親子関係にしてスプラインの座標を変化させる
	WorldTransform parent;
	parent.SetRotType(RotType::Quaternion);
	parent.position_ = selfActor_->GetWorldTransform()->position_;
	parent.quaternion_ = selfActor_->GetWorldTransform()->quaternion_;
	parent.Update();
	for (int32_t i = 0; i < attackpoint.size(); i++)
	{
		WorldTransform spline;
		spline.position_ = attackpoint[i];
		spline.parent_ = &parent;
		spline.Update();
		Vector3 splinePos = spline.GetWorldPos();
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

void BaseAttack::Update()
{
	//攻撃が終了したら
	if (spline_.GetisEnd()) {
		//残りの攻撃がある場合は続ける
		if (index_ < attackinput_.attackinfo.size() - 1) {
			index_++;
			SetSpline();
		}
		//次の攻撃がない場合は終わる
		else {
			isAttaking_ = false;
		}
	}
	else {
		isAttaking_ = true;
	}
	swordPos_ = spline_.GetNowPoint();
	
	spline_.Update();

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
