#include "CollisionManager.h"
#include "ParticleManager.h"
#include "mSound.h"

void CollisionManager::Update()
{
	//���ƃv���C���[
	PlayerToFloor();
	//���ƓG
	EnemyToFloor();
	//���b�N�I������G
	EnemyLockOn();
	//�v���C���[�ƓG�����o��
	PlayerToEnemy();
	//�v���C���[�̍U���ƓG
	PlayerAttackToEnemy();
	//�G�U���ƃv���C���[
	EnemyAttackToPlayer();
}

void CollisionManager::PlayerToFloor()
{
	//���ƃv���C���[
	if (Sphere2PlaneCol(player_->GetCol(), floor_->GetPlaneCol()))
	{
		//�n�ʂɂ߂荞�܂Ȃ��悤�ɉ����o������
		while (true)
		{
			float checkValue = 0.01f;
			Sphere col = player_->GetCol();
			col.center.y -= checkValue;
			col.radius = player_->GetCol().radius;
			//�n�ʂɓ������Ă������ɉ����o���Ă���
			if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
			{
				player_->floorColision();
				break;
			}

			Vector3 playerPos = player_->GetWorldTransform()->position_;
			float upDist = 0.01f;
			playerPos.y += upDist;
			player_->SetAddPos({ 0,upDist,0 });
			player_->ColPosUpdate();
		}

	}
	else
	{
		player_->SetIsFloorCollision(false);
	}
}

void CollisionManager::EnemyToFloor()
{
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		//���ƓG
		if (Sphere2PlaneCol(enemy->GetCol(), floor_->GetPlaneCol()))
		{
			//�n�ʂɂ߂荞�܂Ȃ��悤�ɉ����o������
			while (true)
			{
				float checkValue = 0.01f;
				Sphere col = enemy->GetCol();
				col.center.y -= checkValue;
				col.radius = enemy->GetCol().radius;
				//�n�ʂɓ������Ă������ɉ����o���Ă���
				if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
				{
					enemy->FloorColision();
					break;
				}

				Vector3* enemyPos = &enemy->GetWorldTransform()->position_;
				float upDist = 0.01f;
				enemyPos->y += upDist;

				enemy->ColPosUpdate();
			}
		}
	}
}

void CollisionManager::EnemyLockOn()
{
	IEnemy* lockOnEnemy = nullptr;
	std::vector<IEnemy*> lockOnEnemys_;
	bool isHardLockOn = false;

	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		if (enemy->GetIsSoftLockOn() == false)
		{
			enemy->SetHardIsLockOn(false);
		}
		//��̂ł��n�[�h���b�N�����Ă�����t���O��true�ɂ���
		if (enemy->GetIsHardLockOn())
		{
			isHardLockOn = true;
			break;
		}
		//���b�N�I���p�̑傫�������蔻��
		enemy->SetSoftIsLockOn(false);
		Sphere serchCol = player_->GetCol();
		serchCol.radius *= 20.f;
		if (BallCollision(serchCol, enemy->GetCol()))
		{
			//���b�N�I�������ɂ���G��ǉ����Ă���
			lockOnEnemys_.push_back(enemy.get());
		}
	}
	if (isHardLockOn == false)
	{
		//���̑傫���l�����Ă���
		float dist2d = 5000.f;
		for (auto& enemy : lockOnEnemys_)
		{
			//�X�N���[�����W���擾���ĉ�ʂ̒����ɋ߂��G�����b�N�I������
			Vector2 ScreenPos = GetScreenPos(*enemy->GetWorldTransform(), *Camera::scurrent_);

			Vector2 halfWindowSize = WinAPI::GetWindowSize() / 2.f;

			Vector2 dist = halfWindowSize - ScreenPos;
			float length = dist.length();
			//��r���ĒZ��������G��������
			if (dist2d > length)
			{
				dist2d = length;
				lockOnEnemy = enemy;
			}
		}

		if (lockOnEnemy != nullptr)
		{
			lockOnEnemy->SetSoftIsLockOn(true);
		}
		player_->GetAttackManager()->SetLockOnEnemy(lockOnEnemy);
	}

	if (enemyManager_->GetLockOnEnemy() != nullptr)
	{
		//�����Ń��b�N�I������G��ݒ�
		if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_RIGHT_SHOULDER))
		{
			SoundManager::Play("lockOnSE", false, 0.2f);
			//���b�N�I�����Ă���Ή����A���Ă��Ȃ���΃��b�N�I��
			bool isLockOn = (enemyManager_->GetLockOnEnemy()->GetIsHardLockOn() == true) ?
				false : true;
			enemyManager_->GetLockOnEnemy()->SetHardIsLockOn(isLockOn);

			player_->GetAttackManager()->SetLockOnEnemy(enemyManager_->GetLockOnEnemy());
		}
	}
}

void CollisionManager::PlayerToEnemy()
{
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		if (BallCollision(player_->GetCol(), enemy->GetCol()))
		{
			Vector3 PtoEVec = player_->GetCol().center - enemy->GetCol().center;
			//�x�N�g���̒������擾
			float length = PtoEVec.length();
			PtoEVec = PtoEVec.normalize();
			//Y�����𖳌��ɂ��ĉ����o��
			PtoEVec.y = 0;
			//��̓����蔻��̔��a�̒����𑫂�
			float backLength = player_->GetCol().radius + enemy->GetCol().radius;
			//�x�N�g���̒����������Ă߂荞��ł��钷�������������߂�()
			backLength -= length;

			Vector3 pushBackVec = PtoEVec * backLength;

			player_->SetAddPos(pushBackVec / 2.f);
			enemy->AddVec(-pushBackVec / 2.f);
		}
	}
}

void CollisionManager::PlayerAttackToEnemy()
{
	IAttack* attackCol = player_->GetAttackManager()->GetNowAttack();
	if (attackCol != nullptr)
	{
		for (auto& enemy : *enemyManager_->GetEnemy())
		{
			for (auto& col : *attackCol->GetAttackCol())
			{
				if (enemy->GetDamageCoolTime().GetIsEnd())
				{
					if (BallCollision(col->col_, enemy->GetCol()))
					{
						//�v���C���[�̔��Ε����Ƀm�b�N�o�b�N����
						Vector3 knockVec = enemy->GetCol().center - player_->GetWorldTransform()->position_;
						knockVec.y = col->knockVecY;
						knockVec = knockVec.normalize();
						knockVec = knockVec * col->knockPower;
						//�G�̃m�b�N�o�b�N��R�͂��|����
						knockVec = knockVec * enemy->GetKnockResist();
						enemy->HitPlayerAttack(knockVec, col->damage, col->damageCoolTime);
						enemy->SetIsNock(true);

					

						Vector3 addVec = { 0.15f,0.15f,0.15f };

						ParticleManager::GetInstance()->AddParticle("HitAttack", 3, 40, enemy->GetCol().center, addVec, 0.7f);
						SoundManager::Play("HitSE", false, 0.2f);
					}
				}
			}
		}
	}
}

void CollisionManager::EnemyAttackToPlayer()
{
	for (auto& enemy : *enemyManager_->GetEnemy())
	{
		IAttack* attackCol = enemy->GetAttack();
		if (attackCol != nullptr)
		{
			for (auto& col : *attackCol->GetAttackCol())
			{
				if (player_->GetDamegeCoolTime()->GetIsEnd()) {
					if (BallCollision(col->col_, player_->GetCol()))
					{
						//�G�̔��Ε����Ƀm�b�N�o�b�N����
						Vector3 knockVec =
							player_->GetWorldTransform()->position_ - enemy->GetCol().center;
						knockVec.y = col->knockVecY;
						knockVec = knockVec.normalize();
						knockVec = knockVec * col->knockPower;
						//�G�̃m�b�N�o�b�N��R�͂��|����
						knockVec = knockVec * enemy->GetKnockResist();

						player_->SetKnockVec(knockVec);
						player_->SetAddPos({0,0,0});
						player_->GetDamegeCoolTime()->Reset();
						player_->SetState(PlayerState::Knock);

						Vector3 addVec = { 0.15f,0.15f,0.15f };
						ParticleManager::GetInstance()->
							AddParticle("HitAttack", 3, 40, player_->GetCol().center, addVec, 0.7f);
						SoundManager::Play("HitSE", false, 0.2f);
					}
				}
			}
		}
	}
}
