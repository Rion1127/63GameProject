#include "EnemyRedNocturne.h"
#include "RRandom.h"
#include "Player.h"
#include "Easing.h"
#include "AttackRedNocturne.h"

EnemyRedNocturne::EnemyRedNocturne(Vector3 pos) :
	IEnemy(EnemyType::Air, false, 80)
{
	name_ = "RedNocturne";
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(60);
	shotTimer_.SetLimitTime(180);

	//プライオリティに行動パターンを登録
	priority_.insert(std::make_pair(State::Idle, 0));
	priority_.insert(std::make_pair(State::Wander, 0));
	priority_.insert(std::make_pair(State::FireAttack, 0));
	priority_.insert(std::make_pair(State::Wander_FireAttack, 0));
	priority_.insert(std::make_pair(State::KnockBack, 0));
	priority_.insert(std::make_pair(State::None, 0));

	stateInit = true;
}

void EnemyRedNocturne::SetIsNock(bool flag)
{
}

void EnemyRedNocturne::Draw()
{
	obj_->Draw();

	if (attack_ != nullptr) {
		attack_->DrawCol();
	}
}

void EnemyRedNocturne::MoveUpdate()
{
	//ステータスごとの動きを追加
	void (EnemyRedNocturne:: * Action[]) () =
	{
		&EnemyRedNocturne::Idle,
		&EnemyRedNocturne::Wander,
		&EnemyRedNocturne::FireAttack,
		&EnemyRedNocturne::Wander_FireAttack,
		&EnemyRedNocturne::KnockBack
	};

	if (isKnock_)
	{
		state_ = State::KnockBack;
	}

	/*UpdateVector();*/
	
	
	//実行
	if (isCanMove_)
	{
		actionTimer_.AddTime(1);
		(this->*Action[(int32_t)state_])();
	}

	if (actionTimer_.GetIsEnd())
	{
		SortPriority();
	}
}

void EnemyRedNocturne::BulletShot(std::list<std::unique_ptr<IBullet>>* bullets)
{
	bullets->emplace_back(std::make_unique<AttackRedNocturne>(this));

	auto& bullet = bullets->back();
	bullet->SetLockOnActor(splayer_);
	bullet->Init();
	
	isBulletShot_ = false;
}

void EnemyRedNocturne::Idle()
{
}

void EnemyRedNocturne::Wander()
{
	//初期化
	if (stateInit)
	{
		stateInit = false;

		float wonderDist = RRandom::RandF(-8.f, 8.f);

		Vector3 offsetPos = {
			RRandom::RandF(-1.f,1.f),
			0,
			RRandom::RandF(-1.f,1.f),
		};
		offsetPos *= wonderDist;
		wanderStartPos_ = obj_->WT_.position_;
		wanderEndPos_ = (IEnemy::splayer_->GetWorldTransform()->position_ - wanderStartPos_) + offsetPos;

		pos_ = wanderStartPos_;
	}

	addVec_ = {
		pos_.x - obj_->WT_.position_.x ,
		0,
		pos_.z-obj_->WT_.position_.z ,
	};
	float rate = actionTimer_.GetTimeRate();
	pos_ = {
		Easing::Cubic::easeInOut(rate, wanderStartPos_.x, wanderEndPos_.x, 1.0f),
		0.f,
		Easing::Cubic::easeInOut(rate, wanderStartPos_.z, wanderEndPos_.z, 1.0f),
	};

}

void EnemyRedNocturne::FireAttack()
{
	shotTimer_.AddTime(1);
	if (shotTimer_.GetIsEnd()) {
		isBulletShot_ = true;
		shotTimer_.Reset();
	}
}

void EnemyRedNocturne::Wander_FireAttack()
{
	Wander();
	FireAttack();
}

void EnemyRedNocturne::KnockBack()
{
}

void EnemyRedNocturne::SortPriority()
{
	for (auto& p : priority_)
	{
		p.second = 0;
	}

	Vector3 EtoPVec = splayer_->GetWorldTransform()->position_ - obj_->WT_.position_;
	Vector3 compareShortVec = { 0,0,7.f };
	Vector3 compareLongVec = { 0,0,13.f };
	float length = EtoPVec.length();
	float compareShortlength = compareShortVec.length();
	float compareLonglength = compareLongVec.length();
	//近距離にいるとき
	if (length <= compareShortlength)
	{
		priority_.at(State::FireAttack) += 10;
		priority_.at(State::Wander_FireAttack) += 10;
		priority_.at(State::Wander) += 100;
		priority_.at(State::Idle) += 10;
	}
	//中距離にいるとき
	else if (length > compareShortlength &&
		length < compareLonglength)
	{
		priority_.at(State::FireAttack) += 30;
		priority_.at(State::Wander_FireAttack) += 30;
		priority_.at(State::Wander) += 100;
		priority_.at(State::Idle) += 5;
	}
	//遠くにいるとき
	else
	{
		priority_.at(State::FireAttack) += 80;
		priority_.at(State::Wander_FireAttack) += 60;
		priority_.at(State::Wander) += 60;
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

void EnemyRedNocturne::StateUpdate(State state)
{
	state_ = state;
	actionTimer_.Reset();
	stateInit = true;
	if (state == State::Idle)
	{
		actionTimer_.SetLimitTime(RRandom::Rand(100, 300));
	}
	else if (state == State::Wander)
	{
		actionTimer_.SetLimitTime(60);
	}
	else if (state == State::FireAttack)
	{
		actionTimer_.SetLimitTime(180);
	}
	else if (state == State::Wander_FireAttack)
	{
		actionTimer_.SetLimitTime(180);
	}
}
