#pragma once
#include "DirectX.h"
#include "IPostEffect.h"

/**
 * @file GaussianBlur.h
 * @brief 描画がぼやけるポストエフェクト
 */

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