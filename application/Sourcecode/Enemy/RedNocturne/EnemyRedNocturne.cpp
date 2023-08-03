#include "EnemyRedNocturne.h"
#include "RRandom.h"
#include "Player.h"

EnemyRedNocturne::EnemyRedNocturne(Vector3 pos) :
	IEnemy(EnemyType::Air, false, 80)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(400);

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

	actionTimer_.AddTime(1);
	//実行
	(this->*Action[(int32_t)state_])();

	if (actionTimer_.GetIsEnd())
	{
		stateInit = true;
		//SortPriority();
	}
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

		float wonderDist = 8.f;

		Vector3 offsetPos = {
			RRandom::RandF(-1.f,1.f),
			0,
			RRandom::RandF(-1.f,1.f),
		};
		offsetPos *= wonderDist;
		Vector3 goalPos =
			IEnemy::splayer_->GetWorldTransform()->position_ + offsetPos;
	}


}

void EnemyRedNocturne::Attack()
{
}

void EnemyRedNocturne::KnockBack()
{
}
