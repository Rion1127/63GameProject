#include "Guard.h"
#include "Player.h"
#include "mInput.h"
#include "Util.h"

/**
 * @file Guard.cpp
 * @brief プレイヤーのガード処理をしている
 */

GuardClass::GuardClass()
{
	col_.radius = 0.7f;
	colObj_ = MakeUnique<Object3d>();
	colObj_->SetModel(Model::CreateOBJ_uniptr("sphere", false));
	colObj_->SetAmbient("sphere", { 0.0f, 0, 1.0f });

	timer_.SetLimitTime(40);
	timer_.SetTime(timer_.GetLimitTimer());
	isGurdNow_ = false;

	enabledTime_ = { 5,30 };
}

void GuardClass::Init()
{
	const auto& enemy = player_->GetAttackManager()->GetLockOnEnemy();
	Vector3 frontVec;
	Vector3 lockOnVec;
	Vector2 frontVec2;
	frontDist_ = 0.8f;
	if (enemy != nullptr) {
		//敵をハードロックしているなら敵の方向に向く
		if (enemy->GetIsHardLockOn()) {
			frontVec = player_->GetWorldTransform()->position_;

			lockOnVec = player_->GetLockOnVec();
			//ロックオンしている敵へのベクトルをとる
			frontVec2 = {
				lockOnVec.x,
				lockOnVec.z
			};
			float rotY = Radian(Vec2Angle(frontVec2));
			player_->SetRot({ 0,rotY,0 });

			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(player_->GetWorldTransform()->rotation_.y),
				0,
				cosf(player_->GetWorldTransform()->rotation_.y),
			};

			frontVec = frontVec.normalize();
			frontVec = frontVec * frontDist_;
			frontVec.y = 0;
		}
		else {
			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(player_->GetWorldTransform()->rotation_.y),
				0,
				cosf(player_->GetWorldTransform()->rotation_.y),
			};
			//ロックオンしている敵へのベクトルをとる
			frontVec2 = {
				frontVec.x,
				frontVec.z
			};
		}
	}
	else {
		//回転情報から正面ベクトル(2D)を取得
		frontVec = {
			sinf(player_->GetWorldTransform()->rotation_.y),
			0,
			cosf(player_->GetWorldTransform()->rotation_.y),
		};
		//ロックオンしている敵へのベクトルをとる
		frontVec2 = {
			frontVec.x,
			frontVec.z
		};
	}

	col_.center = player_->GetCol().center + frontVec;
	col_.center.y += colObj_->GetTransform()->scale_.y;

	colObj_->GetTransform()->SetPosition(col_.center);
	Vector3 scale = { col_.radius ,col_.radius ,col_.radius };
	colObj_->GetTransform()->SetScale(scale);

	timer_.Reset();
	isGurdNow_ = true;
}

void GuardClass::Update()
{
	//enabledTime_の間だったら当たり判定を有効にする
	if (enabledTime_.start < timer_.GetTimer() &&
		enabledTime_.end > timer_.GetTimer()) {
		col_.isActive = true;
	}
	else {
		col_.isActive = false;
	}

	Vector3 frontVec = player_->GetWorldTransform()->position_;
	//回転情報から正面ベクトル(2D)を取得
	frontVec = {
		sinf(player_->GetWorldTransform()->rotation_.y),
		0,
		cosf(player_->GetWorldTransform()->rotation_.y),
	};

	frontVec = frontVec.normalize();
	frontVec = frontVec * frontDist_;
	frontVec.y = 0;
	col_.center = player_->GetCol().center + frontVec;
	col_.center.y += colObj_->GetTransform()->scale_.y;
	colObj_->GetTransform()->SetPosition(col_.center);

	timer_.AddTime(1);
	if (timer_.GetIsEnd()) {
		isGurdNow_ = false;
	}
	colObj_->Update();
}

void GuardClass::DrawDebug()
{
	if (col_.isActive) {
		colObj_->Draw();
	}
}

void GuardClass::GuardHit()
{
	//ガードが成功したらガード時間を延長
	timer_.Reset();
	timer_.SetTime(enabledTime_.start);
	Init();
}
