#include "ComboCounter.h"
#include "GameSpeed.h"
#include "Util.h"

#pragma region
ComboCounter* ComboCounter::GetInstance()
{
	static ComboCounter instance;
	return &instance;
}

ComboCounter::ComboCounter() {
	comboCount_ = 0;
	comboSprite_.SetComboNum(comboCount_);
	chainTimer_.SetLimitTime(timerLimit_);
}

void ComboCounter::Update()
{
	if (comboCount_ > 0) {
		chainTimer_.AddTime(GameSpeed::GetGameSpeed());
		if (chainTimer_.GetIsEnd()) {
			ResetCombo();
		}
	}
	comboSprite_.Update(chainTimer_.GetTimeRate());
}

void ComboCounter::AddCombo()
{
	//コンボを加算
	comboCount_++;
	//上限と下限を超えないようにする
	comboCount_ = Clamp(comboCount_,0,maxComboNum_);
	comboSprite_.SetComboNum(comboCount_);
	chainTimer_.Reset();
	//コンボが長く続いていたら待ち受け時間を短くする
	float rate = comboCount_ / attenValue_;
	rate = Clamp(rate, 0.f,1.f);
	float limitTime = timerLimit_ - (attenValue_ * rate);
	chainTimer_.SetLimitTime(limitTime);
}
void ComboCounter::ResetCombo()
{
	comboCount_ = 0;
	comboSprite_.SetComboNum(comboCount_);
}
void ComboCounter::Draw()
{
	comboSprite_.Draw();
}
#pragma endregion

#pragma region ComboSprite
ComboSprite::ComboSprite()
{
	spriteInterval_ = 48;
	comboNum_ = 0;
	scaleSubSpeed_ = 0.08f;
	reactionScale_ = 2.5f;
	baseScale_ = Vector2(1.f / 10.f, 1.f);
	basePos_ = Vector2(1100.f,130.f);
	digitNum_ = 3;
	mainColor_ = Color(255,255,255,0);
	gaugeColor_ = Color::white;
	gaugeInitColor_ = Color(255, 255, 255, 0);
	int32_t index = 0;
	for (auto& sprite : numSprite_) {
		sprite.Ini();
		sprite.SetTexture(TextureManager::GetInstance()->GetTexture("Number"));
		sprite.SetTex_Size(Vector2(64,64));
		sprite.SetScale(baseScale_);
		sprite.SetColor(mainColor_);
		Vector2 pos = {
			basePos_.x + spriteInterval_ * index,
			basePos_.y
		};
		if (index == 1) {
			gaugePos_ = pos;
			gaugePos_.y += 64;
		}
		sprite.SetPos(pos);
		index++;
	}
	gaugeFrameSprite_.Ini();
	gaugeFrameSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("ComboGaugeFrame"));
	gaugeFrameSprite_.SetTex_Size(Vector2(192, 24));
	gaugeFrameSprite_.SetScale(Vector2(0.5f, 1.f));
	gaugeFrameSprite_.SetPos(gaugePos_);
	gaugeFrameSprite_.SetColor(gaugeInitColor_);
	gaugeBackSprite_.Ini();
	gaugeBackSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("ComboGaugeFrame"));
	gaugeBackSprite_.SetTex_Size(Vector2(192, 24));
	gaugeBackSprite_.SetScale(Vector2(0.5f, 1.f));
	gaugeBackSprite_.SetTex_LeftTop(Vector2(192, 0));
	gaugeBackSprite_.SetPos(gaugePos_);
	gaugeBackSprite_.SetColor(gaugeInitColor_);
	gaugeSprite_.Ini();
	gaugeSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("ComboGauge"));
	gaugeSprite_.SetColor(gaugeInitColor_);
	gaugeSprite_.SetAnchor(Vector2(0,0));
	Vector2 gaugePos;
	gaugePos = gaugePos_ - gaugeSprite_.GetTexture().size_ / 2;
	gaugeSprite_.SetPos(gaugePos);
}

void ComboSprite::Update(float rate)
{
	auto digit = GetDigitNum(comboNum_, digitNum_);
	for (int32_t i = 0; i < numSprite_.size();i++) {
		Vector2 leftTop = {
			(numSprite_[i].GetTexture().size_.x / 10.f) * digit[i],
			0,
		};
		//コンボが切れた時は消す
		if (comboNum_ <= 0) {
			auto color = numSprite_[i].GetColor();
			color.a -= 3;
			numSprite_[i].SetColor(color);
		}
		//コンボ中は表示する
		else {
			numSprite_[i].SetColor(Color(255,255,255,255));
		}
		auto scale = numSprite_[i].GetScale();
		MoveTo(baseScale_, scaleSubSpeed_, scale);
		numSprite_[i].SetScale(scale);
		numSprite_[i].SetTex_LeftTop(leftTop);
		numSprite_[i].Update();
	}
	if (comboNum_ <= 0) {
		auto color = gaugeFrameSprite_.GetColor();
		color.a -= 3;
		gaugeFrameSprite_.SetColor(color);
		color = gaugeBackSprite_.GetColor();
		color.a -= 3;
		gaugeBackSprite_.SetColor(color);
		color = gaugeSprite_.GetColor();
		color.a -= 3;
		gaugeSprite_.SetColor(color);
	}
	else {
		gaugeFrameSprite_.SetColor(Color::white);
		gaugeBackSprite_.SetColor(Color::white);
		gaugeSprite_.SetColor(gaugeColor_);
	}
	//ゲージ
	gaugeSprite_.SetScale(Vector2(1.f - rate, 1.f));
	gaugeFrameSprite_.Update();
	gaugeBackSprite_.Update();
	gaugeSprite_.Update();
}

void ComboSprite::Draw()
{
	for (auto& sprite : numSprite_) {
		sprite.Draw();
	}
	gaugeBackSprite_.Draw();
	gaugeSprite_.Draw();
	gaugeFrameSprite_.Draw();
}

void ComboSprite::SetComboNum(int32_t num)
{
	comboNum_ = num;
	ReactionInit();
}
void ComboSprite::ReactionInit()
{
	for (int32_t i = 0; i < numSprite_.size(); i++) {
		auto scale = baseScale_ * reactionScale_;
		numSprite_[i].SetScale(scale);
	}
}
#pragma endregion
