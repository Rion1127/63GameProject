#pragma once
#include "DirectX.h"
#include "IPostEffect.h"

class GaussianBlur final :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
public:
	void PreDraw();
};

class RadialBlur final :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
public:
	void PreDraw();
};