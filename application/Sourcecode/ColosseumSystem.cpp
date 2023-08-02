#include "ColosseumSystem.h"
#include "Texture.h"
#include "WinAPI.h"
#include <imgui.h>
#include "EnemyLoader.h"

ColosseumSystem::ColosseumSystem()
{
	readySprite_ = std::make_unique<Sprite>();
	goSprite_ = std::make_unique<Sprite>();
	blindSprite_ = std::make_unique<Sprite>();
	clearSprite_ = std::make_unique<Sprite>();

	readySprite_->Ini();
	goSprite_->Ini();
	blindSprite_->Ini();
	clearSprite_->Ini();

	readySprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Ready"));
	goSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Go"));
	blindSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	clearSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Clear"));

	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f
	};
	readySprite_->SetPos(pos);
	goSprite_->SetPos(pos);
	clearSprite_->SetPos(pos);

	blindSprite_->SetAnchor({ 0,0 });
	blindSprite_->SetColor(Color(0, 0, 0, 0));



	isStart_ = false;
	isNext_ = false;
	isReset_ = false;

	easeTimer_.SetLimitTime(180);
	displayTimer_.SetLimitTime(100);
	clearBlankTimer_.SetLimitTime(200);
	blindTimer_.SetLimitTime(50);

	roundNum_ = 1;
}

void ColosseumSystem::Update()
{
	//Ready�\��
	if (isStart_ == false)
	{
		easeTimer_.AddTime(1);
		//�S�������Ȃ�����
		std::list<std::unique_ptr<IEnemy>>::iterator itr;
		for (itr = enemyManager_->GetEnemy()->begin(); itr != enemyManager_->GetEnemy()->end();)
		{
			itr->get()->SetIsCanMove(false);
			itr++;
		}
		player_->SetIsCanMove(false);

		//Ready���o���I�������
		if (easeTimer_.GetIsEnd())
		{
			isStart_ = true;
			//�S��������悤�ɂ���
			for (itr = enemyManager_->GetEnemy()->begin(); itr != enemyManager_->GetEnemy()->end();)
			{
				itr->get()->SetIsCanMove(true);
				itr++;
			}
			player_->SetIsCanMove(true);
		}
	}
	//Go!!�\��
	else
	{
		displayTimer_.AddTime(1);
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
			//���ׂẴ��E���h���N���A������
			if(roundNum_ >= maxRoundNum_)
			{
				isClear_ = true;
			}
			//�܂����̃��E���h������ꍇ
			else if (roundNum_ < maxRoundNum_)
			{
				clearBlankTimer_.AddTime(1);
				if (clearBlankTimer_.GetIsEnd())
				{
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
	//���ׂẴ��E���h���I�������Ƃ��̏���
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
	}

	ImGui::Begin("ColosseumSystem");

	ImGui::Text("isClear", "%d", isNext_);

	ImGui::End();
}

void ColosseumSystem::Reset()
{
	isReset_ = true;
	isStart_ = false;
	isNext_ = false;
	easeTimer_.Reset();
	displayTimer_.Reset();
	clearBlankTimer_.Reset();
}

void ColosseumSystem::ClearUpdate()
{
}
