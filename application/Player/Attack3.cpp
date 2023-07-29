#include "Attack3.h"

Attack3::Attack3(IActor* selfActor) :
	IAttack(selfActor,1, 130, 10, 31)
{
}

void Attack3::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		frontDist_ = selfActor_->GetWorldTransform()->scale_.x;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		else {
			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(selfActor_->GetWorldTransform()->rotation_.y),
				0,
				cosf(selfActor_->GetWorldTransform()->rotation_.y),
			};
		}
		frontVec = frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
	}

	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 20;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 1.f,1.f,1.f };
	attackCol_.at(0)->knockVecY = 0.8f;

	attackVec_ = frontVec;

	spline_.SetLimitTime(attackInfo_.maxTime - 70);
}

void Attack3::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	Vector3 speed = attackVec_ * 0.1f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;


	selfActor_->AddVec(speed);


	Vector3 attackBasePos = selfActor_->GetWorldTransform()->position_ + Vector3(0, 1, 0);

	std::vector<Vector3>attackVec;
	Vector3 rightVec = CalculateFrontVec().cross(Vector3(0, 1, 0));
	rightVec = rightVec.normalize();
	Vector3 playerrightPos =
		attackBasePos + rightVec * 2.f;
	attackVec.push_back(playerrightPos);
	attackVec.push_back(playerrightPos);

	Vector3 playerFrontPos =
		attackBasePos + CalculateFrontVec().normalize() * 3.f;
	spline_.AddPosition(playerFrontPos, PosState::Middle);
	attackVec.push_back(playerFrontPos);


	Vector3 playerLeftPos =
		attackBasePos + -rightVec * 2.f;
	attackVec.push_back(playerLeftPos);
	attackVec.push_back(playerLeftPos);

	spline_.SetPositions(attackVec);

	spline_.Update();

	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
}

