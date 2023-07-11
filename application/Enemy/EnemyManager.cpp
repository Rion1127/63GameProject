#include "EnemyManager.h"
#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
EnemyManager::EnemyManager()
{
	lockOnobjTimer_.SetLimitTime(360);
	lockOnobjTimer_.SetIsLoop(true);
	/*enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-20, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, -20))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 20))));*/
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 1, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(10, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(-10, 2, 0))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyDummy>(Vector3(0, 2, 10))));

	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(5, 3, -10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(10, 3, -10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(-5, 3, -10))));
	enemys_.emplace_back(std::move(std::make_unique<EnemyAirDummy>(Vector3(-10, 3, -10))));

	lockOnSprite_.resize(2);
	for (size_t i = 0; i < 2; i++)
	{
		lockOnSprite_[i] = std::move(std::make_unique<Sprite>());
		lockOnSprite_[i]->Ini("");
		lockOnSprite_[i]->SetScale({ 0.5f,0.5f });
	}
	lockOnSprite_[0]->SetTexture(TextureManager::GetInstance()->GetTexture("LockOn1"));
	lockOnSprite_[1]->SetTexture(TextureManager::GetInstance()->GetTexture("LockOn2"));
}

void EnemyManager::PreUpdate()
{
	lockOnEnemy_ = nullptr;
	std::list<std::unique_ptr<IEnemy>>::iterator itr;
	for (itr = enemys_.begin(); itr != enemys_.end();)
	{
		//�G�����񂾂�v�f�������Ď��̃C�e���[�^�ֈڂ�
		if (itr->get()->GetIsDead())
		{
			itr = enemys_.erase(itr);
			continue;
		}
		itr->get()->PreUpdate();
		//���b�N�I�����Ă���G�̃A�h���X����
		if (itr->get()->GetIsLockOn()) {
			lockOnEnemy_ = itr->get();
		}
		//
		itr++;
	}
	//���b�N�I���I�u�W�F
	if (lockOnEnemy_ != nullptr) {
		lockOnobjTimer_.AddTime(1);
		

		lockOnSprite_[0]->SetRot(Radian((float)-lockOnobjTimer_.GetTimer()));
		lockOnSprite_[1]->SetRot(Radian((float)lockOnobjTimer_.GetTimer()));
		for (size_t i = 0; i < 2; i++)
		{
			lockOnSprite_[i]->SetInvisivle(false);
			Vector2 pos = TransformToVec2(*lockOnEnemy_->GetWorldTransform(),*Camera::scurrent_);
			lockOnSprite_[i]->SetPos(pos);
			lockOnSprite_[i]->Update();
		}
	}
	else
	{
		for (size_t i = 0; i < 2; i++)
		{
			lockOnSprite_[i]->SetInvisivle(true);
		}
	}
	//hp�Q�[�W�Ƀ��b�N�I�����̓G���Z�b�g
	hpGauge_.SetLockOnEnemy(lockOnEnemy_);
	hpGauge_.Update();
}

void EnemyManager::PostUpdate()
{
	for (auto& enemy : enemys_) {
		enemy->PostUpdate();
	}

	//�f�X�t���O�̗����������폜
	/*enemys_.remove_if([](std::unique_ptr<IEnemy>& bullet) {
		return bullet->GetIsDead();
		});*/
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::SpriteDraw()
{
	for (size_t i = 0; i < 2; i++)
	{
		lockOnSprite_[i]->Draw();
	}
	hpGauge_.Draw();
}
