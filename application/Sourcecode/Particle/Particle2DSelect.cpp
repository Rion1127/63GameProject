#include "Particle2DSelect.h"
#include "Util.h"

Particle2DSelect::Particle2DSelect()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Ini();
	sprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectParticle"));
	sprite_->SetScale(Vector2(0.15f, 0.15f));

	timer_.SetLimitTime(360);
	isInitPos_ = true;
}

void Particle2DSelect::Update()
{
	timer_.AddTime(1);
	if (timer_.GetIsEnd()) {
		timer_.Reset();
	}


	offsetPos_ = {
		UpAndDown(timer_.GetLimitTimer() / 3.f,20.f,timer_.GetTimer()),
		UpAndDown(timer_.GetLimitTimer() / 4.f,20.f,timer_.GetTimer())
	};

	endPos_ += offsetPos_;

	pos_ += (endPos_ - pos_) * 0.06f;

	sprite_->SetPos(pos_);
	sprite_->Update();
}

void Particle2DSelect::Draw()
{
	sprite_->Draw();
}

void Particle2DSelect::SetPos(Vector2 pos)
{
	endPos_ = pos;
	if (isInitPos_) {
		isInitPos_ = false;
		pos_ = endPos_;
	}
}
