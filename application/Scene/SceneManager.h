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
	static void Ini();
	static void Update();
	static void Draw();

	template <class NextScene>
	//なぜかインラインで宣言しないとエラーが起こる
	static inline void Transition() {
		delete currentScene.release();
		currentScene = std::unique_ptr<NextScene>(new NextScene());
		currentScene->Ini();
	}
	static void SetChangeStart(int sceneNum) {
		if (isNext_ == false) {
			changeStartCount_ = GetNowCount();
			sceneNum_ = sceneNum;
			isNext_ = true;
			isDifferent = true;
		}
	}

	static std::unique_ptr<EmptyScene> currentScene;

	static Sprite sceneChangeSprite;
	static Vector2 sceneChangePos;
	static uint32_t sceneChangeGraph;
	static int changeStartCount_;
	static int sceneNum_;
	static bool isNext_;
	static int continueScene;
	static bool isDifferent;
	static int different;
};

enum SceneNum {
	TITLE_,
	TUTORIAL_,
	STAGE1_,
	STAGE2_,
	STAGE3_,
	STAGE4_,
	STAGE5_,
	RESULT_,
	GAMEOVER_
};


