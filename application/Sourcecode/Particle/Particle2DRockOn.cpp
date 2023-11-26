#include "Particle2DRockOn.h"
#include "Easing.h"

Particle2DRockOn::Particle2DRockOn()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Ini();
	sprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	//sprite_->SetScale(Vector2(0.15f, 0.15f));

	upLength_ = 2;
	downLength_ = 2;

	startPosTimer_.SetLimitTime(20);
	endPosTimer_.SetLimitTime(20);
	delayTimer_.SetLimitTime(15);
}

void Particle2DRockOn::Reset()
{
	startPosTimer_.SetLimitTime(10);
	endPosTimer_.SetLimitTime(10);
	delayTimer_.SetLimitTime(15);

	startPosTimer_.Reset();
	endPosTimer_.Reset();
	delayTimer_.Reset();
	isStart_ = false;
}

void Particle2DRockOn::Update()
{
	if (isStart_) {
		startPosTimer_.AddTime(1);
		delayTimer_.AddTime(1);
		if (delayTimer_.GetIsEnd()) {
			endPosTimer_.AddTime(1);
		}

		Vector2 nowStartPos = {
			Easing::Quint::easeOut(startPos_.x,endPos_.x,startPosTimer_.GetTimeRate()),
			Easing::Quint::easeOut(startPos_.y,endPos_.y,startPosTimer_.GetTimeRate()),
		};
		Vector2 nowEndPos = {
			Easing::Quint::easeOut(startPos_.x,endPos_.x,endPosTimer_.GetTimeRate()),
			Easing::Quint::easeOut(startPos_.y,endPos_.y,endPosTimer_.GetTimeRate()),
		};

		Vector2 lengthVec = startPos_ - endPos_;
		lengthVec = RotateVector(lengthVec, 90.f).normalize();

		LUPos_ = nowStartPos - (lengthVec * upLength_);
		RUPos_ = nowStartPos + (lengthVec * upLength_);
		LDPos_ = nowEndPos - (lengthVec * downLength_);
		RDPos_ = nowEndPos + (lengthVec * downLength_);
		sprite_->SetInvisivle(false);
		sprite_->Update();
	}
	else {
		sprite_->SetInvisivle(true);
	}


}

void Particle2DRockOn::Draw()
{
	sprite_->Draw(LUPos_, RUPos_, LDPos_, RDPos_);
}
