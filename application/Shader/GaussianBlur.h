#pragma once
#include "DirectX.h"
#include "IPostEffect.h"

class GaussianBlur :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
public:
	void PreDraw();
};

class RadialBlur :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
public:
	void PreDraw();
};