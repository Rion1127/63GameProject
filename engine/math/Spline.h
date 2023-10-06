#pragma once
#include "Vector3.h"
#include "Timer.h"
#include <vector>

enum class PosState {
	Start,
	Middle,
	End
};
class Spline
{
private:
	std::vector<Vector3> splinePos_;
	Vector3 nowPos_;
	Vector3 headingVec_;	//進行方向ベクトル
	uint32_t index_;
	TimerFloat timer_;
	bool isStart_;
	bool isEnd_;
public:
	Spline();

	void Update(float speedRate = 1.f);

	void DrawDebug();
	void Reset();
	void DleteAllPoint() { splinePos_.clear(); }
public:
	void SetPositions(const std::vector<Vector3>& pos) { splinePos_ = pos; }
	void AddPosition(const Vector3& pos, PosState state = PosState::Middle);
	void SetLimitTime(float time) { timer_.SetLimitTime(time); }
	void SetIsStart(bool flag) { isStart_ = flag; }
public:
	Vector3 GetNowPoint() { return nowPos_; }
	Vector3 GetHeadingVec() { return headingVec_; }
	const std::vector<Vector3> GetsplinePos() { return splinePos_; }
	bool GetisEnd() { return isEnd_; }
	uint32_t GetIndex() { return index_; }
	TimerFloat GetTimer() { return timer_; }
private:
	void SplineUpdate(float speedRate);
	const Vector3 SplinePosition(const std::vector<Vector3>& point, uint32_t startIndex, const float t);
	// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
	Vector3 GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t);
};

