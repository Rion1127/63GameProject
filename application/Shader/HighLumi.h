#pragma once

#include "IPostEffect.h"
class HighLumi final:
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
};
