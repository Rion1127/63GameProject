#include "DebugActor.h"

DebugActor::DebugActor() : IActor(ActorType::Player)
{
	gravity_.SetAddValue({ 0,-0.01f,0 });

	obj_ = std::make_unique<Object3d>();
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true, false));
	obj_->SetPos(Vector3(0, 1, 0));
	obj_->WT_.SetRotType(RotType::Quaternion);
	obj_->WT_.quaternion_ = IdentityQuaternion();

	displayObj_ = std::make_unique<Object3d>();
	displayObj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	displayObj_->SetShadowOffsetPos(Vector3(0, -1, 0));
	displayObj_->SetPos(Vector3(0, 1, 0));
	displayObj_->WT_.SetRotType(RotType::Quaternion);
	displayObj_->WT_.quaternion_ = IdentityQuaternion();
}

void DebugActor::PreUpdate()
{
	
	Vector3 gravity = {
		0,
		-0.01f * GameSpeed::GetPlayerSpeed(),
		0
	};
	gravity_.SetAddValue(gravity);
	gravity_.Update();

	//モデルの原点を下にしているためその分ずらす
	Vector3 colPos = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y + obj_->GetTransform()->scale_.y,
		obj_->GetTransform()->position_.z,
	};

	col_.SetPos(colPos);
	col_.radius = 1.f;
}

void DebugActor::CollisionUpdate()
{
	Plane floor;
	floor.distance = 1;
	floor.normal = Vector3(0, 1, 0);
	Sphere col = col_;
	col.center += gravity_.GetGravityValue();
	//col.center.y -= 1;

	//床とプレイヤー
	if (Sphere2PlaneCol(col, floor))
	{
		//地面にめり込まないよう処理
		while (true)
		{
			float dist = col_.center.y - floor.distance;
			if (col_.radius <= dist)
			{
				Vector3 pushBackVec = floor.normal * dist;
				obj_->GetTransform()->position_ -= pushBackVec;
			}
			//モデルの原点を下にしているためその分ずらす
			Vector3 colPos = {
				obj_->GetTransform()->position_.x,
				obj_->GetTransform()->position_.y,
				obj_->GetTransform()->position_.z,
			};

			col_.SetPos(colPos);
			col_.radius = 1.f;

			Vector3 pos = { 10,10,10 };
			if (Sphere2PlaneCol(col_, floor, &pos))
			{
				gravity_.SetGrabity({ 0,0,0 });
				//pos.y += 1.f;
				obj_->WT_.SetPosition(pos);
				break;
			}
		}
	}
}

void DebugActor::PostUpdate()
{
	ObjUpdate();

	displayObj_->SetPos(obj_->GetPos());
	displayObj_->Update();
}

void DebugActor::Draw()
{
	displayObj_->Draw();
}
