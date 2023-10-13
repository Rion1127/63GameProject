#pragma once
#include "IPostEffect.h"

/**
 * @file HighLumi.h
 * @brief 実行画像の白い部分を抽出するポストエフェクト
 */

class HighLumi final :
	public IPostEffect {
private:
	void TransferBuff() override {};
	void SendToShader() override {};
};
