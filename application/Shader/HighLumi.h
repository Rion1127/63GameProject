#pragma once

#include "IPostEffect.h"
class HighLumi :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
};
