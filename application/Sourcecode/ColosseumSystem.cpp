#include "ColosseumSystem.h"
#include "Texture.h"
#include "WinAPI.h"
#include <imgui.h>
#include "EnemyLoader.h"
#include "mSound.h"
#include "SceneManager.h"
#include "Easing.h"
#include "GameSpeed.h"

ColosseumSystem::ColosseumSystem()
{
	blindSprite_ = std::make_unique<Sprite>();
	//retrySprite_ = std::make_unique<Sprite>();
	//titleSprite_ = std::make_unique<Sprite>();
	
	blindSprite_->Ini();
	//retrySprite_->Ini();
	//titleSprite_->Ini();
	
	blindSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	//retrySprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Retry"));
	//titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("TitleTex"));
	
	blindSprite_->SetAnchor({ 0,0 });
	blindSprite_->SetColor(Color(0, 0, 0, 0));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.4f
	};
	//retrySprite_->SetPos(pos);
	pos.y = WinAPI::GetWindowSize().y / 1.2f;
	//titleSprite_->SetPos(pos);

	isStart_ = false;
	isNext_ = false;
	isReset_ = false;
	
	
	clearBlankTimer_.SetLimitTime(200);
	blindTimer_.SetLimitTime(50);
	
	gameClearBlankTimer_.SetLimitTime(130);

	roundNum_ = 1;
	selectType_ = SelectType::Title;

	isClearCamera_ = false;
}

void ColosseumSystem::Update()
{
#ifdef _DEBUG
	ImGui::Begin("ColosseumSystem");

	if (ImGui::Button("Reset"))
	{
		isStart_ = false;
	}

	ImGui::End();
#endif // _DEBUG

	//Ready�\��
	if (isStart_ == false)
	{
		//�S�������Ȃ�����
		std::list<std::unique_ptr<IEnemy>>::iterator itr;
		for (itr = enemyManager_->GetEnemy()->begin(); itr != enemyManager_->GetEnemy()->end();)
		{
			itr->get()->SetIsCanMove(false);
			itr++;
		}
		player_->SetIsCanMove(false);
		player_->SetIsCanInput(false);
		
		//Ready���o���I�������
		if (readyGoSprite_.GetReadyTimer().GetIsEnd())
		{
			isStart_ = true;
			//�S��������悤�ɂ���
			for (itr = enemyManager_->GetEnemy()->begin(); itr != enemyManager_->GetEnemy()->end();)
			{
				itr->get()->SetIsCanMove(true);
				itr++;
			}
			player_->SetIsCanMove(true);
			player_->SetIsCanInput(true);
		}
	}

	if (isNext_ == false)
	{
		if (blindTimer_.GetTimer() > 0)
		{
			blindTimer_.AddTime(-1);
			Color color = blindSprite_->GetColor();
			color.a = 255.f * blindTimer_.GetTimeRate();
			blindSprite_->SetColor(color);
		}
		//�G��S���|������
		if (enemyManager_->GetEnemy()->size() <= 0)
		{
			if (isClearCamera_ == false) {
				isClearCamera_ = true;
				GameSpeed::SetGameSpeed(0.2f);
				player_->SetIsCanInput(false);
			}
			//���ׂẴ��E���h���N���A������
			if (roundNum_ >= maxRoundNum_)
			{
				clearType_ = ClearType::GameClear;
				gameClearBlankTimer_.AddTime(1);
				clearSprite_.SetIsStart(true);

				if (gameClearBlankTimer_.GetIsEnd()) {
					isClear_ = true;
				}
			}
			//�܂����̃��E���h������ꍇ
			else 
			{
				clearType_ = ClearType::NextRound;
				clearBlankTimer_.AddTime(1);
				if (clearBlankTimer_.GetIsEnd())
				{
					GameSpeed::SetGameSpeed(1.f);
					isNext_ = true;
					roundNum_++;
					blindTimer_.Reset();
				}
			}
		}
	}
	else
	{
		blindTimer_.AddTime(1);
		Color color = blindSprite_->GetColor();
		color.a = 255.f * blindTimer_.GetTimeRate();
		blindSprite_->SetColor(color);

		if (blindTimer_.GetIsEnd())
		{
			Reset();
		}
	}
	
	readyGoSprite_.Update(isStart_);
	clearSprite_.Update(gameClearBlankTimer_);
	blindSprite_->Update();
}

void ColosseumSystem::DrawSprite()
{
	blindSprite_->Draw();
	readyGoSprite_.Draw(isStart_);

	clearSprite_.Draw();
	if (isClear_)
	{
		selectSprite_.Draw();
	}
}

void ColosseumSystem::Reset()
{
	isReset_ = true;
	isStart_ = false;
	isNext_ = false;
	isClearCamera_ = false;
	
	clearBlankTimer_.Reset();
	clearSprite_.Reset();
	readyGoSprite_.Reset();

	GameSpeed::SetGameSpeed(1.f);
}

void ColosseumSystem::ClearUpdate()
{
	player_->SetIsCanMove(false);
	player_->SetIsCanInput(false);
	//���j���[�I��
	if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
		Controller::GetTriggerButtons(PAD::INPUT_UP))
	{
		SoundManager::Play("SelectSE");
		bool type = (selectType_ == SelectType::Retry);
		//���ݑI�����Ă�����̂��R���e�B�j���[��������
		selectType_ = (type) ? SelectType::Title : SelectType::Retry;
	}

	selectSprite_.Update((int32_t)selectType_);
	//����
	if (Controller::GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE))
	{
		GameSpeed::SetGameSpeed(1.f);
		if (SceneManager::GetIsSetNext() == false)
		{
			SoundManager::Play("EnterSE", false, 1.5f);
		}
		if (selectType_ == SelectType::Retry)
		{
			SceneManager::SetChangeStart(SceneName::Game);
		}
		else if (selectType_ == SelectType::Title)
		{
			SceneManager::SetChangeStart(SceneName::Title);
		}
	}
}

#pragma region ClearSprite
ClearSprite::ClearSprite()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Ini();
	sprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Clear"));

	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 4.0f
	};
	sprite_->SetPos(pos);
	sprite_->SetScale({ 2,2 });
	sprite_->SetColor({ 255,255,255,0 });

	shineTimer_.SetLimitTime(60);
}

void ClearSprite::Update(Timer timer)
{
	if (timer.GetIsEnd() == false) {
		Color col = sprite_->GetColor();
		col.a = 255.f * timer.GetTimeRate();

		float rot = Easing::Circ::easeOut(10.f, 0.f, timer.GetTimeRate());
		Vector2 scale = {
			Easing::Circ::easeOut(5.f,1.5f, timer.GetTimeRate()),
			Easing::Circ::easeOut(5.f,1.5f, timer.GetTimeRate())
		};
		sprite_->SetColor(col);
		sprite_->SetRot(rot);
		sprite_->SetScale(scale);
	}
	else {
		Color col = sprite_->GetColor();
		if (shineTimer_.GetTimer() == 0) {
			float colRGB = 1000.f;
			col = { colRGB ,colRGB ,colRGB ,255 };
		}
		shineTimer_.AddTime(1);

		if (col.r > 255.f) {
			col -= {25, 25, 25, 0};
		}

		sprite_->SetColor(col);
	}

	sprite_->Update();
}

void ClearSprite::Draw()
{
	sprite_->Draw();
}
void ClearSprite::Reset()
{
}
#pragma endregion

#pragma region ReadyGoSprite
ReadyGoSprite::ReadyGoSprite()
{
	readySprite_ = std::make_unique<Sprite>();
	goShapeSprite_ = std::make_unique<Sprite>();
	goSprite_ = std::make_unique<Sprite>();
	
	for (uint32_t i = 0; i < readyShapeSprite_.size(); i++)
	{
		readyShapeSprite_[i] = std::make_unique<Sprite>();
		readyShapeSprite_[i]->Ini();
		readyShapeSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("ReadyShape"));
		readyShapeSprite_[i]->SetColor(Color(255, 255, 255, 0));
		readyShapeSprite_[i]->SetPos({ 0,WinAPI::GetWindowSize().y / 4.0f });
		readyShapeSprite_[i]->SetRot(Radian(180.f * i));
	}
	
	readySprite_->Ini();
	goSprite_->Ini();
	goShapeSprite_->Ini();

	readySprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Ready"));
	goSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Go"));
	goShapeSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("GoShape"));

	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 4.0f
	};
	readySprite_->SetPos(pos);
	readySprite_->SetScale({ 2.f,2.f });
	readySprite_->SetColor(Color(255, 255, 255, 0));
	goSprite_->SetPos(pos);
	goShapeSprite_->SetPos(pos);
	goShapeSprite_->SetColor(Color(255, 255, 255, 100));

	readyTimer_.SetLimitTime(180);
	displayTimer_.SetLimitTime(100);
	rdyEaseTimer_.SetLimitTime(100);
}

void ReadyGoSprite::Update(bool flag)
{
	if (flag == false)
	{
		readyTimer_.AddTime(1);
		//Ready�X�v���C�g�̓����X�V
		ReadySpriteUpdate();
	}
	else {
		GoSpriteUpdate();
	}
	goShapeSprite_->Update();
	readySprite_->Update();
	goSprite_->Update();
}

void ReadyGoSprite::Draw(bool flag)
{
	if (flag == false)
	{
		for (uint32_t i = 0; i < readyShapeSprite_.size(); i++)
		{
			readyShapeSprite_[i]->Draw();
		}
		readySprite_->Draw();
	}
	else
	{
		if (displayTimer_.GetIsEnd() == false)
		{
			goShapeSprite_->Draw();
			goSprite_->Draw();
		}
	}
}
void ReadyGoSprite::Reset()
{
	readyTimer_.Reset();
	displayTimer_.Reset();
	rdyEaseTimer_.Reset();
}

void ReadyGoSprite::ReadySpriteUpdate()
{
	///Ready�̐F�ƃX�P�[���ύX
	rdyEaseTimer_.AddTime(1);
	float scale = Easing::Circ::easeOut(4.f, 1.f, rdyEaseTimer_.GetTimeRate());
	readySprite_->SetScale({ scale ,scale });
	Color color = readySprite_->GetColor();
	color.a = 255.f * rdyEaseTimer_.GetTimeRate();
	readySprite_->SetColor(color);

	float colorLight = readySprite_->GetColor().r;
	//Ready�X�v���C�g�����X�V
	if (readySprite_->GetScale().x <= 1)
	{
		int32_t limitTime = readyTimer_.GetLimitTimer() - rdyEaseTimer_.GetLimitTimer();
		float halflimitTime = limitTime / 2.f;
		limitTime /= 5;
		int32_t time = readyTimer_.GetTimer() - rdyEaseTimer_.GetTimer();

		int32_t absValue = abs((limitTime / 2) - time);

		float rate = 1.f - (float)absValue / (limitTime / 2.f);
		if (time < limitTime)
		{
			colorLight = 455.f + 600.f * fabs(rate);
			if (rate <= 0.f)SoundManager::Play("ShineSE");
		}

		color = { colorLight ,colorLight ,colorLight ,color.a };

		readySprite_->SetColor(color);
	}

	float rightStartPosX = 1050;
	float rightEndPosX = 530;
	float leftStartPosX = 310;
	float leftEndPosX = 730;

	float easerightPosx = Easing::Cubic::easeOut(rightStartPosX, rightEndPosX, rdyEaseTimer_.GetTimeRate());
	float easeleftPosx = Easing::Cubic::easeOut(leftStartPosX, leftEndPosX, rdyEaseTimer_.GetTimeRate());
	float alpha = Easing::Circ::easeOut(0.f, 200.f, rdyEaseTimer_.GetTimeRate());

	for (uint32_t i = 0; i < readyShapeSprite_.size(); i++)
	{
		Vector2 pos = {
			0,
			WinAPI::GetWindowSize().y / 4.0f
		};
		if (i == 0)pos.x = easerightPosx;
		if (i == 1)pos.x = easeleftPosx;
		readyShapeSprite_[i]->SetPos(pos);
		Color color = readyShapeSprite_[i]->GetColor();
		color.a = alpha;
		readyShapeSprite_[i]->SetColor(color);
		readyShapeSprite_[i]->Update();
	}
}
void ReadyGoSprite::GoSpriteUpdate()
{
	displayTimer_.AddTime(1);

	float rate = displayTimer_.GetTimeRate();
	float goScale = Easing::Circ::easeOut(4.f, 2.f, Min(rate * 2.f, 1.0f));
	float alpha = 500.f * (1.f - rate);
	goSprite_->SetScale(Vector2(goScale, goScale));
	Color color = goSprite_->GetColor();
	color.a = alpha;
	goSprite_->SetColor(color);

	float goShapeScale = Easing::Circ::easeOut(0.5f, 1.0f, Min(rate * 2.f, 1.0f));

	goShapeSprite_->SetScale(Vector2(goShapeScale, goShapeScale));
	Color  goShapeColor = goShapeSprite_->GetColor();
	goShapeColor.a = alpha;
	goShapeSprite_->SetColor(goShapeColor);
}
#pragma endregion

#pragma region SelectSprite
SelectSprite::SelectSprite()
{
	for (uint32_t i = 0; i < frameSprite_.size(); i++) {
		frameSprite_[i] = std::make_unique<Sprite>();
		texSprite_[i] = std::make_unique<Sprite>();

		frameSprite_[i]->Ini();
		texSprite_[i]->Ini();

		frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		Vector2 framePos = {
			WinAPI::GetWindowSize().x / 2.f,
			500.f+ 80.f * i
		};
		frameSprite_[i]->SetPos(framePos);
		frameSprite_[i]->SetScale(Vector2(0.5f,0.6f));
		texSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("SelectTex"));
		Vector2 texPos = {
			WinAPI::GetWindowSize().x / 2.f + 10 * i,
			500.f + 80.f * i
		};
		Vector2 leftTopPos = {
			160.f * i,
			0
		};
		Vector2 texSize = {
			160.f,
			38
		};
		Vector2 scale = {
			(1.f / 3.f) * 0.6f,
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

void SelectSprite::Update(int32_t index)
{
	Color selectColor = { 230,50,50,255 };
	Color unSelectColor = { 0,35,255,255 };

	for (uint32_t i = 0; i < frameSprite_.size(); i++) {

		if (i == index) {
			frameSprite_[i]->SetColor(selectColor);
			frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));

			Vector2 pos = {
				frameSprite_[i]->GetPos().x + frameSprite_[i]->GetTexture().size_.x / 4.5f,
				frameSprite_[i]->GetPos().y - frameSprite_[i]->GetTexture().size_.y / 4.f
			};
			selectParticle_.SetPos(pos);
		}
		else {
			frameSprite_[i]->SetColor(unSelectColor);
			frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		}

		frameSprite_[i]->Update();
		texSprite_[i]->Update();
	}
	selectParticle_.Update();
}

void SelectSprite::Draw()
{
	for (uint32_t i = 0; i < frameSprite_.size(); i++) {
		frameSprite_[i]->Draw();
		texSprite_[i]->Draw();

		frameSprite_[i]->DrawImGui();
		texSprite_[i]->DrawImGui();
	}
	selectParticle_.Draw();
}
#pragma endregion