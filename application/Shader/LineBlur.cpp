#include "LineBlur.h"
#include "Util.h"

LineBlur::LineBlur()
{
	constBuff_ = CreateBuff(constMap_);
}

void LineBlur::TransferBuff()
{
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBuffRad* map{};
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	map->radian = radian_;
	
	constBuff_->Unmap(0, nullptr);
}

void LineBlur::SendToShader()
{
	SetBuff(1, constBuff_.Get());
}