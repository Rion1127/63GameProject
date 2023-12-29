#include "PlayerCommand.h"
#include "mInput.h"
#include <imgui.h>
#include "Texture.h"
#include "Easing.h"
#include "mSound.h"
#include "Player.h"
#include "SoundVolume.h"

/**
 * @file PlayerCommand.cpp
 * @brief プレイヤーのコマンドシステムを処理している
 */

PlayerCommand::PlayerCommand()
{
	mainCommandSprite_ = std::make_unique<CommandSprite>();
	commandNum_ = 0;
}

void PlayerCommand::Update()
{
	attackManager_.SetLockOnEnemy(lockOnEnemy_);
	if (player_->GetIsCanInput()) {
		//通常攻撃
		if (selectCommand_ == Command::Attack) {
			attackManager_.Attack();
		}
	}
	selectCommand_ = (Command)commandNum_;

	attackManager_.Update();
	mainCommandSprite_->Update(player_->GetState(), attackManager_.GetComboNum());
}

void PlayerCommand::Draw()
{
	ImGui::Begin("PlayerCommand");

	std::string commandState = "";
	if (selectCommand_ == Command::Attack)commandState = "Attack";
	if (selectCommand_ == Command::Magic)commandState = "Magic";
	ImGui::Text(commandState.c_str());

	ImGui::End();
}

void PlayerCommand::DrawSprite()
{
	mainCommandSprite_->DrawSprite();
}

#pragma region CommandSprite
CommandSprite::CommandSprite()
{
	Color selectColor = { 0,35,255,255 };
	basePos_ = { 120,500 };
	commandTitle_ = std::make_unique<Sprite>();
	commandTitle_->Ini();
	commandTitle_->SetTexture(TextureManager::GetInstance()->GetTexture("CommandTitle"));
	commandTitle_->SetPos(basePos_);
	commandTitle_->SetColor(selectColor);
	commandTitle_->SetAnchor(Vector2(0, 0));
	//コマンドスプライトを初期化
	for (int32_t i = 0; i < buttonCommand_.size(); i++) {
		buttonCommand_[i] = std::make_unique<ButtonCommand>();
		buttonCommand_[i]->buttonName_ = ButtonName(i);
		buttonCommand_[i]->commandButton_.Ini();
		buttonCommand_[i]->commandFrame_.Ini();
		buttonCommand_[i]->commandTex_.Ini();

		auto& commandButton = buttonCommand_[i]->commandButton_;
		auto& commandFrame = buttonCommand_[i]->commandFrame_;
		auto& commandTex = buttonCommand_[i]->commandTex_;
		
		Vector2 framePos = basePos_;
		//それぞれのボタンのUIの初期化
		if (i == ButtonName::Abutton) {
			//framePos.x += 100;
			framePos.y += 150;
			commandTex.SetTex_LeftTop(Vector2(0, 25.f));
		}
		else if (i == ButtonName::Bbutton) {
			framePos.x += 110;
			framePos.y += 100;
		}
		else if (i == ButtonName::Xbutton) {
			framePos.x -= 110;
			framePos.y += 100;
			commandTex.SetTex_LeftTop(Vector2(0, 50.f));
		}
		else if (i == ButtonName::Ybutton) {
			//framePos.x += 100;
			framePos.y += 50;
		}
		Vector2 buttonPos = framePos;
		Vector2 texPos = framePos;

		buttonPos.x += 17;
		buttonPos.y += 16;
		texPos.x += 40;
		texPos.y += 5;

		commandButton.SetTex_LeftTop(Vector2(25.f * i,0));
		commandButton.SetTex_Size(Vector2(25.f,25.f));
		commandButton.SetScale(Vector2(0.7f / 4, 0.7f));
		commandButton.SetPos(buttonPos);
		//commandButton.SetAnchor(Vector2(0, 0));
		commandButton.SetTexture(TextureManager::GetInstance()->GetTexture("Button"));
		commandFrame.SetPos(framePos);
		commandFrame.SetColor(selectColor);
		commandFrame.SetAnchor(Vector2(0, 0));
		commandFrame.SetTexture(TextureManager::GetInstance()->GetTexture("CommandFrame"));
		if (i != ButtonName::Ybutton) {
			commandTex.SetTexture(TextureManager::GetInstance()->GetTexture("CommandTex"));
			commandTex.SetTex_Size(Vector2(150.f, 25.f));
		}
		else {
			commandTex.SetTexture(TextureManager::GetInstance()->GetTexture("CommandSpecialTex"));
			commandTex.SetTex_Size(Vector2(200.f, 25.f));
			framePos.x += 25;
		}
		commandTex.SetScale(Vector2(0.8f, 0.8f / 4));
		commandTex.SetPos(texPos);
		commandTex.SetAnchor(Vector2(0, 0));
	}
}

void CommandSprite::Update(PlayerState state, int32_t commbo)
{
	commandTitle_->Update();
	for (int32_t i = 0; i < buttonCommand_.size(); i++) {
		//動いている時は「ガード」を「ドッジロール」に変更
		if (state == PlayerState::Move) {
			buttonCommand_[ButtonName::Xbutton]->commandTex_.SetTex_LeftTop(Vector2(0,75.f));
		}
		else {
			buttonCommand_[ButtonName::Xbutton]->commandTex_.SetTex_LeftTop(Vector2(0, 50.f));
		}
		Vector2 texPos = {
			0,
			commbo * 25.f
		};
		buttonCommand_[ButtonName::Ybutton]->commandTex_.SetTex_LeftTop(texPos);

		buttonCommand_[i]->commandButton_.Update();
		buttonCommand_[i]->commandFrame_.Update();
		buttonCommand_[i]->commandTex_.Update();
	}
}

void CommandSprite::DrawSprite()
{
	commandTitle_->Draw();
	for (auto& sprite : buttonCommand_) {
		sprite->commandFrame_.Draw();
		sprite->commandButton_.Draw();
		sprite->commandTex_.Draw();
	}
}

void CommandSprite::ResetEase()
{
	
}
#pragma endregion
