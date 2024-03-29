#include "EnemyRedNocturne.h"
#include "RRandom.h"
#include "Player.h"
#include "Easing.h"
#include "AttackRedNocturne.h"
#include "ParticleFire.h"
#include "ParticleManager.h"
#include "GameSpeed.h"
#include "mSound.h"
#include "SoundVolume.h"

/**
 * @file EnemyRedNocturne.cpp
 * @brief 空中の敵（レッドノクターン）のクラス
 */

EnemyRedNocturne::EnemyRedNocturne(const Vector3& pos, const Vector3& rot) :
	IEnemy(EnemyType::Air, false, 80)
{
	name_ = "RedNocturne";
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true, false));
	obj_->SetShadowOffsetPos(Vector3(0, -1, 0));
	obj_->SetShadowAtten(Vector3(0.1f, 0.22f, 0));
	obj_->SetShadowFactorAngle(Vector2(0.1f, 0.5f));

	displayObj_ = std::move(std::make_unique<Object3d>());
	displayObj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	displayObj_->SetShadowOffsetPos(Vector3(0, -2, 0));
	displayObj_->SetShadowAtten(Vector3(0.1f, 0.22f, 0));
	displayObj_->SetShadowFactorAngle(Vector2(0.1f, 0.5f));

	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	obj_->GetTransform()->SetRotation(rot);
	displayObj_->GetTransform()->SetPosition(pos);
	displayObj_->GetTransform()->SetRotation(rot);

	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(60);
	shotTimer_.SetLimitTime(240);

	//プライオリティに行動パターンを登録
	priority_.insert(std::make_pair(State::Idle, 0));
	priority_.insert(std::make_pair(State::Wander, 0));
	priority_.insert(std::make_pair(State::FireAttack, 0));
	priority_.insert(std::make_pair(State::Wander_FireAttack, 0));
	priority_.insert(std::make_pair(State::KnockBack, 0));
	priority_.insert(std::make_pair(State::None, 0));

	stateInit = true;

	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	fireCircleEmitter_ = std::make_shared<OneceEmitter>();
	InitFireParticle();


	floatTimer_.SetLimitTime(40);
}

EnemyRedNocturne::~EnemyRedNocturne()
{
	fireEmitter_->isActive = false;
	fireCircleEmitter_->isActive = false;
	fireCircleEmitter_->parentPos = nullptr;
	fireEmitter_.reset();
}

void EnemyRedNocturne::SetIsNock(bool flag)
{
	isKnock_ = flag;
	actionTimer_.SetLimitTime(40);
	actionTimer_.Reset();
}

void EnemyRedNocturne::SetIsDown(bool flag)
{
	isDown_ = flag;
	actionTimer_.SetLimitTime(40);
	actionTimer_.Reset();
}

void EnemyRedNocturne::Draw()
{
	displayObj_->Draw();

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

	if (isKnock_ || isDown_)
	{
		state_ = State::KnockBack;
	}

	/*UpdateVector();*/
	Vector3 lockOnPos = splayer_->GetWorldTransform()->position_;
	Vector3 frontVec;
	//ロックオンしている敵へのベクトルをとる
	frontVec = {
		lockOnPos.x - GetWorldTransform()->position_.x,
		0,
		lockOnPos.z - GetWorldTransform()->position_.z,
	};
	Vector2 frontVec2 = {
		frontVec.x,
		frontVec.z
	};
	float rotY = Radian(Vec2Angle(frontVec2));
	GetWorldTransform()->rotation_ = { 0,rotY ,0 };

	//実行
	if (isCanMove_)
	{
		actionTimer_.AddTime(1 * GameSpeed::GetEnemySpeed());
		(this->*Action[(int32_t)state_])();
	}

	if (actionTimer_.GetIsEnd())
	{
		SortPriority();

		floatTimer_.Reset();
		floatTimer_.SetLimitTime(RRandom::Rand(40,70));
	}
}

void EnemyRedNocturne::BulletShot(std::list<std::unique_ptr<IBullet>>* bullets)
{
	bullets->emplace_back(std::make_unique<AttackRedNocturne>(this));

	auto& bullet = bullets->back();
	bullet->SetLockOnActor(splayer_);
	bullet->Init();

	isBulletShot_ = false;
	SoundManager::Play("FireSE", false, SoundVolume::GetValumeSE());
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

		float max = 1.f;
		float min = -1.f;

		if (wanderStartPos_.y > 5.f)
		{
			max = 0.1f;
			min = -1.0f;
		}
		else if (wanderStartPos_.y < 3.f)
		{
			max = 1.0f;
			min = -0.1f;
		}
		wanderEndPos_.y = RRandom::RandF(min, max);
		pos_ = wanderStartPos_;
	}

	addVec_ = {
		pos_.x - obj_->WT_.position_.x ,
		pos_.y - obj_->WT_.position_.y ,
		pos_.z - obj_->WT_.position_.z ,
	};
	float rate = actionTimer_.GetTimeRate();
	pos_ = {
		Easing::Cubic::easeInOut(rate, wanderStartPos_.x, wanderEndPos_.x, 1.0f),
		Easing::Cubic::easeInOut(rate, wanderStartPos_.y, wanderEndPos_.y, 1.0f),
		Easing::Cubic::easeInOut(rate, wanderStartPos_.z, wanderEndPos_.z, 1.0f),
	};

}

void EnemyRedNocturne::FireAttack()
{
	if (fireEmitter_->isActive) {
		//座標を更新
		Vector3 pos = obj_->GetPos();
		pos.y += obj_->GetScale().y * 2.f;
		fireEmitter_->pos = pos;
	}
	fireCircleEmitter_->pos = obj_->GetPos();
	fireCircleEmitter_->pos.y += obj_->GetScale().y;
	if (shotTimer_.GetTimeRate() > 0.8f) {
		fireEmitter_->isActive = false;
	}

	shotTimer_.AddTime(1 * GameSpeed::GetEnemySpeed());
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
	//一定時間経てばノック状態からアイドル状態に戻る
	attack_.reset();

	Vector3 rot = obj_->GetTransform()->rotation_;

	rot.x = Easing::Back::easeOut(0,Radian(-360), actionTimer_.GetTimeRate());

	obj_->GetTransform()->SetRotation(rot);

	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		isKnock_ = false;
		isDown_ = false;
		actionTimer_.Reset();
		fireEmitter_->isActive = false;
	}
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

void EnemyRedNocturne::StateUpdate(State state)
{
	state_ = state;
	actionTimer_.Reset();
	stateInit = true;
	if (state == State::Idle)
	{
		actionTimer_.SetLimitTime((float)RRandom::Rand(100, 300));
	}
	else if (state == State::Wander)
	{
		actionTimer_.SetLimitTime(60);
	}
	else if (state == State::FireAttack)
	{
		actionTimer_.SetLimitTime(240);

		InitFireParticle();
		AddFireParticle();
	}
	else if (state == State::Wander_FireAttack)
	{
		actionTimer_.SetLimitTime(240);

		InitFireParticle();
		AddFireParticle();
	}
}

void EnemyRedNocturne::InitFireParticle()
{
	Timer timer;
	timer.SetLimitTime(1);
	timer.SetTime(timer.GetLimitTimer());
	Vector3 pos = obj_->GetPos();
	pos.y += obj_->GetScale().y * 2.f;
	
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->pos = pos;
	fireEmitter_->popCoolTime_.Reset();
	fireEmitter_->isActive = true;
	fireCirclePos_ = obj_->GetPos();
	fireCirclePos_.y += obj_->GetScale().y;
	fireCircleEmitter_->parentPos = &fireCirclePos_;
	fireCircleEmitter_->pos = fireCirclePos_;
}

void EnemyRedNocturne::AddFireParticle()
{
	ParticleManager::GetInstance()->AddParticle("fireCharge", fireEmitter_);
	ParticleManager::GetInstance()->AddParticle("fireCircle", fireCircleEmitter_);
}
