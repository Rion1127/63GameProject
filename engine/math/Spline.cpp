#include "Spline.h"
#include <imgui.h>
#include "GameSpeed.h"
#include "Easing.h"

/**
 * @file Spline.cpp
 * @brief スプライン曲線を使いやすいようにまとめたクラス
 */

std::vector<std::string> Spline::sEaseTypeName_;

Spline::Spline()
{
	index_ = 1;
	timer_.SetLimitTime(120);
	isEnd_ = false;

	timerType_ = TimerType::Normal;
	easingType_ = EasingType::Sine;
	easeTypeInOut_ = EasingTypeInOut::In;
	isLineDisplay_ = false;

	line3D_ = std::make_unique<Line3D>(20);
	line3DPos_.resize(line3D_->GetVertSize());
}

void Spline::StaticInit()
{
	sEaseTypeName_.emplace_back("Back");
	sEaseTypeName_.emplace_back("Bounce");
	sEaseTypeName_.emplace_back("Circ");
	sEaseTypeName_.emplace_back("Quint");
	sEaseTypeName_.emplace_back("Cubic");
	sEaseTypeName_.emplace_back("Sine");
}

void Spline::Update(float speedRate)
{
	if (isStart_) {
		if (timerType_ == TimerType::Normal) {
			NormalUpdate(speedRate);
		}
		else if (timerType_ == TimerType::Easing) {
			EasingUpdate(speedRate);
		}
	}
	if (isLineDisplay_) {
		Line3DUpdate();
		line3D_->Update();
		for (int32_t i = 0; i < splineObj_.size(); i++) {
			splineObj_[i]->Update();
		}
		for (int32_t i = 0; i < splineObj_.size(); i++) {
			Vector3 nextVec;
			Quaternion q;
			//次のオブジェがある時
			if (i <= splineObj_.size() - 2) {
				nextVec = splineObj_[i + 1]->WT_.GetWorldPos() - splineObj_[i]->WT_.GetWorldPos();
				nextVec = nextVec.normalize();
				q = DirectionToDirection(Vector3(0, 1, 0), nextVec);
			}
			//最後のオブジェの時
			else {
				nextVec = splineObj_[i]->WT_.GetWorldPos() - splineObj_[i - 1]->WT_.GetWorldPos();
				nextVec = nextVec.normalize();
				q = DirectionToDirection(Vector3(0, 1, 0), nextVec);
			}
			splineObj_[i]->WT_.quaternion_ = q;

			const auto& color = line3D_->GetColor();
			splineObj_[i]->SetAmbient("cone", Vector3(color.r, color.g, color.b));
			splineObj_[i]->Update();
		}
	}
}

void Spline::DrawDebug()
{
	line3D_->Draw();
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	for (int32_t i = 0; i < splineObj_.size(); i++) {
		splineObj_[i]->Draw();
	}
}

void Spline::Reset()
{
	timer_.Reset();
	isEnd_ = false;
	index_ = 1;
	testTime_ = 0;
	maxTime_ = 0;
	nowTime_ = 0;
}

void Spline::AllClear()
{
	Reset();
	splinePos_.clear();
	splineObj_.clear();
}

void Spline::SetPositions(const std::vector<Vector3>& pos)
{
	splinePos_ = pos;

	CalculateMaxTime();

	ObjInit();
}

void Spline::AddPosition(const Vector3& pos, PosState state)
{
	//スプライン曲線のはじめと終わりだったら2回挿入する
	if (state == PosState::Start ||
		state == PosState::End) {
		splinePos_.push_back(pos);
		splinePos_.push_back(pos);
	}
	else if (state == PosState::Middle) {
		splinePos_.push_back(pos);
	}

	if (state == PosState::End) {
		CalculateMaxTime();

		ObjInit();
	}
}

void Spline::SetParent(WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
	worldTransform_.rotType = parent->rotType;
}


const Vector3 Spline::SplinePosition(const std::vector<Vector3>& point, uint32_t startIndex, const float t)
{
	////補完すべき点の数
	size_t n = point.size() - 2;

	if (startIndex > n)return point[n];//Pnの値を返す
	if (startIndex < 1)return point[1];//P1の値を返す

	//p0~p3の制御点を取得する　※p1~p2を補完する
	Vector3 p0 = point[startIndex];
	Vector3 p1 = point[startIndex + 1];
	Vector3 p2;
	if (startIndex > 0)
	{
		p2 = 0.5f * (point[startIndex + 1] - point[startIndex - 1]);
	}
	else
	{
		p2 = point[startIndex + 1] - point[startIndex];
	}
	Vector3 p3;
	if (startIndex < n)
	{
		p3 = 0.5f * (point[startIndex + 2] - point[startIndex]);
	}
	else
	{
		p3 = point[startIndex + 1] - point[startIndex];
	}

	Vector3 position = GetPoint(p0, p1, p2, p3, t);

	return position;
}

// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
Vector3 Spline::GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t)
{
	Vector3 c0 = 2.0f * p0 + -2.0f * p1 + v0 + v1;
	Vector3 c1 = -3.0f * p0 + 3.0f * p1 + -2.0f * v0 - v1;
	Vector3 c2 = v0;
	Vector3 c3 = p0;

	float t2 = t * t;
	float t3 = t2 * t;
	return c0 * t3 + c1 * t2 + c2 * t + c3;
}

void Spline::CalculateMaxTime()
{
	if (maxTime_ == 0) {
		float pointNum = (float)splinePos_.size() - 2.f;

		maxTime_ = pointNum * timer_.GetLimitTimer();
	}
}

void Spline::NormalUpdate(float speedRate)
{
	timer_.AddTime(speedRate);

	float t = timer_.GetTimeRate();

	if (t >= 1.0f) {
		//次の制御点がある場合
		if (index_ < splinePos_.size() - 3) {
			index_++;
			timer_.Reset();
			t = timer_.GetTimeRate();
		}
		//最終地点だった場合1.0fにして動きを止める
		else {
			isEnd_ = true;
		}
	}
	worldTransform_.Update();
	if (worldTransform_.parent_ != nullptr) {
		ParentUpdate();
		nowPos_ = SplinePosition(parentSplinePos_, index_, t);
		t += 0.01f;
		if (t <= 1.f) {
			headingVec_ = SplinePosition(parentSplinePos_, index_, t) - nowPos_;
		}
	}
	else {
		nowPos_ = SplinePosition(splinePos_, index_, t);
		t += 0.01f;
		if (t <= 1.f) {
			headingVec_ = SplinePosition(splinePos_, index_, t) - nowPos_;
		}
	}
}

void Spline::EasingUpdate(float speedRate)
{
	EaseUpdate();

	timer_.AddTime(speedRate);

	float pointNum = (float)(splinePos_.size() - 2);
	float offsetTime = maxTime_ / pointNum;
	if (offsetTime <= (nowTime_ - testTime_)) {
		//次の制御点がある場合
		if (index_ < splinePos_.size() - 2) {
			index_++;

			testTime_ += offsetTime;
		}
		//最終地点だった場合1.0fにして動きを止める
		else {
			isEnd_ = true;
		}
	}
	float splineRate = (nowTime_ - testTime_) / offsetTime;

	if (worldTransform_.parent_ != nullptr) {
		ParentUpdate();
		nowPos_ = SplinePosition(parentSplinePos_, index_, splineRate);
		splineRate += 0.01f;
		if (splineRate <= 1.f) {
			headingVec_ = SplinePosition(parentSplinePos_, index_, splineRate) - nowPos_;
		}
	}
	else {
		nowPos_ = SplinePosition(splinePos_, index_, splineRate);
		splineRate += 0.01f;
		if (splineRate <= 1.f) {
			headingVec_ = SplinePosition(splinePos_, index_, splineRate) - nowPos_;
		}
	}
}

void Spline::EaseUpdate()
{
	if (easingType_ == EasingType::Back) {
		if (easeTypeInOut_ == EasingTypeInOut::In) {
			nowTime_ = Easing::Back::easeIn(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::Out) {
			nowTime_ = Easing::Back::easeOut(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::InOut) {
			nowTime_ = Easing::Back::easeInOut(0, maxTime_, timer_.GetTimeRate());
		}
	}
	else if (easingType_ == EasingType::Bounce) {
		if (easeTypeInOut_ == EasingTypeInOut::In) {
			nowTime_ = Easing::Bounce::easeIn(timer_.GetTimeRate(), 0, maxTime_, 1.f);
		}
		else if (easeTypeInOut_ == EasingTypeInOut::Out) {
			nowTime_ = Easing::Bounce::easeOut(timer_.GetTimeRate(), 0, maxTime_, 1.f);
		}
		else if (easeTypeInOut_ == EasingTypeInOut::InOut) {
			nowTime_ = Easing::Bounce::easeInOut(timer_.GetTimeRate(), 0, maxTime_, 1.f);
		}
	}
	else if (easingType_ == EasingType::Circ) {
		if (easeTypeInOut_ == EasingTypeInOut::In) {
			nowTime_ = Easing::Circ::easeIn(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::Out) {
			nowTime_ = Easing::Circ::easeOut(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::InOut) {
			nowTime_ = Easing::Circ::easeInOut(0, maxTime_, timer_.GetTimeRate());
		}
	}
	else if (easingType_ == EasingType::Cubic) {
		if (easeTypeInOut_ == EasingTypeInOut::In) {
			nowTime_ = Easing::Cubic::easeIn(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::Out) {
			nowTime_ = Easing::Cubic::easeOut(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::InOut) {
			nowTime_ = Easing::Cubic::easeInOut(0, maxTime_, timer_.GetTimeRate());
		}
	}
	else if (easingType_ == EasingType::Quint) {
		if (easeTypeInOut_ == EasingTypeInOut::In) {
			nowTime_ = Easing::Quint::easeIn(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::Out) {
			nowTime_ = Easing::Quint::easeOut(0, maxTime_, timer_.GetTimeRate());
		}
		else if (easeTypeInOut_ == EasingTypeInOut::InOut) {
			nowTime_ = Easing::Quint::easeInOut(0, maxTime_, timer_.GetTimeRate());
		}
	}
	else if (easingType_ == EasingType::Sine) {
		if (easeTypeInOut_ == EasingTypeInOut::In) {
			nowTime_ = Easing::Sine::easeIn(timer_.GetTimeRate(), 0, maxTime_, 1.f);
		}
		else if (easeTypeInOut_ == EasingTypeInOut::Out) {
			nowTime_ = Easing::Sine::easeOut(timer_.GetTimeRate(), 0, maxTime_, 1.f);
		}
		else if (easeTypeInOut_ == EasingTypeInOut::InOut) {
			nowTime_ = Easing::Sine::easeInOut(timer_.GetTimeRate(), 0, maxTime_, 1.f);
		}
	}
}

void Spline::ObjInit()
{
	splineObj_.resize(splinePos_.size() - 2);

	std::vector<Vector3> pos;
	for (int32_t i = 0; i < splinePos_.size(); i++) {
		if (i > 0 && i < splinePos_.size() - 1) {
			pos.emplace_back(splinePos_[i]);
		}
	}

	for (int32_t i = 0; i < splineObj_.size(); i++) {
		splineObj_[i] = std::make_unique<Object3d>();
		splineObj_[i]->SetModel(Model::CreateOBJ_uniptr("cone", false,false));
		splineObj_[i]->SetPos(pos[i]);
		splineObj_[i]->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		splineObj_[i]->WT_.rotType = RotType::Quaternion;
		splineObj_[i]->WT_.parent_ = worldTransform_.parent_;
	}
}

void Spline::ParentUpdate()
{
	if (parentSplinePos_.size() != splinePos_.size())
	{
		parentSplinePos_.clear();
		parentSplinePos_.resize(splinePos_.size());
	}
	for (int32_t i = 0; i < splinePos_.size(); i++) {
		worldTransform_.position_ = splinePos_[i];
		worldTransform_.Update();

		parentSplinePos_[i] = worldTransform_.GetWorldPos();
	}
}

void Spline::Line3DUpdate()
{
	if (splinePos_.size() == 0)return;

	if (worldTransform_.parent_ != nullptr) {
		ParentUpdate();
	}
	else {
		parentSplinePos_ = splinePos_;
	}

	int32_t activeNum = (int32_t)parentSplinePos_.size() - 2;
	float t = 0;
	float addRate = (1.f * activeNum) / (line3D_->GetVertSize() - 1);
	int32_t index = 1;
	int32_t line3DPosIndex = 0;
	while (index <= activeNum) {
		if (line3DPos_.size() <= line3DPosIndex)break;
		//次の制御点がある場合
		if (t >= 1.0f) {
			index++;
			t -= 1.f;
		}
		if (index >= activeNum)t = 0;
		line3DPos_[line3DPosIndex] = SplinePosition(parentSplinePos_, index, t);
		t += addRate;
		line3DPosIndex++;
	}
	if (splinePos_.size() > 2)
	{
		line3D_->SetVertPos(line3DPos_);
	}
}
