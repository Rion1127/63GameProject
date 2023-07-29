#include "Sword.h"
#include "util.h"
#include <imgui.h>

Sword::Sword()
{
	obj_ = MakeUnique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("sword"));
	obj_->SetScale({ 0.3f,0.3f, 0.3f });
}

void Sword::Update()
{
	if (attackManager_->GetNowAttack() != nullptr) {
		Vector3 pos = attackManager_->GetNowAttack()->GetAttackCol()->at(0)->col_.center;
		localPos_ = pos - playerObj_->WT_.position_;
		obj_->SetPos(pos);
	}

	Vector3 PtoSVec = obj_->WT_.position_ - playerObj_->WT_.position_;
	PtoSVec = PtoSVec.normalize();

	Vector2 PtoSVec2 = {
		PtoSVec.x,
		PtoSVec.y,
	};

	obj_->WT_.SetRotType(RotType::Quaternion);
	
	obj_->WT_.quaternion_ = DirectionToDirection(Vector3(0, 1, 0), PtoSVec);

	obj_->Update();
}

void Sword::Draw()
{
	if (attackManager_->GetNowAttack() != nullptr) {
		obj_->Draw();
	}
#ifdef _DEBUG
	ImGui::Begin("Sword");

	float rot[3] = {
		obj_->GetRot().x,
		obj_->GetRot().y,
		obj_->GetRot().z,
	};

	ImGui::SliderFloat3("rot", rot,-3.14f,3.14f);

	float pos[3] = {
		obj_->WT_.matWorld_.m[3][0],
		obj_->WT_.matWorld_.m[3][1],
		obj_->WT_.matWorld_.m[3][2],
	};

	ImGui::SliderFloat3("pos", pos, -3.14f, 3.14f);

	//obj_->SetRot({ pos[0],pos[1], pos[2], });

	ImGui::End();
#endif // _DEBUG

}
