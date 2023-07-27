#include "Stage.h"
#include "Util.h"
Stage::Stage()
{
	//stageObj_.resize(1);
	for (int32_t i = 0; i < stageObj_.size(); i++) {
		stageObj_[i] = MakeUnique<Object3d>();
		/*if (i == 0) {
			stageObj_[i]->SetModel(Model::CreateOBJ_uniptr("cube", false));

			stageObj_[i]->WT_.position_ = { 0,-1.f,0 };
			stageObj_[i]->WT_.rotation_ = { 0,0,0 };
			stageObj_[i]->WT_.scale_ = { 30,1,30 };
			stageObj_[i]->Update();
		}*/
	}
	for (int32_t i = 0; i < walls_.size(); i++) {
		walls_[i] = MakeUnique<Plane>();

		if (Col(i) == Col::Back) {
			walls_[i]->distance = -30;
			walls_[i]->normal = { 0,0,1 };
		}
		else if (Col(i) == Col::Front) {
			walls_[i]->distance = -30;
			walls_[i]->normal = { 0,0,-1 };
		}
		else if (Col(i) == Col::Left) {
			walls_[i]->distance = -30;
			walls_[i]->normal = { 1,0,0 };
		}
		else if (Col(i) == Col::Right) {
			walls_[i]->distance = -30;
			walls_[i]->normal = { -1,0,0 };
		}
	}
	floor_ = MakeUnique<Plane>();
	floor_->distance = 0;
	floor_->normal = { 0,1,0 };
}

void Stage::Update()
{
	for (auto& obj : stageObj_) {
		obj->Update();
	}
}

void Stage::Draw()
{
	for (auto& obj : stageObj_) {
		obj->Draw();
	}
}
