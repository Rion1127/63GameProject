#include "IAttack.h"

IAttack::IAttack(IActor* selfActor, int32_t colNum, int32_t maxTime, int32_t damage, int32_t damageCoolTime) :
	selfActor_(selfActor)
{
	attackCol_.resize(colNum);
	for (auto& col : attackCol_)
	{
		col = std::move(std::make_unique<AttackCol>());
		col->colObj_.SetModel(Model::CreateOBJ_uniptr("sphere", false));
		col->colObj_.SetAmbient("sphere", { 1.0f, 0, 0 });
		col->damageCoolTime = damageCoolTime;
		col->damage = damage;
	}
	attackInfo_.maxTime = maxTime;
}

void IAttack::Update()
{
	if (selfActor_ != nullptr) {
		MoveUpdate();
	}

	for (auto& col : attackCol_)
	{
		col->colObj_.WT_.position_ = col->col_.center;
		col->colObj_.WT_.scale_ = {
			col->col_.radius,
			col->col_.radius,
			col->col_.radius,
		};
		col->colObj_.Update();
	}
}

void IAttack::DrawCol()
{
	for (auto& col : attackCol_)
	{
		col->colObj_.Draw();
	}
}

Vector3 IAttack::CalculateFrontVec()
{
	Vector3 frontVec{};
	if (IAttack::lockOnActor_ != nullptr)
	{
		Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
		//ロックオンしている敵へのベクトルをとる
		frontVec = {
			lockOnPos.x - selfActor_->GetWorldTransform()->position_.x,
			0,
			lockOnPos.z - selfActor_->GetWorldTransform()->position_.z,
		};
	}
	return frontVec;
}

void IAttack::CalculateRotToLockOnActor(Vector3 frontVec)
{
	Vector2 frontVec2 = {
		frontVec.x,
		frontVec.z
	};
	float rotY = Radian(Vec2Angle(frontVec2));
	selfActor_->GetWorldTransform()->rotation_ = { 0,rotY ,0 };
}
