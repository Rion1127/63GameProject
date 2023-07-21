#include "Stage.h"
#include "Util.h"
Stage::Stage()
{
	floor_ = MakeUnique<Floor>();
}

void Stage::Update()
{
	floor_->Draw();
}

void Stage::Draw()
{
	floor_->Draw();
}
