#include "ColosseumSystem.h"
#include "Texture.h"
#include "WinAPI.h"

ColosseumSystem::ColosseumSystem()
{
	readySprite_ = std::make_unique<Sprite>();
	goSprite_ = std::make_unique<Sprite>();
	readySprite_->Ini();
	goSprite_->Ini();
	readySprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Ready"));
	goSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Go"));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f
	};
	readySprite_->SetPos(pos);
	goSprite_->SetPos(pos);



	isStart_ = false;
	isClear_ = false;

	easeTimer_.SetLimitTime(180);
	displayTimer_.SetLimitTime(100);
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

	if (enemyManager_->GetEnemy()->size() <= 0)
	{
		isClear_ = true;
	}

	readySprite_->Update();
	goSprite_->Update();
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
}
