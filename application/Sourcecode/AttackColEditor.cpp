#include "AttackColEditor.h"
#include <imgui.h>

ColSpline::ColSpline()
{
	colObj_ = std::move(std::make_unique<Object3d>());
	colObj_->SetModel(Model::CreateOBJ_uniptr("sphere", false, false));

	spline_.SetIsLineDisplay(true);
	spline_.SetColor(Color(255,0,0,255));

	attackInfo_.radian = 1.f;
	attackInfo_.damage = 10;
	currentColScaleNum_ = 0;

	attackInfo_.colScales.emplace_back();

	attackInfo_.timerType = Spline::TimerType::Easing;
	attackInfo_.easingType = Spline::EasingType::Sine;
	attackInfo_.inOutType = Spline::EasingTypeInOut::In;
}

void ColSpline::Update()
{
	if (isErase_) {
		isErase_ = false;
		auto& pos = attackInfo_.splinePos;
		if (pos.size() > 0) {
			pos.erase(pos.begin() + (pos.size() - 1));
			SetSplinePoint();
		}
	}
	if (isPosChange_) {
		isPosChange_ = false;
		SetSplinePoint();
	}

	if (spline_.GetsplinePos().size()) {
		spline_.Update();
	}
	//スプラインが再生されていたら座標を代入し続ける
	if (!spline_.GetisEnd())
	{
		colObj_->SetPos(spline_.GetNowPoint());
	}
	else
	{
		spline_.SetIsStart(false);
	}
	
	auto radius = attackInfo_.radian;
	colObj_->SetScale(Vector3(radius, radius, radius));
	colObj_->Update();
}

void ColSpline::Draw()
{
	colObj_->Draw();
	spline_.DrawDebug();
}

void ColSpline::DrawImGui()
{
	ImGui::Begin("ColInfo");
	ImGuiColInfo();
	ImGui::End();

	ImGui::Begin("ColSplineEditor");
	ImGuiColSplineEditor();
	ImGuiDisplaySplinePos();
	ImGui::End();

	ImGuiColScaleTransition();
}

void ColSpline::ImGuiColSplineEditor()
{
	ImGui::DragFloat("Frame", &attackInfo_.attackFrame, 1.f, 0.1f, 50.f);

	configCommon_.SetTimerType(attackInfo_.timerType);
	configCommon_.SetEasingType(attackInfo_.easingType);
	configCommon_.SetEasingTypeInOut(attackInfo_.inOutType);
	//制御点追加
	if (ImGui::Button("AddPoint"))
	{
		attackInfo_.splinePos.emplace_back();
		SetSplinePoint();
	}
	//制御点削除
	if (ImGui::Button("DeletePoint"))
	{
		isErase_ = true;
	}
}

void ColSpline::ImGuiDisplaySplinePos()
{
	auto& pos = attackInfo_.splinePos;
	if (ImGui::CollapsingHeader("ColSplinePointPosision"))
	{
		int32_t splinePosIndex = 0;
		std::string splinePosName;
		Vector3 prePos;
		for (auto& spline : pos)
		{
			prePos = spline;
			std::ostringstream num;

			num << splinePosIndex;
			splinePosName = "Sprine" + num.str();

			float splinePos[3] = {
				spline.x,
				spline.y,
				spline.z,
			};
			ImGui::DragFloat3(splinePosName.c_str(), splinePos, 0.1f, 100.f, 100.f, "%.3f");
			spline = { splinePos[0], splinePos[1],  splinePos[2] };
			if (prePos != spline)isPosChange_ = true;
			splinePosIndex++;
		}
	}
}

void ColSpline::ImGuiColInfo()
{
	ImGui::DragFloat("Radian", &attackInfo_.radian, 0.1f, 0.1f, 50.f);
	ImGui::DragInt("Damage", &attackInfo_.damage, 1, 0, 50);
}

void ColSpline::ImGuiColScaleTransition()
{
	ImGui::Begin("ColScaleTransition");

	ImGui::DragFloat("frame", &attackInfo_.colScales[currentColScaleNum_].frame, 1.f, 1.f, 1000.f);
	ImGui::DragFloat("scale", &attackInfo_.colScales[currentColScaleNum_].scale,0.1f,0.1f,100.f);

	ImGui::End();
}

void ColSpline::SetSplinePoint()
{
	auto& spline = spline_;
	auto& splinePos = attackInfo_.splinePos;
	spline.AllClear();
	spline.SetMaxTime(attackInfo_.attackFrame);
	spline.SetTimerType_(attackInfo_.timerType);
	spline.SetEasingType_(attackInfo_.easingType);
	spline.SetEasingTypeInOut_(attackInfo_.inOutType);
	for (int32_t i = 0; i < attackInfo_.splinePos.size(); i++)
	{
		if (i == 0)
		{
			spline.AddPosition(splinePos[i], PosState::Start);
		}
		else if (i == splinePos.size() - 1)
		{
			spline.AddPosition(splinePos[i], PosState::End);
		}
		else
		{
			spline.AddPosition(splinePos[i], PosState::Middle);
		}
	}
}

void ColSpline::PlaySpline()
{
	spline_.Reset();
	spline_.SetIsStart(true);
	spline_.SetMaxTime(attackInfo_.attackFrame);
	spline_.SetTimerType_(attackInfo_.timerType);
	spline_.SetEasingType_(attackInfo_.easingType);
	spline_.SetEasingTypeInOut_(attackInfo_.inOutType);
}
