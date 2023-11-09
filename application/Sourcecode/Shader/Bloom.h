#pragma once
#include "IPostEffect.h"
#include "GaussianBlur.h"
#include "MultiTexture.h"
#include "HighLumi.h"

/**
 * @file Bloom.h
 * @brief 画面の白い部分に光のフリンジを追加するポストエフェクト
 */

class Bloom {
private:
	std::unique_ptr<HighLumi> highLumi_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;
	std::unique_ptr<MultiTexture> compo_;
public:
	Bloom();
	void Update();
	void PreDraw();
	void Draw();
};

