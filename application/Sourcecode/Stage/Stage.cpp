#include "Stage.h"
#include "Util.h"
Stage::Stage()
{

	for (int32_t i = 0; i < walls_.size(); i++) {
		walls_[i] = MakeUnique<Plane>();
		//透明な壁
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
		obj.second->Update();
	}
}

void Stage::Draw()
{
	for (auto& obj : stageObj_) {
		if (obj.first == "skySphere") {
			PipelineManager::PreDraw("SkySphere", TRIANGLELIST);
		}
		else {
			PipelineManager::PreDraw("Object3D", TRIANGLELIST);
		}
		obj.second->Draw();
	}
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
}
