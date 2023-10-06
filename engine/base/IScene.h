#pragma once
#include <stdint.h>
class IScene
{
public:
	//空の関数を宣言しておく
	IScene() {
	};
	virtual void Ini() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawPostEffect() = 0;

	uint32_t GetIsNext() { return isNext_; }
	void SetIsNext(uint32_t isnext) { isNext_ = isnext; }

	uint32_t isNext_ = false;

	virtual ~IScene() {
	};
};

