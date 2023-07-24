#include "Spline.h"
#include <imgui.h>

Spline::Spline()
{
	index_ = 1;
	timer_.SetLimitTime(120);
	isEnd_ = false;
}

void Spline::Update()
{
	if (isStart_) {
		SplineUpdate();
	}
}

void Spline::SplineUpdate()
{
	timer_.AddTime(1);

	float t = timer_.GetTimeRate();

	if (t >= 1.0f) {
		//���̐���_������ꍇ
		if (index_ < splinePos_.size() - 2) {
			index_++;
			timer_.Reset();
			t = timer_.GetTimeRate();
		}
		//�ŏI�n�_�������ꍇ1.0f�ɂ��ē������~�߂�
		else {
			isEnd_ = true;
		}
	}
	nowPos_ = SplinePosition(splinePos_, index_, t);
	t += 0.01f;
	if (t <= 1.f) {
		headingVec_ = SplinePosition(splinePos_, index_, t) - nowPos_;
	}
}

void Spline::DrawDebug()
{
	ImGui::Begin("spline");
	float pos[3] = { nowPos_.x,nowPos_.y,nowPos_.z };
	ImGui::SliderFloat3("NowPos", pos, -100.0f, 100.0f);

	ImGui::End();
}

void Spline::Reset()
{
	timer_.Reset();
	isEnd_ = false;
	index_ = 1;
}

void Spline::AddPosition(const Vector3& pos, PosState state)
{
	//�X�v���C���Ȑ��̂͂��߂ƏI��肾������2��}������
	if (state == PosState::Start ||
		state == PosState::End) {
		splinePos_.push_back(pos);
		splinePos_.push_back(pos);
	}
	else if (state == PosState::Middle) {
		splinePos_.push_back(pos);
	}
}


const Vector3 Spline::SplinePosition(const std::vector<Vector3>& point, uint32_t startIndex, const float t)
{
	////�⊮���ׂ��_�̐�
	size_t n = point.size() - 2;

	if (startIndex > n)return point[n];//Pn�̒l��Ԃ�
	if (startIndex < 1)return point[1];//P1�̒l��Ԃ�

	//p0~p3�̐���_���擾����@��p1~p2��⊮����
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

// �n�_/�I�_�̍��W�� �x�N�g������A�Ȑ��̋O����̍��W��Ԃ�
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