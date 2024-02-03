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
	mainColor_ = { 0,35,255,255 };
	anSelectColRate_ = 0.3f;
	basePos_ = { 120,500 };
	commandTitle_ = std::make_unique<Sprite>();
	commandTitle_->Ini();
	commandTitle_->SetTexture(TextureManager::GetInstance()->GetTexture("CommandTitle"));
	commandTitle_->SetPos(basePos_);
	commandTitle_->SetColor(mainColor_);
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
		commandButton.SetTexture(TextureManager::GetInstance()->GetTexture("Button"));
		commandFrame.SetPos(framePos);
		commandFrame.SetColor(mainColor_);
		commandFrame.SetAnchor(Vector2(0, 0));
		commandFrame.SetTexture(TextureManager::GetInstance()->GetTexture("CommandFrame"));
		if (i != ButtonName::Ybutton) {
			commandTex.SetTexture(TextureManager::GetInstance()->GetTexture("CommandTex"));
			commandTex.SetTex_Size(Vector2(150.f, 25.f));
		}
		else {
			//YボタンのUIを変更
			for (int j = 0; j < specialCommand_.size(); j++) {
				specialCommand_[j] = std::make_unique<SpecialCommand>();
				auto& cmdSprite = specialCommand_[j]->buttonCommand_;
				Vector2 offsetPos = {50,0};
				specialCommand_[j]->easeTimer_.SetLimitTime(10);
				cmdSprite.commandButton_.Ini();
				cmdSprite.commandFrame_.Ini();
				cmdSprite.commandTex_.Ini();
				cmdSprite.commandButton_.SetTex_LeftTop(Vector2(25.f * i, 0));
				cmdSprite.commandButton_.SetTex_Size(Vector2(25.f, 25.f));
				cmdSprite.commandButton_.SetScale(Vector2(0.7f / 4, 0.7f));
				cmdSprite.commandButton_.SetPos(buttonPos + (offsetPos * (float)(j + 1)));
				cmdSprite.commandButton_.SetTexture(TextureManager::GetInstance()->GetTexture("Button"));
				cmdSprite.commandButton_.SetInvisivle(true);
				cmdSprite.commandFrame_.SetPos(framePos + (offsetPos * (float)(j + 1)));
				cmdSprite.commandFrame_.SetColor(mainColor_);
				cmdSprite.commandFrame_.SetAnchor(Vector2(0, 0));
				cmdSprite.commandFrame_.SetTexture(TextureManager::GetInstance()->GetTexture("CommandFrame"));
				cmdSprite.commandFrame_.SetInvisivle(true);
				cmdSprite.commandTex_.SetScale(Vector2(0.8f, 0.8f / 3.7f));
				cmdSprite.commandTex_.SetPos(texPos + (offsetPos * (float)(j + 1)));
				cmdSprite.commandTex_.SetAnchor(Vector2(0, 0));
				Vector2 texLeftTopPos = {
					0,
					(j + 1) * 25.f
				};
				cmdSprite.commandTex_.SetTex_Size(Vector2(200.f, 25.f));
				cmdSprite.commandTex_.SetTex_LeftTop(texLeftTopPos);
				cmdSprite.commandTex_.SetTexture(TextureManager::GetInstance()->GetTexture("CommandSpecialTex"));
				cmdSprite.commandTex_.SetInvisivle(true);
				specialCommand_[j]->easeDist_ = 40;
				specialCommand_[j]->easeStartButtonPosX_ = buttonPos.x + (offsetPos.x * (float)(j + 1)) + specialCommand_[j]->easeDist_;
				specialCommand_[j]->easeEndButtonPosX_ = buttonPos.x + (offsetPos.x * (float)(j + 1));
				specialCommand_[j]->easeStartFramePosX_ = framePos.x + (offsetPos.x * (float)(j + 1)) + specialCommand_[j]->easeDist_;
				specialCommand_[j]->easeEndFramePosX_ = framePos.x + (offsetPos.x * (float)(j + 1));
				specialCommand_[j]->easeStartTexPosX_ = texPos.x + (offsetPos.x * (float)(j + 1)) + specialCommand_[j]->easeDist_;
				specialCommand_[j]->easeEndTexPosX_ = texPos.x + (offsetPos.x * (float)(j + 1));
			}
			commandTex.SetTexture(TextureManager::GetInstance()->GetTexture("CommandSpecialTex"));
			commandTex.SetTex_Size(Vector2(200.f, 25.f));
			framePos.x += 25;
		}
		commandTex.SetScale(Vector2(0.8f, 0.8f / 3.7f));
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

		buttonCommand_[i]->commandButton_.Update();
		buttonCommand_[i]->commandFrame_.Update();
		buttonCommand_[i]->commandTex_.Update();
	}
	Color anSelectTexCol = Color::white * anSelectColRate_;
	Color anSelectCol = mainColor_ * anSelectColRate_;
	anSelectCol.a = 255;
	if (commbo > 0) {
		buttonCommand_[ButtonName::Ybutton]->commandButton_.SetColor(anSelectTexCol);
		buttonCommand_[ButtonName::Ybutton]->commandFrame_.SetColor(anSelectCol);
		buttonCommand_[ButtonName::Ybutton]->commandTex_.SetColor(anSelectTexCol);
	}
	else {
		buttonCommand_[ButtonName::Ybutton]->commandButton_.SetColor(Color::white);
		buttonCommand_[ButtonName::Ybutton]->commandFrame_.SetColor(mainColor_);
		buttonCommand_[ButtonName::Ybutton]->commandTex_.SetColor(Color::white);
	}
	//現在のコンボと比較してUIを表示する
	for (int i = 0; i < specialCommand_.size(); i++) {
		auto& specialCmd = specialCommand_[i];
		auto& cmdSprite = specialCommand_[i]->buttonCommand_;
		//色を暗くする
		if (i < commbo - 1) {
			cmdSprite.commandButton_.SetColor(anSelectTexCol);
			cmdSprite.commandFrame_.SetColor(anSelectCol);
			cmdSprite.commandTex_.SetColor(anSelectTexCol);
		}
		else {
			cmdSprite.commandButton_.SetColor(Color::white);
			cmdSprite.commandFrame_.SetColor(mainColor_);
			cmdSprite.commandTex_.SetColor(Color::white);
		}

		if (i < commbo) {
			cmdSprite.commandButton_.SetInvisivle(false);
			cmdSprite.commandFrame_.SetInvisivle(false);
			cmdSprite.commandTex_.SetInvisivle(false);
			specialCmd->isActive_ = true;
		}
		else {
			cmdSprite.commandButton_.SetInvisivle(true);
			cmdSprite.commandFrame_.SetInvisivle(true);
			cmdSprite.commandTex_.SetInvisivle(true);
			specialCmd->isActive_ = false;
		}
		//アクティブになったらUIを動かす
		if (specialCmd->isActive_) {
			specialCmd->easeTimer_.AddTime();
		}
		else {
			specialCmd->easeTimer_.Reset();
		}
		//イージングで動かす
		Vector2 cmdButtonPos = cmdSprite.commandButton_.GetPos();
		Vector2 cmdFramePos = cmdSprite.commandFrame_.GetPos();
		Vector2 cmdTexPos = cmdSprite.commandTex_.GetPos();
		cmdButtonPos.x = Easing::Back::easeOut(specialCmd->easeStartButtonPosX_, specialCmd->easeEndButtonPosX_, specialCmd->easeTimer_.GetTimeRate());
		cmdFramePos.x = Easing::Back::easeOut(specialCmd->easeStartFramePosX_, specialCmd->easeEndFramePosX_, specialCmd->easeTimer_.GetTimeRate());
		cmdTexPos.x = Easing::Back::easeOut(specialCmd->easeStartTexPosX_, specialCmd->easeEndTexPosX_, specialCmd->easeTimer_.GetTimeRate());
		cmdSprite.commandButton_.SetPos(cmdButtonPos);
		cmdSprite.commandFrame_.SetPos(cmdFramePos);
		cmdSprite.commandTex_.SetPos(cmdTexPos);

		cmdSprite.commandButton_.Update();
		cmdSprite.commandFrame_.Update();
		cmdSprite.commandTex_.Update();
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
	for (auto& sprite : specialCommand_) {
		auto& cmdSprite = sprite->buttonCommand_;
		cmdSprite.commandFrame_.Draw();
		cmdSprite.commandButton_.Draw();
		cmdSprite.commandTex_.Draw();
	}
}

void CommandSprite::ResetEase()
{
	
}
#pragma endregion
