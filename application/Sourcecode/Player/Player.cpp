#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"
#include "mSound.h"

Player::Player() : IActor()
{
	controller_ = Controller::GetInstance();

	gravity_.SetAddValue({ 0,-0.01f,0 });

	// 入力されている方向の角度
	inputAngle_ = 0.0f;

	//移動速度
	moveSpeed_ = 0.02f;

	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//着地硬直時間
	landingTimer_.SetLimitTime(7);

	col_.radius = obj_->GetTransform()->scale_.x;
	damageCol_.radius = obj_->GetTransform()->scale_.x;

	attack_.SetPlayer(&state_);
	damageCoolTime_.SetLimitTime(50);
	maxHealth_ = 100;
	health_ = maxHealth_;
	isAlive_ = true;
	guard_.SetPlayer(this);
	knockDecreaseValue = 0.005f;

	sword_.SetAttackManager(&attack_);
	sword_.SetParent(obj_.get());
}

void Player::PreUpdate()
{
	addVec_ = { 0,0,0 };
	//プレイヤーの状態更新
	StateUpdate();
	//攻撃しているとき　&& 着地しているとき
	if (GetIsCanMove())
	{
		// 入力方向ベクトルを更新
		InputVecUpdate();
	}
	//ドッジロール
	if (GetIsCanDodge())
	{
		if (controller_->GetTriggerButtons(PAD::INPUT_X))
		{
			dodgeRoll_.Begin(moveVec2_.normalize());
		}
	}

	addVec_ += dodgeRoll_.GetDodgeVec();

	dodgeRoll_.Update();

	//重力
	GravityUpdate();

	ColPosUpdate();

	damageCoolTime_.AddTime(1);
	hpGauge_.Update(maxHealth_, health_);
}

void Player::PostUpdate()
{
	if (GetIsCanJump())
	{
		JumpUpdate();
	}
	if (GetIsCanAttack())
	{
		attack_.Update();
	}
	if (GetIsCanGuard())
	{
		if (controller_->GetTriggerButtons(PAD::INPUT_X))
		{
			//空中にいるとき、ノックバックの時攻撃の時はガードができない
			SoundManager::Play("GuardSE", false, 0.5f);
			guard_.Init();
		}
	}

	guard_.Update();
	sword_.Update();

	//当たり判定でgravityの値を変化させてから
	//PostUpdateでaddVec_に代入している
	ObjUpdate();
	if (attack_.GetLockOnEnemy() != nullptr)
	{
		lockOnVec_ =
			attack_.GetLockOnEnemy()->GetWorldTransform()->position_ - obj_->GetTransform()->position_;
	}

	if (health_ <= 0)
	{
		isAlive_ = false;
	}
}

void Player::GravityUpdate()
{
	gravity_.SetAddValue({ 0,-0.01f,0 });
	gravity_.Update();
}
void Player::ColPosUpdate()
{
	//モデルの原点を下にしているためその分ずらす
	Vector3 colPos = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y + obj_->WT_.scale_.y,
		obj_->GetTransform()->position_.z,
	};

	col_.SetPos(colPos);
	damageCol_.SetPos(colPos);
}
#pragma region 入力
void Player::InputVecUpdate()
{
	moveVec2_ = { 0,0 };
	Vector3 sideVec;
	Vector3 upVec = { 0,1,0 };
	Vector2 inputVec;// -> 入力されているベクトル

	//プレイヤーの正面ベクトル
	frontVec_ =
		Camera::scurrent_->target_ - Camera::scurrent_->eye_;
	frontVec_ = frontVec_.normalize();

	sideVec = upVec.cross(frontVec_);
	sideVec = sideVec.normalize();

	// コントローラーが接続されていたら
	if (controller_->GetActive())
	{
		// 左スティックの入力方向ベクトル取得
		inputVec = controller_->GetLStick() / 3276.8f;
		//カメラから見た左右手前奥移動
		moveVec2_.x = (frontVec_.z * -inputVec.x) + (sideVec.z * inputVec.y);
		moveVec2_.y = (frontVec_.z * inputVec.y) + (sideVec.z * inputVec.x);
		moveVec2_.x = -moveVec2_.x;
		moveVec2_ *= moveSpeed_;
	}

	addVec_ += {moveVec2_.x, 0, moveVec2_.y};

	/*obj_->GetTransform()->position_ = {
		Clamp(obj_->GetTransform()->position_.x, -77.f, 77.f),
		Clamp(obj_->GetTransform()->position_.y, 0.f, 100.f),
		Clamp(obj_->GetTransform()->position_.z, -77.f, 77.f)
	};*/

	// 入力しているベクトルの角度を求める
	float inputAngle = Vec2Angle(moveVec2_);

	// 計算結果がオーバーフローしていなかったら値を更新
	if (inputAngle >= 0)
	{
		inputAngle_ = inputAngle;
	}
	if (controller_->GetLStick().x != 0 ||
		controller_->GetLStick().y != 0)
	{
		obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
	}
}


void Player::JumpUpdate()
{
	float jumpSpeed = 0.2f;
	int Maxjumptimer = 10;
	if (controller_->GetButtons(PAD::INPUT_A))
	{
		if (isJump_ == false)
		{
			if (jumpTime_ < Maxjumptimer)
			{
				jumpTime_++;

				gravity_.SetGrabity({ 0, jumpSpeed ,0 });
			}
		}
	}
	//押し戻し処理がまだ

	if (controller_->GetReleasButtons(PAD::INPUT_A))
	{
		isJump_ = true;
	}
}

void Player::StateUpdate()
{
	if (state_ != PlayerState::Knock)
	{
		state_ = PlayerState::Idle;
	}
	landingTimer_.AddTime(1);
	if (landingTimer_.GetIsEnd())
	{
		if (state_ != PlayerState::Knock)
		{
			if (isFloorCollision_ == false)
			{
				state_ = PlayerState::Jump;
			}
			if (attack_.GetIsAttacking())
			{
				state_ = PlayerState::Attack;
			}
			if (isFloorCollision_ == false && attack_.GetIsAttacking())
			{
				state_ = PlayerState::AirAttack;
			}
		}
	}
	else
	{
		state_ = PlayerState::Landing;
	}

	if (guard_.GetIsGurdNow())
	{
		state_ = PlayerState::Guard;
	}
}

void Player::Draw()
{
	Model::lightGroup_->SetCircleShadowCasterPos(0, obj_->WT_.position_);
	obj_->Draw();
	//if (attack_.GetIsAttacking()) {
	sword_.Draw();
	//}

#ifdef _DEBUG
	DrawImGui();

	attack_.DrawDebug();
	guard_.DrawDebug();
#endif // _DEBUG
}

void Player::DrawImGui()
{
	ImGui::Begin("Player");
	/* ここに追加したいGUIを書く */
	// Menu Bar
	float pos[3] = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y,
		obj_->GetTransform()->position_.z,
	};
	ImGui::SliderFloat3("Posision", pos, -100.0f, 100.0f);

	//回転
	float rot[3] = {
		obj_->GetTransform()->rotation_.x,
		obj_->GetTransform()->rotation_.y,
		obj_->GetTransform()->rotation_.z,
	};
	ImGui::SliderFloat3("Rotation", rot, -100.0f, 100.0f);

	//スケール
	float scale[3] = {
		obj_->GetTransform()->scale_.x,
		obj_->GetTransform()->scale_.y,
		obj_->GetTransform()->scale_.z,
	};
	ImGui::SliderFloat3("Scale", scale, -100.0f, 100.0f);

	std::string text = "State : ";
	if (state_ == PlayerState::Idle)		text += "Idle";
	if (state_ == PlayerState::Jump)		text += "Jump";
	if (state_ == PlayerState::Attack)		text += "Attack";
	if (state_ == PlayerState::AirAttack)	text += "AirAttack";
	if (state_ == PlayerState::Landing)		text += "Landing";
	if (state_ == PlayerState::Knock)		text += "Knock";
	if (state_ == PlayerState::Guard)		text += "Guard";
	if (state_ == PlayerState::DodgeRoll)	text += "DodgeRoll";

	ImGui::Text(text.c_str());
	float addvec[3] = { addVec_.x,addVec_.y, addVec_.z };
	ImGui::SliderFloat3("addvec", addvec, 0.f, 80.f, "x = %.3f");

	float gravity = gravity_.GetGravityValue().y;
	ImGui::SliderFloat("gravity", &gravity, -80.f, 0.f, "y = %.3f");

	if (ImGui::Button("Damage"))
	{
		Damage(10, { 0,0,0 });
	}
	if (ImGui::Button("Max_Heal"))
	{
		health_ = 100;
	}

	ImGui::End();

}

void Player::DrawSprite()
{
	hpGauge_.Draw();
}

void Player::FloorColision(Vector3 pos)
{
	//前フレームで地面に接していなかったとき
	if (isFloorCollision_ == false)
	{
		state_ = PlayerState::Landing;
		landingTimer_.Reset();

	}
	isFloorCollision_ = true;
	isJump_ = false;
	jumpTime_ = 0;

	obj_->WT_.SetPosition(pos);
}

void Player::WallColision()
{
	ColPosUpdate();
	ObjUpdate();
}

#pragma region それぞれ行動できる条件
bool Player::GetIsCanMove()
{
	if (isCanMove_ == false) return false;
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock &&
		state_ != PlayerState::Guard &&
		state_ != PlayerState::Landing)
	{
		return true;
	}
	return false;
}

bool Player::GetIsCanDodge()
{
	if (state_ == PlayerState::Idle)
	{
		if (addVec_.x != 0 ||
			addVec_.y != 0 ||
			addVec_.z != 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::GetIsCanGuard()
{
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock &&
		state_ != PlayerState::Guard &&
		state_ != PlayerState::Jump)
	{
		if (addVec_.x == 0 &&
			addVec_.y == 0 &&
			addVec_.z == 0)
		{
			return true;
		}
	}
	return false;
}

bool Player::GetIsCanJump()
{
	if (state_ != PlayerState::Attack &&
		state_ != PlayerState::AirAttack &&
		state_ != PlayerState::Knock &&
		state_ != PlayerState::Guard)
	{
		return true;
	}
	return false;
}

bool Player::GetIsCanAttack()
{
	return isCanMove_;
}
#pragma endregion

void Player::Damage(int32_t damage, Vector3 knockVec)
{
	health_ -= damage;
	knockVec_ = knockVec;
	damageCoolTime_.Reset();
	hpGauge_.Damage();
}

void Player::GuardHit(Vector3 knockVec)
{
	knockVec_ += knockVec;
	guard_.GuardHit();
	damageCoolTime_.Reset();
}

void Player::Reset()
{
	obj_->WT_.position_ = { 0,0,0 };
	attack_.SetLockOnEnemy(nullptr);
	inputAngle_ = 0;
	obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
}
