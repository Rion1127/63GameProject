#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>


class SwordTrail
{
private:
	struct PosBuffer
	{
		Vector3 head; //剣の先端の位置
		Vector3 tail; //剣の末端の位置
	};
	struct SwordTrailVertex
	{
		Vector3 pos = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
	};
private:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	//現在フレームでの剣の位置
	PosBuffer tempPos;
	//転送するかどうか
	bool isTransfer_;
public:
	SwordTrail(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& head, const Vector3& tail);
};

