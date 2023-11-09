#include "PostureDisplay.h"
#include <imgui.h>

PostureDisplay::PostureDisplay()
{
	obj_ = std::make_unique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true,false));
	obj_->SetPos(Vector3(0, 0, 0));
	obj_->SetShadowOffsetPos(Vector3(0, -1, 0));
	obj_->WT_.SetRotType(RotType::Quaternion);
	obj_->WT_.quaternion_ = IdentityQuaternion();

	camera_.SetDebugCameraMode(DebugCameraMode::Trans_Zero);
}

void PostureDisplay::Update(Quaternion q)
{
	obj_->WT_.SetQuaternion(q);
	camera_.SetFrontDist(3.8f);
	camera_.Update();

	obj_->Update(camera_.GetCamera());
}

void PostureDisplay::Draw()
{
	obj_->Draw();
}

void PostureDisplay::DrawImGui()
{
	ImGui::Begin("PostureDisplay");

	float pos[3] = { obj_ ->GetPos().x,obj_->GetPos().y, obj_->GetPos().z };

	ImGui::DragFloat3("Pos", pos,0.1f,-50.f,50.f);

	obj_->SetPos(Vector3(pos[0], pos[1], pos[2]));

	ImGui::End();
}
