#include "IAttack.h"

IAttack::IAttack(IActor* selfActor, IActor* lockOnActor, int32_t colNum, int32_t maxTime, int32_t damage, int32_t damageCoolTime) :
	selfActor_(selfActor),lockOnActor_(lockOnActor)
{
	attackCol_.resize(colNum);
	for (auto& col : attackCol_)
	{
		col = std::move(std::make_unique<AttackCol>());
		col->colObj_.SetModel(Model::CreateOBJ_uniptr("cube", false));
		col->colObj_.SetAmbient("cube", { 1.0f, 0, 0 });
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
