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
	displayObj_->WT_.SetRotType(RotType::Quaternion);
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	obj_->GetTransform()->SetRotation(rot);
	displayObj_->GetTransform()->SetPosition(pos);
	displayObj_->GetTransform()->SetRotation(rot);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(400);

	sinkTimer_.SetLimitTime(40);

	slimeTimer_.SetLimitTime(7);

	followLength = 5.f;
	moveSpeed = 0.1f;
	randRange_ = 30;
	isWanderInit_ = false;

	//プライオリティに行動パターンを登録
	priority_.insert(std::make_pair(State::Idle, 0));
	priority_.insert(std::make_pair(State::Following, 0));
	priority_.insert(std::make_pair(State::Wander, 0));
	priority_.insert(std::make_pair(State::HideMove, 0));
	priority_.insert(std::make_pair(State::Attack, 0));
	priority_.insert(std::make_pair(State::JumpAttack, 0));
	priority_.insert(std::make_pair(State::KnockBack, 0));
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
		actionTimer_.SetLimitTime(100 + randRange);
	}
	else if (State::Following == state_)
	{
		actionTimer_.SetLimitTime(150 + randRange);
	}
}

void EnemyShadow::Draw()
{
	displayObj_->Draw();
	if (attack_ != nullptr)
	{
		attack_->DrawCol();
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
	//プレイヤーへのベクトル更新
	UpdateEtoPVec();

	actionTimer_.AddTime(1 * GameSpeed::GetEnemySpeed());
	//実行
	(this->*Action[(int32_t)state_])();

	if (actionTimer_.GetIsEnd())
	{
		SortPriority();
	}
#ifdef _DEBUG
	ImGui::Begin("Enemy");

	ImGui::Text(stateName_.c_str());

	ImGui::End();
#endif // _DEBUG
}

void EnemyShadow::DrawSprite()
{

}

void EnemyShadow::DamageUpdate()
{
	EToPQuaternion_ = VecToDir(EtoPVec_);
	knockQuaternion_ = {
		RRandom::RandF(-0.7f,-0.3f),
		RRandom::RandF(-0.4f, 0.4f),
		0,
		1.0f
	};
	obj_->WT_.SetQuaternion(EToPQuaternion_);

	obj_->SetScale(Vector3(1, 1, 1));

	state_ = State::KnockBack;
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
			actionTimer_.SetLimitTime(160);

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
			obj_->WT_.scale_.y = Easing::Sine::easeIn(t, 1.0f, -0.9f, 1.0f);
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
		obj_->WT_.SetQuaternion(VecToDir(spline_.GetHeadingVec()));

		
	}
	//移動が終わったら別のパターンへ
	else
	{
		if (sinkTimer_.GetIsEnd() == false)
		{
			float t = sinkTimer_.GetTimeRate();
			obj_->WT_.scale_.y = Easing::Sine::easeIn(t, 0.1f, 0.9f, 1.0f);
			sinkTimer_.AddTime(1 * GameSpeed::GetEnemySpeed());
		}
		else
		{
			sinkTimer_.Reset();
			state_ = State::Idle;
			actionTimer_.Reset();
			actionTimer_.SetLimitTime(100);
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
	if (actionTimer_.GetTimeRate() <= 0.2f) {
		scaleZ = Easing::Bounce::easeOut(actionTimer_.GetTimeRate(), 1.f, -0.6f, 0.2f);
	}
	else {
		float maxRate = 0.6f;
		float rate = actionTimer_.GetTimeRate() - 0.2f;
		rate = Min(maxRate, rate);
		scaleZ = Easing::Bounce::easeOut(rate, 0.4f, 0.6f, maxRate);
	}
	col_.radius = scaleZ;

	obj_->SetScale(Vector3(1,1, scaleZ));
	if (actionTimer_.GetTimeRate() >= 0.6f) {
		obj_->WT_.quaternion_ = obj_->WT_.quaternion_.Slerp(EToPQuaternion_, 0.2f);
	}
	else {
		auto resultQ = EToPQuaternion_ * Quaternion(-1,0,0,1);
		obj_->GetTransform()->SetQuaternion(obj_->GetTransform()->quaternion_.Slerp(resultQ, 0.2f));
	}
	if (actionTimer_.GetIsEnd())
	{
		slimeTimer_.Reset();
		obj_->SetScale(Vector3(1, 1, 1));
		col_.radius = 1;
		state_ = State::Idle;
		actionTimer_.Reset();
		actionTimer_.SetLimitTime(70);
		attack_.reset();
	}
}

void EnemyShadow::Attack()
{
	stateName_ = "Attack";
	attack_->Update();
	attack_->SetNowTime(actionTimer_.GetTimer());
	actionTimer_.SetLimitTime(attack_->GetInfo().maxTime);

	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		actionTimer_.Reset();
		actionTimer_.SetLimitTime(120);
		attack_.reset();
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
		state_ = State::Idle;
		actionTimer_.Reset();
		actionTimer_.SetLimitTime(120);
		attack_.reset();
	}
}

void EnemyShadow::KnockBack()
{
	stateName_ = "KnockBack";

	if (isDown_ == false) {
		auto resultQ = EToPQuaternion_ * knockQuaternion_;
		if (actionTimer_.GetTimeRate() <= 0.4f)
		{
			obj_->GetTransform()->SetQuaternion(obj_->GetTransform()->quaternion_.Slerp(resultQ, 0.2f));
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
		//priority_.at(State::JumpAttack) += 20;
		priority_.at(State::Wander) += 10;
		priority_.at(State::Idle) += 20;
	}
	//中距離にいるとき
	else if (length > compareShortlength &&
		length < compareLonglength)
	{
		//priority_.at(State::JumpAttack) += 70;
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
		actionTimer_.SetLimitTime((float)RRandom::Rand(100, 300));
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
