#include "SwordTrail.h"

SwordTrail::SwordTrail(uint32_t vertSize)
{
}

void SwordTrail::Update()
{
}

void SwordTrail::SetPos(const Vector3& head, const Vector3& tail)
{
	tempPos.head = head;
	tempPos.tail = tail;
	isTransfer_ = true;
}
