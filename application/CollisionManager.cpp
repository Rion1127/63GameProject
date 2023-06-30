#include "CollisionManager.h"

void CollisionManager::Update()
{
	//���ƃv���C���[
	if (Sphere2PlaneCol(player_->GetColPos(), floor_->GetPlaneCol()))
	{
		player_->floorColision();

		//�n�ʂɂ߂荞�܂Ȃ��悤�ɉ����o������
		while (true)
		{
			Sphere col = player_->GetColPos();
			//�n�ʂɓ������Ă������ɉ����o���Ă���
			if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
			{
				Vector3 playerPos = player_->GetWorldTransform()->position_;
				float upDist = 0.01f;

				playerPos.y += upDist;

				player_->SetAddPos({ 0,upDist,0 });
				player_->ColPosUpdate();
			}
			else
			{
				break;
			}
		}
	}
}
