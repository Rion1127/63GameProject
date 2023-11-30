#pragma once
#include "Vector3.h"
#include "Timer.h"
#include <vector>
#include "Object3d.h"
#include "Line3D.h"

/**
 * @file Spline.h
 * @brief スプライン曲線を使いやすいようにまとめたクラス
 */

enum class PosState {
	Start,
	Middle,
	End
};
class Spline
{
public:
	enum class TimerType {
		Normal,
		Easing
	};
	enum class EasingType {
		Back,
		Bounce,
		Circ,
		Quint,
		Cubic,
		Sine,
		EasingTypeEnd
	};
	enum class EasingTypeInOut {
		In,
		Out,
		InOut,
		EasingTypeInOutEnd
	};
private:
	WorldTransform worldTransform_;
	std::vector<Vector3> splinePos_;
	Vector3 nowPos_;
	Vector3 headingVec_;	//進行方向ベクトル
	uint32_t index_;
	TimerFloat timer_;
	bool isStart_;
	bool isEnd_;
	bool isLineDisplay_;

	float maxTime_;
	float nowTime_;
	float testTime_;

	TimerType timerType_;
	EasingType easingType_;
	EasingTypeInOut easeTypeInOut_;

	std::vector<std::unique_ptr<Object3d>> splineObj_;
	std::unique_ptr<Line3D> line3D_;
	std::vector<Vector3> parentSplinePos_;
	std::vector<Vector3> line3DPos_;
private:
	static std::vector<std::string> sEaseTypeName_;
public:
	Spline();

	static void StaticInit();

	void Update(float speedRate = 1.f);

	void DrawDebug();
	void Reset();
	void AllClear();
	void DeleteAllPoint() { splinePos_.clear(); }
public:
	void SetPositions(const std::vector<Vector3>& pos);
	void AddPosition(const Vector3& pos, PosState state = PosState::Middle);
	void SetLimitTime(float time) { timer_.SetLimitTime(time); }
	void SetMaxTime(float time) { maxTime_ = time; timer_.SetLimitTime(time); }
	void SetIsStart(bool flag) { isStart_ = flag; }
	void SetTimerType_(TimerType timerType) { timerType_ = timerType; }
	void SetEasingType_(EasingType easingType) { easingType_ = easingType; }
	void SetEasingTypeInOut_(EasingTypeInOut easeTypeInOut) { easeTypeInOut_ = easeTypeInOut; }
	void SetParent(WorldTransform* parent);
	void SetIsLineDisplay(bool flag) { isLineDisplay_ = flag; }
	void SetColor(Color col) { line3D_->SetColor(col); }
public:
	Vector3 GetNowPoint() { return nowPos_; }
	Vector3 GetHeadingVec() { return headingVec_; }
	const std::vector<Vector3> GetsplinePos() { return splinePos_; }
	bool GetisEnd() { return isEnd_; }
	uint32_t GetIndex() { return index_; }
	TimerFloat GetTimer() { return timer_; }
	float GetMaxTime() { return maxTime_; }
	TimerType GetTimerType() { return timerType_; }
	EasingType GetEasingType() { return easingType_; }
	EasingTypeInOut GetEasingTypeInOut() { return easeTypeInOut_; }
public:
	static std::vector<std::string> GetEaseTypeNames() { return sEaseTypeName_; }
private:
	const Vector3 SplinePosition(const std::vector<Vector3>& point, uint32_t startIndex, const float t);
	// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
	Vector3 GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t);
	void CalculateMaxTime();
	void NormalUpdate(float speedRate);
	void EasingUpdate(float speedRate);
	void EaseUpdate();
	void ObjInit();		//スプラインポイントに配置するオブジェ初期化
	void ParentUpdate();
	void Line3DUpdate();
};

