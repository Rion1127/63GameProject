#include "CrossFilter.h"
#include "SceneManager.h"
CrossFilter::CrossFilter()
{
	highLumi_ = std::move(std::make_unique<HighLumi>());
	lineBlurs_[0] = std::move(std::make_unique<LineBlur>());
	lineBlurs_[1] = std::move(std::make_unique<LineBlur>());
	compo_ = std::move(std::make_unique<MultiTexture>(3));
}

void CrossFilter::Update() {
	highLumi_->PUpdate();
	for (size_t i = 0; i < lineBlurs_.size(); i++) {
		lineBlurs_[i]->PUpdate();
	}
	lineBlurs_[0]->SetRadian(45.f);
	lineBlurs_[1]->SetRadian(135.f);
	compo_->PUpdate();
}

void CrossFilter::PreDraw()
{
	highLumi_->PreDrawScene();
	SceneManager::Draw();
	highLumi_->PostDrawScene();

	for (size_t i = 0; i < lineBlurs_.size(); i++) {
		lineBlurs_[i]->PreDrawScene();
		highLumi_->Draw("HighLumi");
		lineBlurs_[i]->PostDrawScene();
	}

	compo_->PreDrawSceneAssin(0);
	SceneManager::Draw();
	compo_->PostDrawSceneAssin(0);

	compo_->PreDrawSceneAssin(1);
	lineBlurs_[1]->Draw("LineBlur");
	compo_->PostDrawSceneAssin(1);

	compo_->PreDrawSceneAssin(2);
	lineBlurs_[0]->Draw("LineBlur");
	compo_->PostDrawSceneAssin(2);
}

void CrossFilter::Draw()
{
	compo_->Draw("CrossFilter");
}