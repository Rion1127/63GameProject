#include <imgui.h>
#include "Player.h"
#include "Util.h"
#include "Camera.h"
#include "mSound.h"

Player::Player() : IActor()
{

	gravity_.SetAddValue({ 0,-0.01f,0 });

	// 入力されている方向の角度
	inputAngle_ = 0.0f;

	//移動速度
	moveSpeed_ = 0.2f;
	walklimitValue_ = 0.7f;

	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//着地硬直時間
	landingTimer_.SetLimitTime(7);

	col_.radius = obj_->GetTransform()->scale_.x;
	damageCol_.radius = obj_->GetTransform()->scale_.x;

	damageCoolTime_.SetLimitTime(50);
	maxHealth_ = 100;
	health_ = maxHealth_;
	isAlive_ = true;
	guard_.SetPlayer(this);
	knockDecreaseValue = 0.005f;

	sword_.SetAttackManager(command_.GetAttackManager());
	sword_.SetParent(obj_.get());

	command_.SetPlayerInfo(&state_);
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
		if (Controller::GetTriggerButtons(PAD::INPUT_X))
		{
			dodgeRoll_.Begin(moveVec_.normalize());
			damageCoolTime_.Reset();
			uint32_t limit = dodgeRoll_.GetdodgeTimer().GetLimitTimer();
			damageCoolTime_.SetLimitTime(limit);
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
		command_.Update();
	}

	if (GetIsCanGuard())
	{
		if (Controller::GetTriggerButtons(PAD::INPUT_X))
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
	if (command_.GetLockOnEnemy() != nullptr)
	{
		lockOnVec_ =
			command_.GetLockOnEnemy()->GetWorldTransform()->position_ - obj_->GetTransform()->position_;
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
	moveVec_ = { 0,0 };
	Vector3 sideVec;
	Vector3 upVec = { 0,1,0 };
	inputVec_ = { 0,0 };
	
	//プレイヤーの正面ベクトル
	frontVec_ =
		Camera::scurrent_->target_ - Camera::scurrent_->eye_;
	frontVec_.y = 0;
	frontVec_ = frontVec_.normalize();

	sideVec = upVec.cross(frontVec_);
	sideVec = sideVec.normalize();

	// コントローラーが接続されていたら
	if (Controller::GetActive())
	{
		float inputlength = 0;
		// 左スティックの入力方向ベクトル取得
		inputVec_ = Controller::GetLStick() / 32768.f;
		inputlength = inputVec_.length();
		//スティックの傾きが小さければ歩く
		if (inputlength <= walklimitValue_) {
			inputVec_ = inputVec_.normalize() * 0.5f;
		}
		else {
			inputVec_ = inputVec_.normalize();
		}
		//カメラから見た左右手前奥移動
		moveVec_.x = -((frontVec_.z * -inputVec_.x) + (sideVec.z * inputVec_.y));
		moveVec_.y = (frontVec_.z * inputVec_.y) + (sideVec.z * inputVec_.x);
		moveVec_ *= moveSpeed_;
	}

	addVec_ += {moveVec_.x, 0, moveVec_.y};

	/*obj_->GetTransform()->position_ = {
		Clamp(obj_->GetTransform()->position_.x, -77.f, 77.f),
		Clamp(obj_->GetTransform()->position_.y, 0.f, 100.f),
		Clamp(obj_->GetTransform()->position_.z, -77.f, 77.f)
	};*/

	// 入力しているベクトルの角度を求める
	float inputAngle = Vec2Angle(moveVec_);

	// 計算結果がオーバーフローしていなかったら値を更新
	if (inputAngle >= 0)
	{
		inputAngle_ = inputAngle;
	}
	if (Controller::GetLStick().x != 0 ||
		Controller::GetLStick().y != 0)
	{
		obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
	}
}


void Player::JumpUpdate()
{
	float jumpSpeed = 0.2f;
	int Maxjumptimer = 10;
	
	if (Controller::GetTriggerButtons(PAD::INPUT_A)) {
		//ジャンプしたらジャンプ可能フラグをfalseにする
		isCanJump_ = false;
	}
	if (isCanJump_ == false) {
		//Aを押し続けた分高くジャンプする
		if (Controller::GetButtons(PAD::INPUT_A))
		{
			if (jumpTime_ < Maxjumptimer)
			{
				jumpTime_++;

				gravity_.SetGrabity({ 0, jumpSpeed ,0 });
			}
		}
	}
	//途中でAを離したら着地するまでジャンプできないようにする
	if (Controller::GetReleasButtons(PAD::INPUT_A))
	{
		jumpTime_ = Maxjumptimer;
	}
}

void Player::StateUpdate()
{
	if (state_ != PlayerState::Knock)
	{
		state_ = PlayerState::Idle;
		sword_.SetState(Sword::SwordState::Idle);
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
			if (command_.GetAttackManager()->GetIsAttacking())
			{
				state_ = PlayerState::Attack;
				sword_.SetState(Sword::SwordState::Attack);
			}
			if (isFloorCollision_ == false && command_.GetAttackManager()->GetIsAttacking())
			{
				state_ = PlayerState::AirAttack;
				sword_.SetState(Sword::SwordState::Attack);
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
		sword_.SetState(Sword::SwordState::Guard);
	}
	if (dodgeRoll_.GetIsDodge()) {
		state_ = PlayerState::DodgeRoll;
	}
}

void Player::Draw()
{
	Model::lightGroup_->SetCircleShadowCasterPos(0, obj_->WT_.position_);
	obj_->Draw();
	
	sword_.Draw();
	

#ifdef _DEBUG
	DrawImGui();

	command_.GetAttackManager()->DrawDebug();
	guard_.DrawDebug();
	command_.Draw();
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
	ImGui::Text("Pos x: %.2f y: %.2f z: %.2f ", pos[0], pos[1], pos[2]);
	//回転
	float rot[3] = {
		obj_->GetTransform()->rotation_.x,
		obj_->GetTransform()->rotation_.y,
		obj_->GetTransform()->rotation_.z,
	};
	ImGui::Text("Rot x: %.2f y: %.2f z: %.2f ", rot[0], rot[1], rot[2]);
	//スケール
	float scale[3] = {
		obj_->GetTransform()->scale_.x,
		obj_->GetTransform()->scale_.y,
		obj_->GetTransform()->scale_.z,
	};
	ImGui::Text("Scale x: %.2f y: %.2f z: %.2f ", scale[0], scale[1], scale[2]);

	std::string text = "State : ";
	if (state_ == PlayerState::Idle)		text += "Idle";
	if (state_ == PlayerState::Walk)		text += "Walk";
	if (state_ == PlayerState::Run)			text += "Run";
	if (state_ == PlayerState::Jump)		text += "Jump";
	if (state_ == PlayerState::Attack)		text += "Attack";
	if (state_ == PlayerState::AirAttack)	text += "AirAttack";
	if (state_ == PlayerState::Landing)		text += "Landing";
	if (state_ == PlayerState::Knock)		text += "Knock";
	if (state_ == PlayerState::Guard)		text += "Guard";
	if (state_ == PlayerState::DodgeRoll)	text += "DodgeRoll";

	ImGui::Text(text.c_str());
	float addvec[3] = { addVec_.x,addVec_.y, addVec_.z };
	ImGui::SliderFloat3("addvec", addvec, 0.f, 1.f, "x = %.3f");

	float gravity = gravity_.GetGravityValue().y;
	ImGui::SliderFloat("gravity", &gravity, -1.f, 0.f, "y = %.3f");

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
	command_.DrawSprite();
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
	isCanJump_ = true;
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
		state_ != PlayerState::DodgeRoll &&
		state_ != PlayerState::Landing)
	{
		return true;
	}
	return false;
}

bool Player::GetIsCanDodge()
{
	if (state_ == PlayerState::Idle &&
		state_ != PlayerState::DodgeRoll)
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
		state_ != PlayerState::Jump &&
		state_ != PlayerState::DodgeRoll)
	{
		if (inputVec_.x == 0 &&
			inputVec_.y == 0)
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
		state_ != PlayerState::Guard &&
		state_ != PlayerState::DodgeRoll)
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
	SoundManager::Play("HitSE", false, 0.5f);
}

void Player::GuardHit(Vector3 knockVec)
{
	knockVec_ += knockVec;
	guard_.GuardHit();
	damageCoolTime_.Reset();
	damageCoolTime_.SetLimitTime(50);
}

void Player::Reset()
{
	obj_->WT_.position_ = { 0,0,0 };
	command_.SetLockOnEnemy(nullptr);
	inputAngle_ = 0;
	obj_->WT_.rotation_ = { 0,Radian(inputAngle_) ,0 };
}
