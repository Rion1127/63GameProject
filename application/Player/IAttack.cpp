#include "IAttack.h"

IAttack::IAttack(size_t colNum)
{
	attackCol_.resize(colNum);
	for (auto& col : attackCol_)
	{
		col = std::move(std::make_unique<AttackCol>());
		col->colObj_.SetModel(Model::CreateOBJ_uniptr("cube", false));
	}
	controller_ = Controller::GetInstance();
}

void IAttack::Update()
{
	MoveUpdate();

	for (auto& col : attackCol_)
	{
		col->colObj_.SetPos(col->col_.center);
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
