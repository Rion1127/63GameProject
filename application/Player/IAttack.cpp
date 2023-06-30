#include "IAttack.h"

PlayerInfo* IAttack::playerInfo_ = nullptr;

IAttack::IAttack(size_t colNum, size_t maxTime)
{
	attackCol_.resize(colNum);
	for (auto& col : attackCol_)
	{
		col = std::move(std::make_unique<AttackCol>());
		col->colObj_.SetModel(Model::CreateOBJ_uniptr("cube", false));
		col->colObj_.SetAmbient("cube",{ 1.0f, 0, 0 });
	}
	attackInfo_.maxTime = maxTime;
}

void IAttack::Update()
{
	MoveUpdate();

	for (auto& col : attackCol_)
	{
		col->colObj_.WT_.position_ = col->col_.center;
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
