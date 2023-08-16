#include "PauseMenu.h"
#include "Texture.h"
#include "mInput.h"
#include "SceneManager.h"
#include "mSound.h"

PauseMenu::PauseMenu()
{
	backSprite_ = std::make_unique<Sprite>();
	pauseSprite_ = std::make_unique<Sprite>();
	continueSprite_ = std::make_unique<Sprite>();
	titleSprite_ = std::make_unique<Sprite>();
	backSprite_->Ini();
	pauseSprite_->Ini();
	continueSprite_->Ini();
	titleSprite_->Ini();

	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	pauseSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Pause"));
	continueSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Continue"));
	titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("TitleTex"));

	backSprite_->SetColor(Color(0, 0, 0, 150.f));
	backSprite_->SetAnchor({ 0,0 });

	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 4.0f
	};
	pauseSprite_->SetPos(pos);

	pos = {
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
	}

	if (isPause_) {
		//���j���[�I��
		if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
			Controller::GetTriggerButtons(PAD::INPUT_UP))
		{
			SoundManager::Play("SelectSE");
			bool type = (selectType_ == SelectType::Continue);
			//���ݑI�����Ă�����̂��R���e�B�j���[��������
			selectType_ = (type) ? SelectType::Title : SelectType::Continue;
		}
		//�X�v���C�g�̐F��ς���
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
		//����
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
