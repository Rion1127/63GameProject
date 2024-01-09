#include "BaseAttack.h"
#include "GameSpeed.h"
#include "RRandom.h"
#include "mSound.h"
#include "SoundVolume.h"
#include "ParticleManager.h"

/**
 * @file BaseAttack.cpp
 * @brief プレイヤーの基本攻撃の動作をするクラス
 */

BaseAttack::BaseAttack(const AttackData& input, IActor* selfActor, IActor* lockOnActor) :
	attackdata_(input),
	selfActor_(selfActor),
	lockOnActor_(lockOnActor)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("sphere", false, false));
	spline_.SetParent(selfActor_->GetWorldTransform());
	colSpline_.SetParent(selfActor_->GetWorldTransform());
	index_ = 0;

	resultQuaternion_ = selfActor_->GetWorldTransform()->quaternion_;
	//プレイヤーの姿勢を敵の方向に向ける
	CalculateRotToLockOnActor();
	//こうげきのステータスをセットする
	SetNextAttack();

	//攻撃に掛かる時間を計算する
	float attackAllTime = 0;
	for (auto& info : attackdata_.attackinfo) {
		attackAllTime += info.attackFrame + info.gapFrame;
	}
	attackAllTime_.SetLimitTime(attackAllTime);
	//剣の座標を代入する
	spline_.Update();
	swordPos_ = spline_.GetNowPoint();

	//剣の座標をに当たり判定代入
	col_.center = swordPos_;
	if (attackdata_.attackinfo[index_].colType_ == ColType::Normal) {
		col_.radius = 1.0f;
	}
	else {
		col_.radius = attackdata_.attackinfo[index_].colInfo.firstRadius;
	}
	//ダメージのクールタイムを計算する
	//（攻撃の全てのフレーム数 - 現在の攻撃のフレーム数）
	float& attackTime = attackdata_.attackinfo[index_].attackFrame;
	damageCoolTime_ = (attackTime - spline_.GetTimer().GetTimer());
	//次のこうげきまでの時間を計算する
	nextAttackFrame_ += attackdata_.attackinfo[index_].attackAllFrame;
	//姿勢の線形補間の時間を計算する
	float time = (attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame - attackAllTime_.GetTimer());
	slerpSpeed_ = 1.f / time;
	for (auto& info : attackdata_.attackinfo)
	{
		for (auto& q : info.quaternion)
		{
			q.q = selfActor_->GetAxisY() * q.q;
		}
	}
	quaternionTimer_ = 0;
	//ダメージを代入
	if (attackdata_.attackinfo[index_].colType_ == ColType::Normal)
	{
		damage_ = attackdata_.attackinfo[index_].damage;
	}
	else
	{
		damage_ = attackdata_.attackinfo[index_].colInfo.damage;
	}
}

BaseAttack::~BaseAttack()
{
	if (emitter_ != nullptr) {
		emitter_->isActive = false;
	}
}

void BaseAttack::SetNextAttack()
{
	auto& attackInfo = attackdata_.attackinfo[index_];
	//剣の軌道初期化
	NormalSplinePosInit();
	//当たり判定が分離していたら別途計算する
	if (attackInfo.colType_ == ColType::Separate) {
		SeparateSplinePosInit();
		col_.radius = attackInfo.colInfo.firstRadius;
	}

	auto& addVec = attackInfo.playerMoveVec;

	addVec = RotateVector(addVec, selfActor_->GetAxisY());

	selfActor_->SetGravity(attackInfo.gravity);
	quaternionIndex_ = 0;
	oneSwingTimer_ = attackAllTime_.GetTimer();
}

void BaseAttack::Update()
{
	spline_.Update(GameSpeed::GetPlayerSpeed());
	attackAllTime_.AddTime(GameSpeed::GetPlayerSpeed());
	//攻撃が終了したら
	if (attackAllTime_.GetTimer() > nextAttackFrame_) {
		//残りの攻撃がある場合は続ける
		if (index_ < attackdata_.attackinfo.size() - 1) {
			index_++;
			nextAttackFrame_ += attackdata_.attackinfo[index_].attackAllFrame;
			SetNextAttack();

			float picth = RRandom::RandF(0.7f, 1.5f);
			SoundManager::Play("SwingSE", false, SoundVolume::GetValumeSE(), picth);
		}
	}
	//攻撃中フラグ
	if (attackAllTime_.GetIsEnd()) {
		isAttaking_ = false;
	}
	else {
		isAttaking_ = true;
	}
	//ダメージのクールタイムを計算する
	//（攻撃の全てのフレーム数 - 現在の攻撃のフレーム数）
	DamageCoolTimerUpdate();
	//プレイヤーの移動処理
	PlayerMove();
	//プレイヤーの姿勢更新
	QuaternionUpdate();
	swordPos_ = spline_.GetNowPoint();
	//当たり判定更新
	ColUpdate();
	//カメラシェイク・エフェクトを発生させる
	EffectPopUpdate();

	Vector3 scale = Vector3(col_.radius, col_.radius, col_.radius);
	obj_->SetScale(scale);
	obj_->Update();
}

void BaseAttack::Draw()
{
	if (col_.isActive)
	{
		PipelineManager::PreDraw("Object3DWireFrame", LINESTRIP);
		obj_->Draw();
		PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	}
}

Vector3 BaseAttack::CalculateFrontVec()
{
	Vector3 frontVec = { 0,0,0 };
	if (lockOnActor_ != nullptr)
	{
		Vector3& lockOnPos = lockOnActor_->GetWorldTransform()->position_;
		//ロックオンしている敵へのベクトルをとる
		frontVec = {
			lockOnPos.x - selfActor_->GetWorldTransform()->position_.x,
			0,
			lockOnPos.z - selfActor_->GetWorldTransform()->position_.z,
		};
	}
	else
	{
		frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetAxisY());
	}
	return frontVec.normalize();
}

void BaseAttack::CalculateRotToLockOnActor()
{
	Vector2 vec = { CalculateFrontVec().x,CalculateFrontVec().z };
	float rot = Vec2Angle(vec);
	Vector3 vecY = { 0, 1, 0 };
	auto axisY = MakeAxisAngle(vecY, Radian(rot));
	selfActor_->SetAxisY(axisY);
	Vector3 frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetAxisY());
	frontVec.y = 0;
	Vector2 angleVec2 = {
		frontVec.x,
		frontVec.z
	};
	selfActor_->SetObjAngleY(Vec2Angle(angleVec2));
	selfActor_->GetWorldTransform()->quaternion_ = axisY;
}

void BaseAttack::NormalSplinePosInit()
{
	spline_.AllClear();
	spline_.SetIsStart(true);
	spline_.SetTimerType_(attackdata_.attackinfo[index_].timerType);
	spline_.SetEasingType_(attackdata_.attackinfo[index_].easingType);
	spline_.SetEasingTypeInOut_(attackdata_.attackinfo[index_].inOutType);
	spline_.SetMaxTime(attackdata_.attackinfo[index_].attackFrame);
	auto& attackpoint = attackdata_.attackinfo[index_].splinePos;
	for (int32_t i = 0; i < attackpoint.size(); i++)
	{
		Vector3 splinePos = attackpoint[i];
		splinePos.y += selfActor_->GetWorldTransform()->scale_.y;
		if (i == 0)
		{
			spline_.AddPosition(splinePos, PosState::Start);
		}
		else if (i == attackpoint.size() - 1)
		{
			spline_.AddPosition(splinePos, PosState::End);
		}
		else
		{
			spline_.AddPosition(splinePos, PosState::Middle);
		}
	}
}

void BaseAttack::SeparateSplinePosInit()
{
	auto& colInfo = attackdata_.attackinfo[index_].colInfo;
	colSpline_.AllClear();
	colSpline_.SetIsStart(true);
	colSpline_.SetTimerType_(colInfo.timerType);
	colSpline_.SetEasingType_(colInfo.easingType);
	colSpline_.SetEasingTypeInOut_(colInfo.inOutType);
	colSpline_.SetMaxTime(colInfo.attackFrame);
	auto& colAttackpoint = colInfo.splinePos;
	for (int32_t i = 0; i < colAttackpoint.size(); i++)
	{
		Vector3 splinePos = colAttackpoint[i];
		splinePos.y += selfActor_->GetWorldTransform()->scale_.y;
		if (i == 0)
		{
			colSpline_.AddPosition(splinePos, PosState::Start);
		}
		else if (i == colAttackpoint.size() - 1)
		{
			colSpline_.AddPosition(splinePos, PosState::End);
		}
		else
		{
			colSpline_.AddPosition(splinePos, PosState::Middle);
		}
	}
}

void BaseAttack::PlayerMove()
{
	selfActor_->AddaddVec(attackdata_.attackinfo[index_].playerMoveVec);
	if (attackdata_.attackinfo[index_].playerMoveVec.length() != 0) {
		MoveTo(Vector3(0, 0, 0), attackdata_.attackinfo[index_].deceleration, attackdata_.attackinfo[index_].playerMoveVec);
	}
}

void BaseAttack::ColUpdate()
{
	auto& info = attackdata_.attackinfo[index_];
	//剣と当たり判定が同一の場合
	if (info.colType_ == ColType::Normal)
	{
		//剣の座標をに当たり判定代入
		colPos_ = swordPos_;
		//当たり判定は剣を振っている間だけ
		if (damageCoolTime_ == 0)
		{
			col_.isActive = false;
		}
		else
		{
			col_.isActive = true;
		}
	}
	//剣と当たり判定が分離している場合
	else
	{
		colSpline_.Update(GameSpeed::GetPlayerSpeed());
		//当たり判定有効フレームより小さければ無効
		if (info.colInfo.activeFrame > colSpline_.GetTimer().GetTimer())
		{
			col_.isActive = false;
		}
		else
		{
			col_.isActive = true;
			col_.radius += info.colInfo.addRadiusValue;
			colPos_ = colSpline_.GetNowPoint();
		}
	}
	col_.center = colPos_;

	obj_->SetPos(colPos_);
}

void BaseAttack::QuaternionUpdate()
{
	auto& frame = attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame;
	auto timer = attackAllTime_.GetTimer() - oneSwingTimer_;
	if (timer > frame)
	{
		if (quaternionIndex_ < attackdata_.attackinfo[index_].quaternion.size() - 1)
		{
			quaternionTimer_ += attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame;
			quaternionIndex_++;

			float time = (attackdata_.attackinfo[index_].quaternion[quaternionIndex_].frame - attackAllTime_.GetTimer());
			slerpSpeed_ = 1.f / time;
		}
	}
	auto& currentQuaternion = attackdata_.attackinfo[index_].quaternion[quaternionIndex_].q;

	resultQuaternion_ = resultQuaternion_.Slerp(currentQuaternion, 0.3f * GameSpeed::GetPlayerSpeed());

	selfActor_->GetDisplayWorldTransform()->SetQuaternion(resultQuaternion_);
}

void BaseAttack::DamageCoolTimerUpdate()
{
	auto& attackInfo = attackdata_.attackinfo[index_];
	if (attackInfo.colType_ == ColType::Normal)
	{
		//ダメージクールタイム計算
		float& attackTime = attackInfo.attackFrame;
		damageCoolTime_ = (attackTime - spline_.GetTimer().GetTimer());
	}
	else
	{
		float maxTime = attackInfo.attackAllFrame;
		float nowTime = colSpline_.GetTimer().GetTimer();
		damageCoolTime_ = maxTime - nowTime;
	}
}

void BaseAttack::EffectPopUpdate()
{
	auto& effectInfo = attackdata_.attackinfo[index_].effectInfo;
	//カメラシェイク・エフェクトを発生させる
	if (effectInfo.frame == (int32_t)attackAllTime_.GetTimer()) {
		//パーティクルの発生位置を親子関係にして計算しなおす
		if (effectInfo.emitterPosType == EmitterPos::Separate) {
			WorldTransform paritcleTransform;
			paritcleTransform.parent_ = selfActor_->GetWorldTransform();
			paritcleTransform.position_ = effectInfo.separatePos;
			paritcleTransform.Update();

			effectInfo.separatePos = paritcleTransform.GetWorldPos();
		}

		//カメラをシェイクさせる
		if (effectInfo.cameraShakePower != 0) {
			isCameraShake_ = true;
		}
		//emitter_ = std::make_shared<OneceEmitter>();
		emitter_ = std::make_unique<ContinuousEmitter>();
		//パーティクルの座標代入
		if (effectInfo.emitterPosType == EmitterPos::Player)
		{
			emitter_->pos = selfActor_->GetWorldTransform()->position_;
		}
		else if (effectInfo.emitterPosType == EmitterPos::Sword) {
			emitter_->pos = colPos_;
		}
		else {
			emitter_->pos = effectInfo.separatePos;
		}
		emitter_->addVec = attackdata_.attackinfo[index_].playerMoveVec;
		if (effectInfo.particleName != "")
		{
			ParticleManager::GetInstance()->
				AddParticle(effectInfo.particleName, emitter_);
		}
	}

	if (emitter_ != nullptr) {
		//パーティクルの座標代入
		if (effectInfo.emitterPosType == EmitterPos::Player)
		{
			emitter_->pos = selfActor_->GetWorldTransform()->position_;
		}
		else if (effectInfo.emitterPosType == EmitterPos::Sword)
		{
			emitter_->pos = colPos_;
		}
		else
		{
			emitter_->pos = effectInfo.separatePos;
		}
	}
}
