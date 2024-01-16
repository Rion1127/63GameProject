#include "EnemyShadow.h"
#include "mInput.h"
#include "Player.h"
#include <imgui.h>

#include "RRandom.h"
#include "Easing.h"

#include "AttackShadow.h"
#include "AttackJumpShadow.h"
#include "GameSpeed.h"

/**
 * @file EnemyShadow.cpp
 * @brief 地上の敵（シャドウ）のクラス
 */

EnemyShadow::EnemyShadow(const Vector3& pos, const Vector3& rot) :
	IEnemy(EnemyType::Ground, true, 80)
{
	name_ = "Shadow";
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("shadow", true, false));
	displayObj_ = std::move(std::make_unique<Object3d>());
	displayObj_->SetModel(Model::CreateOBJ_uniptr("shadow", true));
	displayObj_->SetShadowOffsetPos(Vector3(0,-0.8f,0));
	displayObj_->WT_.SetRotType(RotType::Quaternion);
	knockResist_ = { 1,1,1 };

	handObj_ = std::move(std::make_unique<Object3d>());
	handObj_->SetModel(Model::CreateOBJ_uniptr("shadowHand", true,false));
	handObj_->WT_.SetRotType(RotType::Quaternion);
	handObj_->SetScale(Vector3(0, 0, 0));

	obj_->GetTransform()->SetPosition(pos);
	obj_->GetTransform()->SetRotation(rot);
	displayObj_->GetTransform()->SetPosition(pos);
	displayObj_->GetTransform()->SetRotation(rot);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(200);

	sinkTimer_.SetLimitTime(40);

	slimeTimer_.SetLimitTime(7);

	knockRotTimer_.SetLimitTime(60);
	knockEndRot_ = -810.f;

	followLength = 5.f;
	moveSpeed = 0.1f;
	randRange_ = 30;
	isWanderInit_ = false;

	handAxisX_ = IdentityQuaternion();

	//プライオリティに行動パターンを登録
	priority_.insert(std::make_pair(State::Idle, 0));
	priority_.insert(std::make_pair(State::Following, 0));
	priority_.insert(std::make_pair(State::Wander, 0));
	priority_.insert(std::make_pair(State::HideMove, 0));
	priority_.insert(std::make_pair(State::Attack, 0));
	priority_.insert(std::make_pair(State::JumpAttack, 0));
	priority_.insert(std::make_pair(State::KnockBack, 0));

	gravity_.SetAddValue(Vector3(0,-0.007f,0));
	stateIdleTime_ = 80;
	stateFollowingTime_ = 120;
	stateDownTime_ = 160;

	randKnockValueX_.x = -0.7f;
	randKnockValueX_.y = -0.3f;
	randKnockValueY_.x = -0.4f;
	randKnockValueY_.y = 0.4f;
	baseScale_ = 0.8f;
	hideScale_ = 0.1f;
	collapseScale_ = 0.4f;
	collapseRateLimit_ = 0.2f;
	slerpSpeed_ = 0.2f;
	downRecoverRate_ = 0.6f;
	handScale_ = 1.5f;
	handRotLimit_ = 720.f;
}

void EnemyShadow::SetIsNock(bool flag)
{
	isKnock_ = flag;
	actionTimer_.SetLimitTime(40);
	actionTimer_.Reset();
}

void EnemyShadow::SetIsDown(bool flag)
{
	isDown_ = flag;
	actionTimer_.SetLimitTime(40);
	actionTimer_.Reset();
}

void EnemyShadow::SetState(State state)
{
	state_ = state;
	actionTimer_.Reset();
	float randRange = (float)RRandom::Rand(-randRange_, randRange_);
	if (State::Idle == state_)
	{
		actionTimer_.SetLimitTime(stateIdleTime_ + randRange);
	}
	else if (State::Following == state_)
	{
		actionTimer_.SetLimitTime(stateFollowingTime_ + randRange);
	}
}

void EnemyShadow::Draw()
{
	displayObj_->Draw();
	handObj_->Draw();
	if (attack_ != nullptr)
	{
		//attack_->DrawCol();
	}
}

void EnemyShadow::MoveUpdate()
{
	//ステータスごとの動きを追加
	void (EnemyShadow:: * Action[]) () =
	{
		&EnemyShadow::Idle,
		&EnemyShadow::Following,
		&EnemyShadow::Wander,
		&EnemyShadow::HideMove,
		&EnemyShadow::Attack,
		&EnemyShadow::JumpAttack,
		&EnemyShadow::KnockBack,
		&EnemyShadow::Down,
	};

	if (isKnock_)
	{
		//ダウン状態の時はノック状態にしない
		if (state_ != State::Down) {
			state_ = State::KnockBack;
		}
	}
	if (isDown_) {
		//フィニッシュで飛ばされたときの動き
		KnockRotate();
	}
	//プレイヤーへのベクトル更新
	UpdateEtoPVec();

	actionTimer_.AddTime(GameSpeed::GetEnemySpeed());
	//実行
	(this->*Action[(int32_t)state_])();

	if (actionTimer_.GetIsEnd())
	{
		SortPriority();
	}

	float offsetPos = displayObj_->GetScale().y * baseScale_;
	displayObj_->SetShadowOffsetPos(Vector3(0, -offsetPos, 0));

	handObj_->Update();
}

void EnemyShadow::DrawSprite()
{

}

void EnemyShadow::DamageUpdate()
{
	EToPQuaternion_ = VecToDir(EtoPVec_);
	knockQuaternion_ = IdentityQuaternion();
	//後ろへランダムな方向にのけぞる
	knockQuaternion_.x = RRandom::RandF(randKnockValueX_.x, randKnockValueX_.y);
	knockQuaternion_.y = RRandom::RandF(randKnockValueX_.x, randKnockValueX_.y);
	
	obj_->WT_.SetQuaternion(EToPQuaternion_);

	obj_->SetScale(Vector3(1, 1, 1));

	state_ = State::KnockBack;

	handObj_->SetScale(Vector3(0,0,0));
	handObj_->SetIsVisible(false);
	knockRotTimer_.Reset();
}

void EnemyShadow::FloorCollisionDerived()
{
	//前フレームで地面に接していなかったとき
	if (isFloorCollision_ == false)
	{
		//フィニッシュ技を食らってダウンする
		if (isDown_) {
			isKnock_ = false;
			isDown_ = false;
			state_ = State::Down;
			actionTimer_.Reset();
			actionTimer_.SetLimitTime(stateDownTime_);

			slimeTimer_.SetLimitTime(30);
			slimeTimer_.Reset();
		}
	}
}

void EnemyShadow::Idle()
{
	stateName_ = "Idle";
}

void EnemyShadow::Following()
{
	stateName_ = "Following";

	float length = EtoPVec_.length();
	addVec_ = EtoPVec_.normalize() * moveSpeed;

	obj_->WT_.SetQuaternion(VecToDir(EtoPVec_));
	//一定距離まで近づいたらOR一定時間追いかけたら
	if (length < followLength || actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		actionTimer_.Reset();
	}
}

void EnemyShadow::Wander()
{
	//地面に潜って移動する
	stateName_ = "Wander";
	WanderInit();
	col_.isActive = false;

	//移動する
	if (spline_.GetisEnd() == false)
	{
		if (sinkTimer_.GetIsEnd() == false)
		{
			float t = sinkTimer_.GetTimeRate();
			obj_->WT_.scale_.y = Easing::Sine::easeIn(t, baseScale_, hideScale_ - baseScale_, 1.0f);
			sinkTimer_.AddTime(1 * GameSpeed::GetEnemySpeed());
		}
		else
		{
			//スプライン曲線更新
			spline_.Update(GameSpeed::GetEnemySpeed());
			obj_->WT_.SetPosition(spline_.GetNowPoint());
			if (spline_.GetisEnd()) {
				sinkTimer_.Reset();
			}
		}
		//進行方向に回転

		Vector2 moveVec = {
			spline_.GetHeadingVec().x,
			spline_.GetHeadingVec().z
		};

		float angle = Vec2Angle(moveVec.normalize());

		obj_->WT_.SetQuaternion(MakeAxisAngle(Vector3(0,1,0), Radian(angle)));
	}
	//移動が終わったら別のパターンへ
	else
	{
		if (sinkTimer_.GetIsEnd() == false)
		{
			float t = sinkTimer_.GetTimeRate();
			obj_->WT_.scale_.y = Easing::Sine::easeIn(t, hideScale_, 1.0f, 1.0f);
			sinkTimer_.AddTime(1 * GameSpeed::GetEnemySpeed());
		}
		else
		{
			sinkTimer_.Reset();
			SetState(State::Idle);
			col_.isActive = true;
		}
	}

	col_.radius = obj_->WT_.scale_.y;
}

void EnemyShadow::HideMove()
{
	stateName_ = "HideMove";
}

void EnemyShadow::Down()
{
	slimeTimer_.AddTime(1);

	float scaleZ = 0;
	//地面に潰れる
	if (actionTimer_.GetTimeRate() <= collapseRateLimit_) {
		scaleZ = Easing::Bounce::easeOut(actionTimer_.GetTimeRate(), 1.f, 1.f - collapseScale_, collapseRateLimit_);
	}
	//潰れたスケールが元に戻る
	else {
		float maxRate = downRecoverRate_;
		float rate = actionTimer_.GetTimeRate() - collapseRateLimit_;
		rate = Min(maxRate, rate);
		scaleZ = Easing::Bounce::easeOut(rate, collapseScale_, 1.f - collapseScale_, maxRate);
	}
	col_.radius = scaleZ;

	obj_->SetScale(Vector3(1, 1, scaleZ));
	if (actionTimer_.GetTimeRate() >= downRecoverRate_) {
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(EToPQuaternion_, slerpSpeed_);
	}
	else {
		auto resultQ = EToPQuaternion_ * Quaternion(-1, 0, 0, 1);
		obj_->GetTransform()->SetQuaternion(obj_->GetTransform()->quaternion_.Slerp(resultQ, slerpSpeed_));
	}
	if (actionTimer_.GetIsEnd())
	{
		knockRotTimer_.Reset();
		slimeTimer_.Reset();
		obj_->SetScale(Vector3(1, 1, 1));
		col_.radius = 1;
		SetState(State::Idle);
		attack_.reset();
	}
}

void EnemyShadow::Attack()
{
	handObj_->SetIsVisible(true);
	if (attackTimer_.GetIsEnd()) {
		if (attack_->GetLockOnActor() == nullptr) {
			
		}
		else {
			handAxisX_ = handAxisX_.Slerp(Quaternion(1, 0, 0, 0.8f), slerpSpeed_);
			handResultQ_ = displayObj_->WT_.quaternion_;
			handResultQ_ = handResultQ_ * handAxisX_;
			handObj_->WT_.quaternion_ = handResultQ_;

			stateName_ = "Attack";
			attack_->Update();
			attack_->SetNowTime(actionTimer_.GetTimer());
			actionTimer_.SetLimitTime(attack_->GetInfo().maxTime + attackTimer_.GetLimitTimer());
			handObj_->SetPos(attack_->GetAttackCol()->at(0)->col_.center);
		}

		if (actionTimer_.GetIsEnd())
		{
			SetState(State::Idle);
			attack_.reset();
			attackTimer_.Reset();
			handAxisX_ = IdentityQuaternion();
			handObj_->SetIsVisible(false);
		}
	}
	else {
		attackTimer_.AddTime(1);

		Vector3 startPos = GetWorldTransform()->position_;
		startPos.y = GetWorldTransform()->scale_.y;
		Vector3 up = Vector3(0, 1, 0) * (GetWorldTransform()->scale_.y * 2.f);
		Vector3 endPos = startPos + up;

		Vector3 handPos;
		handPos = Easing::Quint::easeOut(startPos.z, endPos.z, attackTimer_.GetTimeRate());
		Vector3 handscale;
		handscale = Easing::Back::easeOut(0, handScale_, attackTimer_.GetTimeRate());
		float handRot = Easing::Quint::easeOut(0, Radian(handRotLimit_), attackTimer_.GetTimeRate());

		handObj_->WT_.SetQuaternion(VecToDir(EtoPVec_));

		handObj_->SetPos(handPos);
		handObj_->SetScale(handscale);
		handResultQ_ = displayObj_->WT_.quaternion_ * MakeAxisAngle(Vector3(0, 1, 0), handRot);
		handResultQ_ = handResultQ_ * handAxisX_;
		handObj_->WT_.quaternion_ = handResultQ_;

		if (attackTimer_.GetTimeRate() < 0.6f) {
			addVec_ = -EtoPVec_.normalize() * 0.05f;
		}
	}
}

void EnemyShadow::JumpAttack()
{
	stateName_ = "JumpAttack";

	attack_->Update();
	attack_->SetNowTime(actionTimer_.GetTimer());
	actionTimer_.SetLimitTime(attack_->GetInfo().maxTime);

	if (actionTimer_.GetIsEnd())
	{
		SetState(State::Idle);
		attack_.reset();
	}
}

void EnemyShadow::KnockBack()
{
	stateName_ = "KnockBack";

	if (isDown_ == false) {
		//プレイヤーの方に向く姿勢と後ろにのけぞる姿勢を掛ける
		auto resultQ = EToPQuaternion_ * knockQuaternion_;
		if (actionTimer_.GetTimeRate() <= 0.4f)
		{
			obj_->GetTransform()->SetQuaternion(obj_->GetTransform()->quaternion_.Slerp(resultQ, slerpSpeed_));
		}
		else
		{
			obj_->GetTransform()->SetQuaternion(obj_->GetTransform()->quaternion_.Slerp(EToPQuaternion_, 0.15f));
		}
	}

	//一定時間経てばノック状態からアイドル状態に戻る
	attack_.reset();
	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		isKnock_ = false;
		actionTimer_.Reset();

		obj_->SetScale(Vector3(1, 1, 1));
	}
}

void EnemyShadow::SortPriority()
{
	for (auto& p : priority_)
	{
		p.second = 0;
	}

	Vector3 EtoPVec = splayer_->GetWorldTransform()->position_ - obj_->WT_.position_;
	Vector3 compareShortVec = { 0,0,2.f };
	Vector3 compareLongVec = { 0,0,5.f };
	float length = EtoPVec.length();
	float compareShortlength = compareShortVec.length();
	float compareLonglength = compareLongVec.length();
	//近距離にいるとき
	if (length <= compareShortlength)
	{
		priority_.at(State::Attack) += 70;
		priority_.at(State::Wander) += 10;
		priority_.at(State::Idle) += 20;
	}
	//中距離にいるとき
	else if (length > compareShortlength &&
		length < compareLonglength)
	{
		priority_.at(State::Attack) += 30;
		priority_.at(State::Wander) += 10;
		priority_.at(State::Following) += 2;
		priority_.at(State::Idle) += 10;
	}
	//遠くにいるとき
	else
	{
		priority_.at(State::Wander) += 100;
		priority_.at(State::Following) += 70;
		priority_.at(State::Idle) += 5;
	}

	std::vector<std::pair<State, int32_t>> arr;
	std::vector<int32_t> probability;
	int32_t allPriolityValue = 0;
	for (const auto& p : priority_)
	{
		//プライオリティが0のものは除外する
		if (p.second == 0) continue;

		arr.emplace_back(p);
		//優先度を足していく
		allPriolityValue += p.second;
	}
	//降順に並び変える
	std::sort(arr.begin(), arr.end(),
		[](const auto& x, const auto& y) {return x.second > y.second; });

	uint32_t rand = RRandom::Rand(0, allPriolityValue);

	uint32_t prePriolityValue = 0;	//累計の優先度
	for (uint32_t i = 0; i < arr.size(); i++)
	{
		uint32_t nowPriolityValue = 0;	//現在の優先度

		if (i >= 1)
		{
			prePriolityValue += arr[i - 1].second;
		}
		//
		nowPriolityValue = prePriolityValue + arr[i].second;
		//ランダムで決めた値が、
		//前回の優先度よりも高い & 前回の優先度 + 今回の優先度よりも低い時
		if (prePriolityValue <= rand &&
			nowPriolityValue >= rand)
		{
			//ステートを代入
			StateUpdate(arr[i].first);
		}
	}
}

void EnemyShadow::WanderInit()
{
	if (isWanderInit_)
	{
		//削除する
		spline_.DeleteAllPoint();
		spline_.Reset();
		auto pos = obj_->WT_.position_;
		pos.y = 0.1f;
		//初期地点を挿入
		spline_.AddPosition(pos, PosState::Start);

		//移動するポイントを計算する
		for (int32_t i = 0; i < 2; i++)
		{
			size_t index = spline_.GetsplinePos().size() - 1;
			Vector3 splinePos = spline_.GetsplinePos().at(index);
			//スプラインのポイントからtargetPosへのベクトルを計算
			Vector3 targetPos = splayer_->GetWorldTransform()->position_;
			targetPos += {
				RRandom::RandF(-2.f, 2.f),
					0,
					RRandom::RandF(-2.f, 2.f)
			};
			Vector3 EtoPVec = targetPos - splinePos;
			EtoPVec = EtoPVec.normalize();

			EtoPVec = EtoPVec * Vector3(
				RRandom::RandF(3.f, 7.f),
				0,
				RRandom::RandF(3.f, 7.f));
			//乱数

			splinePos += EtoPVec;

			spline_.AddPosition(splinePos, PosState::Middle);
		}

		size_t index = spline_.GetsplinePos().size() - 1;
		Vector3 splinePos = spline_.GetsplinePos().at(index);
		//最終地点はプレイヤーの近くへ行く
		Vector3 endPos = {
			splinePos.x + RRandom::RandF(-3.f, 3.f),
			0,
			splinePos.z + RRandom::RandF(-3.f, 3.f),
		};
		//最終地点を代入
		spline_.AddPosition(endPos, PosState::End);
		spline_.SetIsStart(true);
		spline_.SetLimitTime(50);

		isWanderInit_ = false;
	}
}

void EnemyShadow::StateUpdate(State state)
{
	state_ = state;
	actionTimer_.Reset();
	if (state == State::Idle)
	{
		actionTimer_.SetLimitTime((float)RRandom::Rand(100, 200));
	}
	else if (state == State::Following)
	{
		actionTimer_.SetLimitTime(300);
	}
	else if (state == State::Wander)
	{

		actionTimer_.SetLimitTime(300);
		isWanderInit_ = true;
	}
	else if (state == State::HideMove)
	{

	}
	else if (state == State::Attack)
	{
		actionTimer_.SetLimitTime(70);
		attackTimer_.SetLimitTime(30);

		obj_->WT_.SetQuaternion(VecToDir(EtoPVec_));
		attack_.reset();
		attack_ = std::move(std::make_unique<AttackShadow>(this));
		attack_->SetLockOnActor(splayer_);
		attack_->Init();
	}
	else if (state == State::JumpAttack)
	{
		actionTimer_.SetLimitTime(100);
		attack_.reset();
		attack_ = std::move(std::make_unique<AttackJumpShadow>(this));
		attack_->SetLockOnActor(splayer_);
		attack_->Init();
	}
}

void EnemyShadow::KnockRotate()
{
	//プレイヤーの方に向く姿勢と回転する姿勢を掛ける
	auto resultQ = EToPQuaternion_ * MakeAxisAngle({ 1, 0, 0 }, Radian(objAngleX_));

	knockRotTimer_.AddTime(1);

	objAngleX_ = Easing::Quint::easeOut(0, knockEndRot_, knockRotTimer_.GetTimeRate());

	obj_->WT_.quaternion_ = resultQ;
}
