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

	//プライオリティに行動パターンを登録
	priority_.insert(std::make_pair(State::Idle, 0));
	priority_.insert(std::make_pair(State::Wander, 0));
	priority_.insert(std::make_pair(State::Attack, 0));
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
		&EnemyRedNocturne::Attack,
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
		stateInit = true;
		state_ = State::Attack;
		actionTimer_.Reset();
		//SortPriority();

		isBulletShot_ = true;
	}
}

void EnemyRedNocturne::BulletShot(std::list<std::unique_ptr<IAttack>>* bullets)
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

void EnemyRedNocturne::Attack()
{
	//attack_->Update();
}

void EnemyRedNocturne::KnockBack()
{
}
