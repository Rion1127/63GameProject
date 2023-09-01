#include "EnemyRedNocturne.h"
#include "RRandom.h"
#include "Player.h"
#include "Easing.h"
#include "AttackRedNocturne.h"
#include "ParticleFire.h"
#include "ParticleManager.h"

EnemyRedNocturne::EnemyRedNocturne(Vector3 pos, Vector3 rot) :
	IEnemy(EnemyType::Air, false, 80)
{
	name_ = "RedNocturne";
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	obj_->GetTransform()->SetRotation(rot);
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

	InitFireParticle();
}

EnemyRedNocturne::~EnemyRedNocturne()
{
	fireEmitter_->isActive = false;
	fireCircleEmitter_->isActive = false;
	fireCircleEmitter_->parentPos = nullptr;
}

void EnemyRedNocturne::SetIsNock(bool flag)
{
	isKnock_ = flag;
	actionTimer_.SetLimitTime(40);
	actionTimer_.Reset();
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
		actionTimer_.AddTime(1);
		(this->*Action[(int32_t)state_])();
	}

	if (actionTimer_.GetIsEnd())
	{
		SortPriority();
	}
#ifdef _DEBUG

	ImGui::Begin("RedNocturne");

	std::string text = "State : ";
	if (state_ == State::Idle)		text += "Idle";
	if (state_ == State::KnockBack)		text += "KnockBack";
	if (state_ == State::FireAttack)			text += "FireAttack";
	if (state_ == State::Wander)		text += "Wander";
	if (state_ == State::Wander_FireAttack)		text += "Wander_FireAttack";
	
	ImGui::Text(text.c_str());

	ImGui::End();
#endif // _DEBUG
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
	if (fireEmitter_->isActive) {
		//座標を更新
		Vector3 pos = obj_->GetPos();
		pos.y += obj_->GetScale().y * 2.f;
		fireEmitter_->pos = pos;
	}
	fireCircleEmitter_->pos = obj_->GetPos();
	fireCircleEmitter_->pos.y += obj_->GetScale().y;
	if (shotTimer_.GetTimeRate() > 0.7f) {
		fireEmitter_->isActive = false;
	}

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
	//一定時間経てばノック状態からアイドル状態に戻る
	attack_.reset();
	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		isKnock_ = false;
		actionTimer_.Reset();
	}
	fireEmitter_->isActive = false;
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
	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	fireEmitter_->particle = std::make_unique<ParticleFire>();
	fireEmitter_->addVec = { 0.25f,0.3f, 0.25f, };
	fireEmitter_->addNum = 4;
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->time = 30;
	fireEmitter_->pos = pos;
	fireEmitter_->scale = 0.3f;
	
	fireCirclePos_ = obj_->GetPos();
	fireCirclePos_.y += obj_->GetScale().y;
	fireCircleEmitter_ = std::make_shared<OneceEmitter>();
	fireCircleEmitter_->particle = std::make_unique<ParticleFireCircle>();
	fireCircleEmitter_->addVec = { 0.0f,0.0f, 0.0f, };
	fireCircleEmitter_->addNum = 2;
	fireCircleEmitter_->time = 180;
	fireCircleEmitter_->parentPos = &fireCirclePos_;
	fireCircleEmitter_->pos = fireCirclePos_;
	fireCircleEmitter_->scale = 1.0f;
}

void EnemyRedNocturne::AddFireParticle()
{
	ParticleManager::GetInstance()->AddParticle("fireCharge", fireEmitter_);
	ParticleManager::GetInstance()->AddParticle("fireCircle", fireCircleEmitter_);
}
