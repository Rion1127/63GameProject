#include "ColosseumSystem.h"
#include "Texture.h"
#include "WinAPI.h"
#include <imgui.h>
#include "EnemyLoader.h"
#include "mSound.h"
#include "SceneManager.h"
#include "Easing.h"

ColosseumSystem::ColosseumSystem()
{
	readySprite_ = std::make_unique<Sprite>();
	goSprite_ = std::make_unique<Sprite>();
	blindSprite_ = std::make_unique<Sprite>();
	clearSprite_ = std::make_unique<Sprite>();
	retrySprite_ = std::make_unique<Sprite>();
	titleSprite_ = std::make_unique<Sprite>();

	readySprite_->Ini();
	goSprite_->Ini();
	blindSprite_->Ini();
	clearSprite_->Ini();
	retrySprite_->Ini();
	titleSprite_->Ini();

	readySprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Ready"));
	goSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Go"));
	blindSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	clearSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Clear"));
	retrySprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Retry"));
	titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("TitleTex"));

	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 4.0f
	};
	readySprite_->SetPos(pos);
	readySprite_->SetScale({ 2.f,2.f });
	readySprite_->SetColor(Color(255, 255, 255, 0));
	goSprite_->SetPos(pos);
	clearSprite_->SetPos(pos);

	pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.4f
	};
	retrySprite_->SetPos(pos);

	pos.y = WinAPI::GetWindowSize().y / 1.2f;
	titleSprite_->SetPos(pos);

	blindSprite_->SetAnchor({ 0,0 });
	blindSprite_->SetColor(Color(0, 0, 0, 0));



	isStart_ = false;
	isNext_ = false;
	isReset_ = false;

	readyTimer_.SetLimitTime(180);
	displayTimer_.SetLimitTime(100);
	clearBlankTimer_.SetLimitTime(200);
	blindTimer_.SetLimitTime(50);
	//演出用イージングタイマー
	rdyEaseTimer_.SetLimitTime(100);

	roundNum_ = 1;
	selectType_ = SelectType::Title;
}

void ColosseumSystem::Update()
{
#ifdef _DEBUG
	ImGui::Begin("ColosseumSystem");

	if (ImGui::Button("Reset"))
	{
		isStart_ = false;
		readyTimer_.Reset();
		rdyEaseTimer_.Reset();
	}

	ImGui::End();
#endif // _DEBUG

	//Ready表示
	if (isStart_ == false)
	{
		readyTimer_.AddTime(1);
		//全員動けなくする
		std::list<std::unique_ptr<IEnemy>>::iterator itr;
		for (itr = enemyManager_->GetEnemy()->begin(); itr != enemyManager_->GetEnemy()->end();)
		{
			itr->get()->SetIsCanMove(false);
			itr++;
		}
		player_->SetIsCanMove(false);
		//Readyスプライトの動き更新
		ReadySpriteUpdate();

		//Ready演出が終わったら
		if (readyTimer_.GetIsEnd())
		{
			isStart_ = true;
			//全員動けるようにする
			for (itr = enemyManager_->GetEnemy()->begin(); itr != enemyManager_->GetEnemy()->end();)
			{
				itr->get()->SetIsCanMove(true);
				itr++;
			}
			player_->SetIsCanMove(true);
		}
	}
	//Go!!表示
	else
	{
		displayTimer_.AddTime(1);
	}

	if (isNext_ == false)
	{
		//if (blindTimer_.GetTimer() > 0)
		//{
		//	blindTimer_.AddTime(-1);
		//	Color color = blindSprite_->GetColor();
		//	color.a = 255.f * blindTimer_.GetTimeRate();
		//	blindSprite_->SetColor(color);
		//}
		////敵を全員倒したら
		//if (enemyManager_->GetEnemy()->size() <= 0)
		//{
		//	//すべてのラウンドをクリアしたら
		//	if (roundNum_ >= maxRoundNum_)
		//	{
		//		isClear_ = true;
		//	}
		//	//まだ次のラウンドがある場合
		//	else if (roundNum_ < maxRoundNum_)
		//	{
		//		clearBlankTimer_.AddTime(1);
		//		if (clearBlankTimer_.GetIsEnd())
		//		{
		//			isNext_ = true;
		//			roundNum_++;
		//			blindTimer_.Reset();
		//		}
		//	}
		//}
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
	//すべてのラウンドが終了したときの処理
	if (isClear_)
	{
		ClearUpdate();
	}

	clearSprite_->Update();
	readySprite_->Update();
	goSprite_->Update();
	blindSprite_->Update();
}

void ColosseumSystem::DrawSprite()
{
	if (isStart_ == false)
	{
		readySprite_->Draw();
	}
	else
	{
		if (displayTimer_.GetIsEnd() == false)
		{
			goSprite_->Draw();
		}
	}
	if (isNext_)
	{
		blindSprite_->Draw();
	}

	if (isClear_)
	{
		clearSprite_->Draw();
		retrySprite_->Draw();
		titleSprite_->Draw();
	}

	
}

void ColosseumSystem::Reset()
{
	isReset_ = true;
	isStart_ = false;
	isNext_ = false;
	readyTimer_.Reset();
	displayTimer_.Reset();
	clearBlankTimer_.Reset();
}

void ColosseumSystem::ReadySpriteUpdate()
{
	rdyEaseTimer_.AddTime(1);
	float scale = Easing::Circ::easeOut(4.f,1.f, rdyEaseTimer_.GetTimeRate());
	readySprite_->SetScale({ scale ,scale });
	Color color = readySprite_->GetColor();
	color.a = 255.f * rdyEaseTimer_.GetTimeRate();
	readySprite_->SetColor(color);

	float colorLight = readySprite_->GetColor().r;

	if (readySprite_->GetScale().x <= 1)
	{
		int32_t limitTime = readyTimer_.GetLimitTimer() - rdyEaseTimer_.GetLimitTimer();
		limitTime /= 5;
		int32_t time = readyTimer_.GetTimer() - rdyEaseTimer_.GetTimer();

		int32_t absValue = abs((limitTime / 2) - time);

		float rate = 1.f - (float)absValue / (limitTime / 2.f);
		if (time < limitTime)
		{
			colorLight = 255.f + 600.f * fabs(rate);
		}

		color = { colorLight ,colorLight ,colorLight ,color.a };
		
		readySprite_->SetColor(color);
	}
}

void ColosseumSystem::ClearUpdate()
{
	//メニュー選択
	if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_DOWN) ||
		Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_UP))
	{
		SoundManager::Play("SelectSE");
		bool type = (selectType_ == SelectType::Retry);
		//現在選択しているものがコンティニューだったら
		selectType_ = (type) ? SelectType::Title : SelectType::Retry;
	}

	Color selectColor = { 200,50,50,255 };
	if (selectType_ == SelectType::Retry)
	{
		retrySprite_->SetColor(selectColor);
		titleSprite_->SetColor(Color(255, 255, 255, 255));

	}
	else if (selectType_ == SelectType::Title)
	{
		retrySprite_->SetColor(Color(255, 255, 255, 255));
		titleSprite_->SetColor(selectColor);
	}
	//決定
	if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE))
	{
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
	retrySprite_->Update();
	titleSprite_->Update();
}
