#pragma once
#include "HighLumi.h"
#include "MultiTexture.h"
#include "LineBlur.h"
#include <array>
class CrossFilter
{
private:
	std::unique_ptr<HighLumi> highLumi_;
	std::array<std::unique_ptr<LineBlur>,2> lineBlurs_;
	std::unique_ptr<MultiTexture> compo_;
public:
	CrossFilter();
	void Update();
	void PreDraw();
	void Draw();
};

