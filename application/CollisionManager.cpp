#include "CollisionManager.h"

void CollisionManager::Update()
{
	//床とプレイヤー
	if (Sphere2PlaneCol(player_->GetColPos(), floor_->GetPlaneCol()))
	{
		player_->floorColision();

		//地面にめり込まないように押し出し処理
		while (true)
		{
			Sphere col = player_->GetColPos();
			col.radius -= 0.2f;
			//地面に当たっていたら上に押し出していく
			if (Sphere2PlaneCol(col, floor_->GetPlaneCol()))
			{
				Vector3 playerPos = player_->GetWorldTransform().position_;
				float upDist = 0.01f;

				playerPos.y += upDist;

				player_->SetPos(playerPos);
				player_->ColPosUpdate();
			}
			else
			{
				break;
			}
		}
	}
}
