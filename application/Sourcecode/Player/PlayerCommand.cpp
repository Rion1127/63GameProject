#include "PlayerCommand.h"
#include "mInput.h"
#include <imgui.h>
#include "Texture.h"
#include "Easing.h"
#include "mSound.h"

PlayerCommand::PlayerCommand()
{
	for (uint32_t i = 0; i < frame_.size(); i++) {
		basePos_[i] = {120,550};
		basePos_[i] = {
			basePos_[i].x,
			basePos_[i].y + i * (32.f * 1.2f)
		};

		frame_[i] = std::make_unique<Sprite>();
		frame_[i]->Ini("");
		frame_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("CommandFrame"));
		frame_[i]->SetPos(basePos_[i]);
		frame_[i]->SetScale(Vector2(0.5f,0.6f));
		frame_[i]->SetColor(Color(255, 255, 255, 200));

		commandTex_[i] = std::make_unique<Sprite>();
		commandTex_[i]->Ini("");
		commandTex_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("CommandTex"));
		commandTex_[i]->SetPos(basePos_[i]);
		commandTex_[i]->SetTex_LeftTop(Vector2(i * 160.f, 1));
		commandTex_[i]->SetTex_Size(Vector2(160.f, 36));
		commandTex_[i]->SetScale(Vector2(0.3f, 0.6f));
		commandTex_[i]->SetAnchor(Vector2(0.8f,0.5f ));
		commandTex_[i]->SetColor(Color(255, 255, 255, 255));
	}

	commandTitle_ = std::make_unique<Sprite>();
	commandTitle_->Ini("");
	commandTitle_->SetTexture(TextureManager::GetInstance()->GetTexture("CommandTitle"));
	Vector2 titlePos = basePos_[0];
	titlePos.y -= (30.f * 1.2f);
	commandTitle_->SetPos(titlePos);
	commandTitle_->SetScale(Vector2(0.5f, 0.6f));
	commandTitle_->SetColor(Color(255, 255, 255, 255));
	commandTitle_->SetTex_LeftTop(Vector2(0, 5));

	easeTimer_.SetLimitTime(5);
}

void PlayerCommand::Update()
{
	if (Controller::GetTriggerButtons(PAD::INPUT_UP)) {
		commandNum_--;
		if (commandNum_ < 0) {
			commandNum_ = (uint16_t)Command::END - 1;
		}
		easeTimer_.Reset();
		SoundManager::Play("SelectSE",false,1.0f,0.5f);
	}
	if (Controller::GetTriggerButtons(PAD::INPUT_DOWN)) {
		commandNum_++;
		if (commandNum_ >= (uint16_t)Command::END) {
			commandNum_ = 0;
		}
		easeTimer_.Reset();
		SoundManager::Play("SelectSE", false, 1.0f, 0.5f);
	}
	selectCommand_ = (Command)commandNum_;

	attack_.SetLockOnEnemy(lockOnEnemy_);
	attack_.SetPlayer(playerState_);

	if (selectCommand_ == Command::Attack) {
		attack_.Attack();
	}
	else if (selectCommand_ == Command::Magic) {

	}

	SpriteUpdate();

	attack_.Update();
}

void PlayerCommand::SpriteUpdate()
{
	easeTimer_.AddTime(1);

	float startPos = basePos_[commandNum_].x;
	float endPos = basePos_[commandNum_].x + 20.f;

	Vector2 easePos = {
		Easing::Back::easeOut(startPos,endPos,easeTimer_.GetTimeRate()),
		basePos_[commandNum_].y,
	};

	if (selectCommand_ == Command::Attack) {
		frame_[(uint32_t)Command::Attack]->SetPos(easePos);
		frame_[(uint32_t)Command::Attack]->
			SetTexture(TextureManager::GetInstance()->
				GetTexture("CommandSelect"));
		frame_[(uint32_t)Command::Attack]->SetColor(Color(255, 255, 255, 255));
		commandTex_[(uint32_t)Command::Attack]->SetPos(easePos);

		frame_[(uint32_t)Command::Magic]->SetPos(basePos_[(uint32_t)Command::Magic]);
		commandTex_[(uint32_t)Command::Magic]->SetPos(basePos_[(uint32_t)Command::Magic]);
		frame_[(uint32_t)Command::Magic]->
			SetTexture(TextureManager::GetInstance()->
				GetTexture("CommandFrame"));
		frame_[(uint32_t)Command::Magic]->SetColor(Color(255, 255, 255, 200));
	}
	else if (selectCommand_ == Command::Magic) {
		frame_[(uint32_t)Command::Attack]->SetPos(basePos_[(uint32_t)Command::Attack]);
		frame_[(uint32_t)Command::Attack]->
			SetTexture(TextureManager::GetInstance()->
				GetTexture("CommandFrame"));
		frame_[(uint32_t)Command::Attack]->SetColor(Color(255, 255, 255, 200));
		commandTex_[(uint32_t)Command::Attack]->SetPos(basePos_[(uint32_t)Command::Attack]);

		frame_[(uint32_t)Command::Magic]->SetPos(easePos);
		frame_[(uint32_t)Command::Magic]->
			SetTexture(TextureManager::GetInstance()->
				GetTexture("CommandSelect"));
		frame_[(uint32_t)Command::Magic]->SetColor(Color(255, 255, 255, 255));
		commandTex_[(uint32_t)Command::Magic]->SetPos(easePos);
	}

	for (uint32_t i = 0; i < frame_.size(); i++) {
		frame_[i]->Update();
		commandTex_[i]->Update();
	}
	commandTitle_->Update();
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
	for (uint32_t i = 0; i < frame_.size(); i++) {
		frame_[i]->Draw();
		commandTex_[i]->Draw();
	}
	commandTitle_->Draw();
}
