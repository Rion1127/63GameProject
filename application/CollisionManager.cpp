#include "CollisionManager.h"

void CollisionManager::Update()
{
	//���ƃv���C���[
	if (Sphere2PlaneCol(player_->GetCol(), floor_->GetPlaneCol()))
	{
		//�n�ʂɂ߂荞�܂Ȃ��悤�ɉ����o������
		while (true)
		{
			float checkValue = 0.01f;
			Sphere col = player_->GetCol();
			col.center.y -= checkValue;
			col.radius = player_->GetCol().radius;
			//�n�ʂɓ������Ă������ɉ����o���Ă���
			if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
			{
				player_->floorColision();
				break;
			}

			Vector3 playerPos = player_->GetWorldTransform()->position_;
			float upDist = 0.01f;
			playerPos.y += upDist;
			player_->SetAddPos({ 0,upDist,0 });
			player_->ColPosUpdate();
		}
		
	}
		
}
