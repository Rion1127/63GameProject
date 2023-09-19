#include "PauseMenu.h"
#include "Texture.h"
#include "mInput.h"
#include "SceneManager.h"
#include "mSound.h"
#include "Easing.h"
#include "Util.h"

PauseMenu::PauseMenu()
{
	backSprite_ = std::make_unique<Sprite>();
	pauseSprite_ = std::make_unique<PauseSprite>();
	continueSprite_ = std::make_unique<Sprite>();
	titleSprite_ = std::make_unique<Sprite>();
	backSprite_->Ini();
	continueSprite_->Ini();
	titleSprite_->Ini();

	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	continueSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Continue"));
	titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("TitleTex"));

	backSprite_->SetColor(Color(0, 0, 0, 200.f));
	backSprite_->SetAnchor({ 0,0 });

	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.4f
	};
	continueSprite_->SetPos(pos);
	pos.y = WinAPI::GetWindowSize().y / 1.2f;
	titleSprite_->SetPos(pos);

	selectType_ = SelectType::Continue;
}

void PauseMenu::Update()
{
	if (Controller::GetTriggerButtons(PAD::INPUT_START)) {
		isPause_ = (isPause_ == true) ? false : true;

		pauseSprite_->SetIsAvtive(isPause_);
		pauseSprite_->Reset();
	}

	if (isPause_) {
		//メニュー選択
		if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
			Controller::GetTriggerButtons(PAD::INPUT_UP))
		{
			SoundManager::Play("SelectSE");
			bool type = (selectType_ == SelectType::Continue);
			//現在選択しているものがコンティニューだったら
			selectType_ = (type) ? SelectType::Title : SelectType::Continue;
		}
		//スプライトの色を変える
		Color selectColor = { 200,50,50,255 };
		if (selectType_ == SelectType::Continue)
		{
			continueSprite_->SetColor(selectColor);
			titleSprite_->SetColor(Color(255, 255, 255, 255));

		}
		else if (selectType_ == SelectType::Title)
		{
			continueSprite_->SetColor(Color(255, 255, 255, 255));
			titleSprite_->SetColor(selectColor);
		}
		//決定
		if (Controller::GetTriggerButtons(PAD::INPUT_A) ||
			Key::TriggerKey(DIK_SPACE))
		{
			if (SceneManager::GetIsSetNext() == false)
			{
				SoundManager::Play("EnterSE", false, 1.5f);
			}
			if (selectType_ == SelectType::Continue)
			{
				isPause_ = false;
			}
			else if (selectType_ == SelectType::Title)
			{
				SceneManager::SetChangeStart(SceneName::Title);
			}
		}

		backSprite_->Update();
		pauseSprite_->Update();
		continueSprite_->Update();
		titleSprite_->Update();
	}
}

void PauseMenu::Draw()
{
	if (isPause_) {
		backSprite_->Draw();
		pauseSprite_->Draw();
		continueSprite_->Draw();
		titleSprite_->Draw();
	}
}

PauseSprite::PauseSprite()
{
	for (uint32_t i = 0; i < pauseSprite_.size(); i++) {
		pauseSprite_[i] = std::make_unique<PauseSprits>();
		pauseSprite_[i]->sprite = std::make_unique<Sprite>();
		pauseSprite_[i]->sprite->Ini();
		pauseSprite_[i]->sprite->SetTexture(TextureManager::GetInstance()->GetTexture("Pause"));

		pauseSprite_[i]->sprite->SetTex_Size(Vector2(64, 64));
		pauseSprite_[i]->sprite->SetTex_LeftTop(Vector2(64 * (float)i, 0));
		Vector2 scale = {
			1.f / 5.f,
			1.f
		};
		pauseSprite_[i]->sprite->SetScale(scale);
		scale_ = scale;

		Vector2 easeEndpos = {
			WinAPI::GetWindowSize().x / 2.f + (64 * i) - (64.f * 2.f),
			WinAPI::GetWindowSize().y / 4.0f
		};
		pauseSprite_[i]->sprite->SetPos(easeEndpos);
		pauseSprite_[i]->easeEndPos_ = easeEndpos;

		pauseSprite_[i]->sprite->Update();
		pauseSprite_[i]->isActive = false;

		Vector2 easeStartpos = {
			WinAPI::GetWindowSize().x / 2.f + (64 * i) - (64.f * 2.f),
			WinAPI::GetWindowSize().y / 4.0f - 128.f
		};
		pauseSprite_[i]->easeStartPos_ = easeStartpos;

		pauseSprite_[i]->timer.SetLimitTime(30);
		pauseSprite_[i]->sprite->SetColor(Color(255, 255, 255, 0));

		if (i == pauseSprite_.size() - 1) {
			pauseSprite_[i]->sprite->SetRot(Radian(180));
			pauseSprite_[i]->sprite->SetAnchor(Vector2(0.5f,0.0f));

			Vector2 easeEndpos = {
			WinAPI::GetWindowSize().x / 2.f + (64 * i) - (64.f * 2.f),
			WinAPI::GetWindowSize().y / 4.0f + 32.f
			};
			pauseSprite_[i]->sprite->SetPos(easeEndpos);
			pauseSprite_[i]->easeEndPos_ = easeEndpos;
		}

	}
	timer_.SetLimitTime(10);
	index_ = 0;
	isActive_ = false;
	effectTimer_.SetLimitTime(30);
	state_ = State::Collapse;
}

void PauseSprite::Update()
{
	//徐々に文字をアクティブにしていく
	if (isActive_) {
		timer_.AddTime(1);
		if (timer_.GetIsEnd()) {
			pauseSprite_[index_]->isActive = true;
			index_++;

			uint32_t max = (uint32_t)pauseSprite_.size() - 1;
			index_ = Min(max, index_);

			timer_.Reset();
		}
	}

	for (uint32_t i = 0; i < pauseSprite_.size(); i++) {
		if (pauseSprite_[i]->isActive) {
			if (state_ != State::Jump && state_ != State::Landing) {
				pauseSprite_[i]->timer.AddTime(1);

				Vector2 start = pauseSprite_[i]->easeStartPos_;
				Vector2 end = pauseSprite_[i]->easeEndPos_;
				float rate = pauseSprite_[i]->timer.GetTimeRate();

				Vector2 pos = {
					Easing::Circ::easeOut(start.x,end.x,rate),
					Easing::Circ::easeOut(start.y,end.y,rate)
				};

				pauseSprite_[i]->sprite->SetPos(pos);

				auto color = pauseSprite_[i]->sprite->GetColor();
				color.a = 255.f * rate;
				pauseSprite_[i]->sprite->SetColor(color);
			}

			if (i == pauseSprite_.size() - 1) {
				//Eの文字が回転する
				
				if (pauseSprite_[i]->timer.GetIsEnd()) {
					effectTimer_.AddTime(1);

					if (state_ == State::Collapse) {
						//0.f → 1.f → 0.fのrateを計算する
						float rate01 = 0.2f + fabs(0.5f - effectTimer_.GetTimeRate()) * 2.f;
						float rate02 = 1.f - fabs(0.5f - effectTimer_.GetTimeRate()) * 2.f;

						rate01 = Min(rate01, 1.0f);
						Vector2 scale = {
							scale_.x + (0.1f * rate02),
							scale_.y * rate01
						};

						pauseSprite_[i]->sprite->SetScale(scale);
					}
					//飛び跳ねる
					else if (state_ == State::Jump) {
						Vector2 pos = pauseSprite_[i]->sprite->GetPos();

						pos.y += gravity_;
						gravity_ += 0.5f;

						pauseSprite_[i]->sprite->SetPos(pos);

						float rot = Easing::Sine::easeOut(effectTimer_.GetTimeRate(),Radian(180), Radian(180),1.f);
						pauseSprite_[i]->sprite->SetRot(rot);

						pos = pauseSprite_[i]->sprite->GetPos();
						if (pos.y > pauseSprite_[i]->easeEndPos_.y - 32.f) {
							state_ = State::Landing;
							effectTimer_.Reset();
							pos = pauseSprite_[i]->easeEndPos_;
							pos.y -= 32.f;
							pauseSprite_[i]->sprite->SetPos(pos);
						}
						
					}
					//着地する
					else if (state_ == State::Landing) {

					}

					if (effectTimer_.GetIsEnd()) {
						if (state_ == State::Collapse) {
							state_ = State::Jump;
							gravity_ = -10;
							effectTimer_.Reset();

							pauseSprite_[i]->sprite->SetAnchor(Vector2(0.5f, 0.5f));
							Vector2 pos = pauseSprite_[i]->sprite->GetPos();
							pos.y -= 32.f;
							pauseSprite_[i]->sprite->SetPos(pos);
						}
						else if (state_ == State::Jump) {
							
							
						}
						
					}
				}
			}
		}
		pauseSprite_[i]->sprite->Update();
	}
}

void PauseSprite::Draw()
{
	for (uint32_t i = 0; i < pauseSprite_.size(); i++) {
		if (pauseSprite_[i]->isActive) {
			pauseSprite_[i]->sprite->Draw();
		}
	}
}

void PauseSprite::Reset()
{
	for (uint32_t i = 0; i < pauseSprite_.size(); i++) {
		pauseSprite_[i]->timer.Reset();
		pauseSprite_[i]->isActive = false;
		timer_.Reset();
		index_ = 0;
		state_ = State::Collapse;
		if (i == pauseSprite_.size() - 1) {
			pauseSprite_[i]->sprite->SetRot(Radian(180));
			pauseSprite_[i]->sprite->Update();
			pauseSprite_[i]->sprite->SetAnchor(Vector2(0.5f, 0.0f));
		}
	}
	effectTimer_.Reset();
}
