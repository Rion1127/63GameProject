#include "PlayerCommand.h"
#include "mInput.h"
#include <imgui.h>
#include "Texture.h"
#include "Easing.h"
#include "mSound.h"
#include "Player.h"

PlayerCommand::PlayerCommand()
{
	Color mainCommandColor = { 0,35,255,255 };
	mainCommandSprite_ = std::make_unique<CommandSprite>(mainCommandColor, Vector2(120, 550), 2);
	mainCommandSprite_->SetTitleTex(TextureManager::GetInstance()->GetTexture("CommandTitle"));
	mainCommandSprite_->SetFrameTex(TextureManager::GetInstance()->GetTexture("CommandFrame"));
	mainCommandSprite_->SetCharaTex(TextureManager::GetInstance()->GetTexture("CommandTex"));
	Color magicCommandColor = { 140,130,230,255 };
	magicCommandSprite_ = std::make_unique<CommandSprite>(magicCommandColor, Vector2(150, 550), 1);
	magicCommandSprite_->SetTitleTex(TextureManager::GetInstance()->GetTexture("CommandMagicTitle"));
	magicCommandSprite_->SetFrameTex(TextureManager::GetInstance()->GetTexture("CommandFrame"));
	magicCommandSprite_->SetCharaTex(TextureManager::GetInstance()->GetTexture("CommandMagicTex"));
	magicCommandSprite_->SetIsVisible(false);
	//選択していないコマンドを透けないようにする
	magicCommandSprite_->SetTranslucent(false);
	magicType_ = MagicType::Fire;
	magicNum_ = 0;
	commandNum_ = 0;
}

void PlayerCommand::Update()
{
	if (isMagicMenu_ == false) {

	}


	attackManager_.SetLockOnEnemy(lockOnEnemy_);
	magicManager_.SetEnemy(lockOnEnemy_);

	//魔法を選択
	if (isMagicMenu_) {
		magicCommandSprite_->SetIsVisible(true);
		
		//キャンセル（魔法コマンドを閉じる）
		if (Controller::GetTriggerButtons(PAD::INPUT_A))
		{
			isMagicMenu_ = false;
			magicCommandSprite_->ResetEase();
			commandNum_ = 0;
		}
		//MPがある時
		if (Controller::GetTriggerButtons(PAD::INPUT_B)) {
			if (player_->GetIsMPCharge() == false) {
				isMagicMenu_ = false;
				magicCommandSprite_->ResetEase();
				commandNum_ = 0;
				//選択した魔法を撃つ
				if (MagicType::Fire == (MagicType)magicNum_) {
					magicManager_.ShotMagic(MagicType::Fire);
				}
			}
			//MPチャージ中の時
			else {

			}
		}
	}
	else {
		magicCommandSprite_->SetIsVisible(false);
		//上下でコマンド選択
		if (Controller::GetTriggerButtons(PAD::INPUT_UP)) {
			commandNum_--;
			if (commandNum_ < 0) {
				commandNum_ = (uint16_t)Command::END - 1;
			}
			mainCommandSprite_->ResetEase();
			SoundManager::Play("SelectSE", false, 1.0f, 0.5f);
		}
		if (Controller::GetTriggerButtons(PAD::INPUT_DOWN)) {
			commandNum_++;
			if (commandNum_ >= (uint16_t)Command::END) {
				commandNum_ = 0;
			}
			mainCommandSprite_->ResetEase();
			SoundManager::Play("SelectSE", false, 1.0f, 0.5f);
		}
		//通常攻撃
		if (selectCommand_ == Command::Attack) {
			attackManager_.Attack();
		}
		//魔法コマンド選択
		else if (selectCommand_ == Command::Magic) {
			if (Controller::GetTriggerButtons(PAD::INPUT_B)) {
				isMagicMenu_ = true;
				magicCommandSprite_->ResetEase();
			}
		}
	}
	selectCommand_ = (Command)commandNum_;

	attackManager_.Update();
	magicManager_.Update();

	mainCommandSprite_->SetCurrentNum(commandNum_);
	mainCommandSprite_->SpriteUpdate();
	magicCommandSprite_->SetCurrentNum(magicNum_);
	magicCommandSprite_->SpriteUpdate();
}

void PlayerCommand::Draw()
{
	ImGui::Begin("PlayerCommand");

	std::string commandState = "";
	if (selectCommand_ == Command::Attack)commandState = "Attack";
	if (selectCommand_ == Command::Magic)commandState = "Magic";
	ImGui::Text(commandState.c_str());

	ImGui::End();

	magicManager_.Draw();
}

void PlayerCommand::DrawSprite()
{
	mainCommandSprite_->DrawSprite();
	magicCommandSprite_->DrawSprite();
}

#pragma region CommandSprite
CommandSprite::CommandSprite(Color color, Vector2 basePos, uint32_t menuNum)
{
	isVisible_ = true;
	basePos_.resize(menuNum);
	frame_.resize(menuNum);
	commandTex_.resize(menuNum);
	baseColor_.resize(menuNum);
	for (uint32_t i = 0; i < menuNum; i++) {
		basePos_[i] = basePos;
		basePos_[i] = {
			basePos_[i].x,
			basePos_[i].y + i * (32.f * 1.2f)
		};

		frame_[i] = std::make_unique<Sprite>();
		frame_[i]->Ini("");
		frame_[i]->SetPos(basePos_[i]);
		frame_[i]->SetScale(Vector2(0.5f, 0.6f));
		frame_[i]->SetColor(color);
		baseColor_[i] = color;

		commandTex_[i] = std::make_unique<Sprite>();
		commandTex_[i]->Ini("");
		commandTex_[i]->SetPos(basePos_[i]);
		commandTex_[i]->SetTex_LeftTop(Vector2(i * 160.f, 1));
		commandTex_[i]->SetTex_Size(Vector2(160.f, 36));
		commandTex_[i]->SetScale(Vector2(0.3f, 0.6f));
		commandTex_[i]->SetAnchor(Vector2(0.8f, 0.5f));
		commandTex_[i]->SetColor(Color(255, 255, 255, 255));
	}

	commandTitle_ = std::make_unique<Sprite>();
	commandTitle_->Ini("");
	Vector2 titlePos = basePos_[0];
	titlePos.y -= (30.f * 1.2f);
	commandTitle_->SetPos(titlePos);
	commandTitle_->SetScale(Vector2(0.5f, 0.6f));
	commandTitle_->SetColor(color);
	Color col = commandTitle_->GetColor();
	col.a = 255.f;
	commandTitle_->SetColor(col);
	commandTitle_->SetTex_LeftTop(Vector2(0, 5));

	easeTimer_.SetLimitTime(5);
}

void CommandSprite::SpriteUpdate()
{
	easeTimer_.AddTime(1);

	float startPos = basePos_[currentNum_].x;
	float endPos = basePos_[currentNum_].x + 20.f;

	Vector2 easePos = {
		Easing::Back::easeOut(startPos,endPos,easeTimer_.GetTimeRate()),
		basePos_[currentNum_].y,
	};

	for (uint32_t i = 0; i < frame_.size(); i++) {
		if (i == currentNum_) {
			frame_[i]->SetPos(easePos);
			frame_[i]->
				SetTexture(TextureManager::GetInstance()->
					GetTexture("CommandSelect"));
			frame_[i]->SetColor(baseColor_[i]);
			commandTex_[i]->SetPos(easePos);
		}
		else {
			frame_[i]->SetPos(basePos_[i]);
			frame_[i]->
				SetTexture(TextureManager::GetInstance()->
					GetTexture("CommandFrame"));
			Color col = baseColor_[i];
			if (isTranslucent_) {
				col.a = 200;
			}
			frame_[i]->SetColor(col);
			commandTex_[i]->SetPos(basePos_[i]);
		}
	}

	for (uint32_t i = 0; i < frame_.size(); i++) {
		frame_[i]->Update();
		commandTex_[i]->Update();
	}
	commandTitle_->Update();
}

void CommandSprite::DrawSprite()
{
	if (isVisible_ == false) return;
	for (uint32_t i = 0; i < frame_.size(); i++) {
		frame_[i]->Draw();
		commandTex_[i]->Draw();
	}
	commandTitle_->Draw();
}

void CommandSprite::ResetEase()
{
	easeTimer_.Reset();
}

void CommandSprite::SetFrameTex(Texture* texture)
{
	for (auto& sprite : frame_) {
		sprite->SetTexture(texture);
	}
}

void CommandSprite::SetCharaTex(Texture* texture)
{
	for (auto& sprite : commandTex_) {
		sprite->SetTexture(texture);
	}
}
#pragma endregion
