#pragma once
#include <memory>
#include "IScene.h"
#include "mInput.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Util.h"
#include "Timer.h"

enum class SceneName {
	Title,
	Game,
	GameOver,
	GameClear,
	None
};

class SceneManager
{
private:
	static std::unique_ptr<IScene> scurrentScene_;

	static SceneName ssceneName_;
	static bool sisSetNext_;
	static Timer animeTimer_;
	static Sprite whiteSprite_;
public:
	static void Ini();
	static void Update();
	static void Draw();

	template <class NextScene>
	static inline void Transition() {
		scurrentScene_.reset();
		scurrentScene_ = std::move(std::make_unique<NextScene>());
		scurrentScene_->Ini();
	}
	static void SetChangeStart(const SceneName sceneName);
public:
	static bool GetIsSetNext() { return sisSetNext_; }
private:
	static void SceneChange();
};



