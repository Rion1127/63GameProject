#pragma once
#include <memory>
#include "EmptyScene.h"
#include "mInput.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Util.h"
class SceneManager
{
public:
	static std::unique_ptr<EmptyScene> scurrentScene_;

	static uint32_t ssceneNum_;
	static bool sisNext_;
	static bool sisDifferent;
public:
	static void Ini();
	static void Update();
	static void Draw();

	template <class NextScene>
	//なぜかインラインで宣言しないとエラーが起こる
	static inline void Transition() {
		/*deletee*/ scurrentScene_.release();
		scurrentScene_ = std::unique_ptr<NextScene>(new NextScene());
		scurrentScene_->Ini();
	}
	static inline void SetChangeStart(uint32_t sceneNum) {
		if (sisNext_ == false) {
			
			ssceneNum_ = sceneNum;
			sisNext_ = true;
			sisDifferent = true;
		}
	}
};



