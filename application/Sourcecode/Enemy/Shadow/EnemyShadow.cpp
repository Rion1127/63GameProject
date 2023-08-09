#include "EnemyShadow.h"
#include "mInput.h"
#include "Player.h"
#include <imgui.h>

#include "RRandom.h"

#include "AttackShadow.h"
#include "AttackJumpShadow.h"

EnemyShadow::EnemyShadow(Vector3 pos) :
	IEnemy(EnemyType::Ground, true, 100)
{
	name_ = "Shadow";
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("shadow", true));
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(400);

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
	actionTimer_.SetLimitTime(100);
	actionTimer_.Reset();
}

void EnemyShadow::SetState(State state)
{
	state_ = state;
	actionTimer_.Reset();
	int32_t randRange = RRandom::Rand(-randRange_, randRange_);
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
	obj_->Draw();
	if (attack_ != nullptr)
	{
		attack_->DrawCol();
	}
}

void EnemyShadow::MoveUpdate()
{
	if (health_ < 100)
	{
		//health_ += 5;
	}
	//ステータスごとの動きを追加
	void (EnemyShadow:: * Action[]) () =
	{
		&EnemyShadow::Idle,
		&EnemyShadow::Following,
		&EnemyShadow::Wander,
		&EnemyShadow::HideMove,
		&EnemyShadow::Attack,
		&EnemyShadow::JumpAttack,
		&EnemyShadow::KnockBack
	};

	if (isKnock_)
	{
		state_ = State::KnockBack;
	}

	UpdateEtoPVec();

	actionTimer_.AddTime(1);
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

void EnemyShadow::Idle()
{
	stateName_ = "Idle";


}

void EnemyShadow::Following()
{
	stateName_ = "Following";

	float length = EtoPVec_.length();
	addVec_ = EtoPVec_.normalize() * moveSpeed;

	Vector2 dir = {
			EtoPVec_.x,
			EtoPVec_.z
	};
	//進行方向に回転
	float rotY = Vec2Angle(dir);
	obj_->WT_.rotation_.y = Radian(rotY);
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
		if (obj_->WT_.scale_.y > 0.1f)
		{
			obj_->WT_.scale_.y -= 0.1f;
		}
		else
		{
			//スプライン曲線更新
			spline_.Update();
			obj_->WT_.SetPosition(spline_.GetNowPoint());
		}
		Vector2 dir = {
			spline_.GetHeadingVec().x,
			spline_.GetHeadingVec().z
		};
		//進行方向に回転
		float rotY = Vec2Angle(dir);
		obj_->WT_.rotation_.y = Radian(rotY);
	}
	//移動が終わったら別のパターンへ
	else
	{
		if (obj_->WT_.scale_.y < 1.0f)
		{
			obj_->WT_.scale_.y += 0.1f;
		}
		else
		{
			state_ = State::Idle;
			actionTimer_.Reset();
			actionTimer_.SetLimitTime(100);
			col_.isActive = true;
		}
	}
}

void EnemyShadow::HideMove()
{
	stateName_ = "HideMove";

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
	//一定時間経てばノック状態からアイドル状態に戻る
	attack_.reset();
	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		isKnock_ = false;
		actionTimer_.Reset();
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
		priority_.at(State::JumpAttack) += 20;
		priority_.at(State::Wander) += 10;
		priority_.at(State::Idle) += 20;
	}
	//中距離にいるとき
	else if (length > compareShortlength &&
		length < compareLonglength)
	{
		priority_.at(State::JumpAttack) += 70;
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
		//優先度の
		allPriolityValue += p.second;
	}
	//降順に並び変える
	std::sort(arr.begin(), arr.end(),
		[](const auto& x, const auto& y) {return x.second > y.second; });

	uint16_t rand = RRandom::Rand(0, allPriolityValue);

	int32_t prePriolityValue = 0;	//累計の優先度
	for (uint32_t i = 0; i < arr.size(); i++)
	{
		int32_t nowPriolityValue = 0;	//現在の優先度

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
		spline_.DleteAllPoint();
		spline_.Reset();
		//初期地点を挿入
		spline_.AddPosition(obj_->WT_.position_, PosState::Start);

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
		actionTimer_.SetLimitTime(RRandom::Rand(100,300));
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
