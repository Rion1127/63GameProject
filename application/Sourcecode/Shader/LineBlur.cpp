#include "LineBlur.h"
#include "Util.h"

LineBlur::LineBlur()
{
	constBuff_ = CreateBuff(constMap_);
}

void LineBlur::TransferBuff()
{
	//定数バッファのマッピング
	ConstBuffRad* map{};
	HRESULT result;
	result = constBuff_->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	map->radian = radian_;

	constBuff_->Unmap(0, nullptr);
}

void LineBlur::SendToShader()
{
	SetBuff(1, constBuff_.Get());
}