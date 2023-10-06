#include "PauseMenu.h"
#include "Texture.h"
#include "mInput.h"
#include "SceneManager.h"
#include "mSound.h"
#include "Easing.h"
#include "Util.h"
#include "ConfigMenu.h"

PauseMenu::PauseMenu()
{
	backSprite_ = std::make_unique<Sprite>();
	pauseSprite_ = std::make_unique<PauseSprite>();
	backSprite_->Ini();

	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));

	backSprite_->SetColor(Color(0, 0, 0, 200.f));
	backSprite_->SetAnchor({ 0,0 });

	selectType_ = SelectType::Continue;
}

void PauseMenu::Update()
{
	if (Controller::GetTriggerButtons(PAD::INPUT_START))
	{
		if (isConfig_ == false)
		{
			isPause_ = (isPause_ == true) ? false : true;
		}
		pauseSprite_->SetIsActive(isPause_);
		pauseSprite_->Reset();

		SoundManager::Play("MenuSE");
	}

	if (isConfig_ == false)
	{
		if (isPause_)
		{
			if (Controller::GetTriggerButtons(PAD::INPUT_B))
			{
				isPause_ = false;
				SoundManager::Play("CancelSE");
			}

			//メニュー選択
			if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
				Controller::GetTriggerButtons(PAD::INPUT_UP))
			{
				SoundManager::Play("SelectSE");
				int32_t type = (int32_t)(selectType_);
				if (Controller::GetTriggerButtons(PAD::INPUT_DOWN))type++;
				if (Controller::GetTriggerButtons(PAD::INPUT_UP))type--;

				type = Clamp(type, 0, (int32_t)SelectType::Title);

				selectType_ = (SelectType)type;
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
				else if (selectType_ == SelectType::Config)
				{
					isConfig_ = true;
				}
				else if (selectType_ == SelectType::Title)
				{
					SceneManager::SetChangeStart(SceneName::Title);
				}
			}
			backSprite_->Update();
			pauseSprite_->Update();
			pauseSelectSprite_.Update((int32_t)selectType_);

		}
	}
	else
	{
		if (Controller::GetTriggerButtons(PAD::INPUT_START) ||
			Controller::GetTriggerButtons(PAD::INPUT_B))
		{
			isConfig_ = false;
			SoundManager::Play("CancelSE");
		}
		ConfigMenu::GetInstance()->Update();
		ConfigMenu::GetInstance()->SpriteUpdate();
	}


}

void PauseMenu::Draw()
{
	if (isPause_)
	{
		backSprite_->Draw();
	}

	if (isConfig_ == false)
	{
		if (isPause_)
		{
			pauseSprite_->Draw();
			pauseSelectSprite_.Draw();
		}
	}
	else
	{
		ConfigMenu::GetInstance()->Draw();
	}
}

#pragma region PauseSprite
PauseSprite::PauseSprite()
{
	for (uint32_t i = 0; i < pauseSprite_.size(); i++)
	{
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

		if (i == pauseSprite_.size() - 1)
		{
			pauseSprite_[i]->sprite->SetRot(Radian(180));
			pauseSprite_[i]->sprite->SetAnchor(Vector2(0.5f, 0.0f));

			easeEndpos = {
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
	if (isActive_)
	{
		timer_.AddTime(1);
		if (timer_.GetIsEnd())
		{
			pauseSprite_[index_]->isActive = true;
			index_++;

			uint32_t max = (uint32_t)pauseSprite_.size() - 1;
			index_ = Min(max, index_);

			timer_.Reset();
		}
	}

	for (uint32_t i = 0; i < pauseSprite_.size(); i++)
	{
		if (pauseSprite_[i]->isActive)
		{
			if (state_ != State::Jump && state_ != State::Landing)
			{
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

			if (i == pauseSprite_.size() - 1)
			{
				//Eの文字が回転する

				if (pauseSprite_[i]->timer.GetIsEnd())
				{
					effectTimer_.AddTime(1);

					if (state_ == State::Collapse)
					{
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
					else if (state_ == State::Jump)
					{
						Vector2 pos = pauseSprite_[i]->sprite->GetPos();

						pos.y += gravity_;
						gravity_ += 0.5f;

						pauseSprite_[i]->sprite->SetPos(pos);

						float rot = Easing::Sine::easeOut(effectTimer_.GetTimeRate(), Radian(180), Radian(180), 1.f);
						pauseSprite_[i]->sprite->SetRot(rot);

						pos = pauseSprite_[i]->sprite->GetPos();
						if (pos.y > pauseSprite_[i]->easeEndPos_.y - 32.f)
						{
							state_ = State::Landing;
							effectTimer_.Reset();
							pos = pauseSprite_[i]->easeEndPos_;
							pos.y -= 32.f;
							pauseSprite_[i]->sprite->SetPos(pos);
						}

					}
					//着地する
					else if (state_ == State::Landing)
					{

					}

					if (effectTimer_.GetIsEnd())
					{
						if (state_ == State::Collapse)
						{
							state_ = State::Jump;
							gravity_ = -10;
							effectTimer_.Reset();

							pauseSprite_[i]->sprite->SetAnchor(Vector2(0.5f, 0.5f));
							Vector2 pos = pauseSprite_[i]->sprite->GetPos();
							pos.y -= 32.f;
							pauseSprite_[i]->sprite->SetPos(pos);
						}
						else if (state_ == State::Jump)
						{


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
	for (uint32_t i = 0; i < pauseSprite_.size(); i++)
	{
		if (pauseSprite_[i]->isActive)
		{
			pauseSprite_[i]->sprite->Draw();
		}
	}
}

void PauseSprite::Reset()
{
	for (uint32_t i = 0; i < pauseSprite_.size(); i++)
	{
		pauseSprite_[i]->timer.Reset();
		pauseSprite_[i]->isActive = false;
		timer_.Reset();
		index_ = 0;
		state_ = State::Collapse;
		if (i == pauseSprite_.size() - 1)
		{
			pauseSprite_[i]->sprite->SetRot(Radian(180));
			pauseSprite_[i]->sprite->Update();
			pauseSprite_[i]->sprite->SetAnchor(Vector2(0.5f, 0.0f));
		}
	}
	effectTimer_.Reset();
}
#pragma endregion
#pragma region PauseSelectSprite
PauseSelectSprite::PauseSelectSprite()
{
	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i] = std::make_unique<Sprite>();
		texSprite_[i] = std::make_unique<Sprite>();

		frameSprite_[i]->Ini();
		texSprite_[i]->Ini();

		frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		Vector2 framePos = {
			WinAPI::GetWindowSize().x / 2.f,
			450.f + 60.f * i
		};
		frameSprite_[i]->SetPos(framePos);
		frameSprite_[i]->SetScale(Vector2(0.5f, 0.6f));
		texSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("SelectTex"));
		Vector2 texPos = {
			WinAPI::GetWindowSize().x / 2.f,
			450.f + 60.f * i
		};
		if (i == 2)
		{
			texPos.x += 10 * (i - 1);
		}

		float leftUpIndex = 0;
		if (i == 0) leftUpIndex = 2;
		if (i == 1) leftUpIndex = 3;
		if (i == 2) leftUpIndex = 1;
		Vector2 leftTopPos = {
			160.f * leftUpIndex,
			0
		};
		Vector2 texSize = {
			160.f,
			38
		};
		Vector2 scale = {
			(1.f / 4.f) * 0.6f,
			0.6f
		};
		texSprite_[i]->SetPos(texPos);
		texSprite_[i]->SetTex_LeftTop(leftTopPos);
		texSprite_[i]->SetTex_Size(texSize);
		texSprite_[i]->SetScale(scale);

		frameSprite_[i]->Update();
		texSprite_[i]->Update();
	}
}

void PauseSelectSprite::Update(uint32_t index)
{
	Color selectColor = { 230,50,50,255 };
	Color unSelectColor = { 0,35,255,255 };

	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{

		if (i == index)
		{
			frameSprite_[i]->SetColor(selectColor);
			frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));

			Vector2 pos = {
				frameSprite_[i]->GetPos().x + frameSprite_[i]->GetTexture().size_.x / 4.5f,
				frameSprite_[i]->GetPos().y - frameSprite_[i]->GetTexture().size_.y / 4.f
			};
			selectParticle_.SetPos(pos);
		}
		else
		{
			frameSprite_[i]->SetColor(unSelectColor);
			frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		}

		frameSprite_[i]->Update();
		texSprite_[i]->Update();
	}

	selectParticle_.Update();
}

void PauseSelectSprite::Draw()
{
	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i]->Draw();
		texSprite_[i]->Draw();
	}

	selectParticle_.Draw();
}
#pragma endregion