#include "AttackColEditor.h"
#include <imgui.h>
#include <fstream>
#include <iostream>

ColSpline::ColSpline()
{
	colObj_ = std::move(std::make_unique<Object3d>());
	colObj_->SetModel(Model::CreateOBJ_uniptr("sphere", false, false));

	spline_.SetIsLineDisplay(true);
	spline_.SetColor(Color(255,0,0,255));

	attackInfo_.firstRadius = 1.f;
	attackInfo_.damage = 10;
	currentColScaleNum_ = 0;

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
	if (!spline_.GetTimer().GetIsEnd())
	{
		colObj_->SetPos(spline_.GetNowPoint());
		colRadius_ += attackInfo_.addRadiusValue;
	}
	else
	{
		spline_.SetIsStart(false);
	}
	
	colObj_->SetAmbient("sphere",Vector3(255,0,0));
	colObj_->SetScale(Vector3(colRadius_, colRadius_, colRadius_));
	colObj_->Update();
}

void ColSpline::Draw()
{
	if ((spline_.GetTimer().GetTimer() > attackInfo_.activeFrame) && 
		!spline_.GetTimer().GetIsEnd()) {
		colObj_->Draw();
	}
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

}

void ColSpline::ImGuiColSplineEditor()
{
	ImGui::DragFloat("Frame", &attackInfo_.attackFrame, 1.f, 1.0f, 50.f);

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
	ImGui::DragFloat("Radian", &attackInfo_.firstRadius, 0.1f, 0.1f, 50.f);
	ImGui::DragFloat("addRadiusValue", &attackInfo_.addRadiusValue, 0.1f, 0.0f, 50.f);
	ImGui::DragInt("Damage", &attackInfo_.damage, 1, 0, 50);
	ImGui::DragFloat("ActiveFrame", &attackInfo_.activeFrame, 1.f, 0.1f, 50.f);
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
	colRadius_ = attackInfo_.firstRadius;
}

void ColSpline::OutPutStatus(std::ofstream& writing_file)
{
	auto& info = attackInfo_;
	std::string writing_text = "//--SeparateCol--//";
	writing_file << writing_text << std::endl;
	writing_text = "colattackFrame";
	writing_file << writing_text << " = " << info.attackFrame << std::endl;
	writing_text = "colactiveFrame";
	writing_file << writing_text << " = " << info.activeFrame << std::endl;
	writing_text = "coldamage";
	writing_file << writing_text << " = " << info.damage << std::endl;
	writing_text = "colfirstRadius";
	writing_file << writing_text << " = " << info.firstRadius << std::endl;
	writing_text = "coladdRadiusValue";
	writing_file << writing_text << " = " << info.addRadiusValue << std::endl;
	
	std::string timerType;
	//イージングの場合
	writing_text = "colTimerType ";
	if (spline_.GetTimerType() == Spline::TimerType::Normal)
	{
		timerType = "Normal";
		writing_file << writing_text << timerType << std::endl;
	}
	else
	{
		std::string easingType;
		timerType = "Easing";
		writing_file << writing_text << timerType << std::endl;

		writing_text = "colEasingType ";
		easingType = configCommon_.GetEasingType();
		writing_file << writing_text << easingType << std::endl;

		writing_text = "colEasingTypeInOut ";
		easingType = configCommon_.GetEasingInOutType();
		writing_file << writing_text << easingType << std::endl;
	}
	writing_file << std::endl;
	writing_text = "//--ColSplinePos--//";
	writing_file << writing_text << std::endl;
	//スプライトの座標を出力する
	for (auto& spline : attackInfo_.splinePos)
	{
		Vector3& pos = spline;
		writing_text = "ColSplinePos";
		writing_file << writing_text << " " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}
}

void ColSpline::InPutStatus(const std::string& key, std::stringstream& line_stream)
{
	auto& info = attackInfo_;
	//各パラメータ読み込み
	if (key == "colattackFrame")
	{
		line_stream.ignore(1, '=');
		line_stream >> info.attackFrame;
	}
	else if (key == "colactiveFrame")
	{
		line_stream.ignore(1, '=');
		line_stream >> info.activeFrame;
	}
	else if (key == "coldamage")
	{
		line_stream.ignore(1, '=');
		line_stream >> info.damage;
	}
	else if (key == "colfirstRadius")
	{
		line_stream.ignore(1, '=');
		line_stream >> info.firstRadius;
	}
	else if (key == "coladdRadiusValue")
	{
		line_stream.ignore(1, '=');
		line_stream >> info.addRadiusValue;
	}
	if (key == "colTimerType")
	{
		std::string timerTypeName;
		line_stream >> timerTypeName;

		Spline::TimerType timerType;
		if (timerTypeName == "Normal")timerType = Spline::TimerType::Normal;
		else timerType = Spline::TimerType::Easing;
		info.timerType = timerType;
	}
	//イージングの種類読み込み
	if (key == "colEasingType")
	{
		std::string easingType;
		line_stream >> easingType;
		configCommon_.SetEasingTypeName(easingType);

		for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
		{
			if (easingType == Spline::GetEaseTypeNames()[i])
			{
				info.easingType = ((Spline::EasingType)i);
				break;
			}
		}
	}
	//イージング・インアウト読み込み
	if (key == "colEasingTypeInOut")
	{
		std::string easingTypeInOut;
		Spline::EasingTypeInOut typeInout;
		line_stream >> easingTypeInOut;
		configCommon_.SetEasingTypeInOutName(easingTypeInOut);

		if (easingTypeInOut == "In")typeInout = Spline::EasingTypeInOut::In;
		else if (easingTypeInOut == "Out")typeInout = Spline::EasingTypeInOut::Out;
		else typeInout = Spline::EasingTypeInOut::InOut;

		info.inOutType = typeInout;
	}
	if (key == "//--ColSplinePos--//")
	{
		info.splinePos.clear();
	}
	if (key == "ColSplinePos")
	{
		Vector3 pos;

		line_stream >> pos.x;
		line_stream >> pos.y;
		line_stream >> pos.z;
		info.splinePos.emplace_back(pos);

		isPosChange_ = true;
	}
}
