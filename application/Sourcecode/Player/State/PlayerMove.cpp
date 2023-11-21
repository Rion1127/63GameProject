#include "PlayerMove.h"
#include "ParticleManager.h"
#include "ParticleDash.h"

/**
 * @file PlayerMove.cpp
 * @brief ステートマシン
 * @brief プレイヤーの移動している状態
 */

void PlayerMove::SetUp()
{
	shakeTimer_.SetLimitTime(40);
	dashParticleTimer_.SetLimitTime(60);
}

void PlayerMove::Update()
{
	player_->InputVecUpdate();

	if (player_->GetAddVec().x == 0 &&
		player_->GetAddVec().y == 0 &&
		player_->GetAddVec().z == 0)
	{
		player_->GoToState(PlayerState::Idle);
	}

	//動いているときは前傾姿勢にする
	float dashRadian = 15.f + 15.f * player_->GetIsDash();
	
	//ダッシュしているときはタイマーを速くする
	float addTime = 0;
	if (player_->GetIsDash())addTime = 1.5f;
	else addTime = 1;
	//左右に揺れる
	shakeTimer_.AddTime(addTime);
	const auto& timer = shakeTimer_.GetLimitTimer();
	const auto& rate = shakeTimer_.GetTimer();
	float shakeRadian = UpAndDown(timer, 0.2f, rate);
	shakeRadian = Angle(shakeRadian);

	if (shakeTimer_.GetIsEnd())
	{
		shakeTimer_.Reset();
	}

	dashParticleTimer_.AddTime(addTime);

	if (dashParticleTimer_.GetIsEnd())
	{
		Vector3 dashParticlePos =
			player_->GetWorldTransform()->position_ - player_->GetPlayerFrontVec();
		dashParticlePos.y = 0;

		std::shared_ptr<OneceEmitter> dashEmitter_ = std::make_shared<OneceEmitter>();
		dashEmitter_->particle = std::make_unique<ParticleDash>();
		dashEmitter_->addNum = 6;
		dashEmitter_->time = 20;
		dashEmitter_->pos = dashParticlePos;
		dashEmitter_->addVec = -player_->GetPlayerFrontVec();
		dashEmitter_->scale = 0.7f;
		ParticleManager::GetInstance()->
			AddParticle("Dash", dashEmitter_);

		dashParticleTimer_.Reset();
	}
	dashRadian;
	player_->SetObjAngleX(dashRadian);
	player_->SetObjAngleZ(shakeRadian);

	player_->DogeRoll();
	player_->Jump();
}

void PlayerMove::Draw()
{
}

void PlayerMove::CleanUp()
{
}

void PlayerMove::Move()
{
}
