#pragma once
#include "mInput.h"
#include "Texture.h"
#include "GameScene.h"
#include "mSound.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include "LoadManager.h"
#include "Bloom.h"

/**
 * @file Framework.h
 * @brief エンジン部分の処理の流れをまとめたクラス
 */

class Framework
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	LoadManager loadManager_;

	std::unique_ptr<Bloom> bloom_;
	std::unique_ptr<HighLumi> test_;
	bool isPostEffect_;
public:
	virtual ~Framework() = default;

	virtual void Init();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	//実行
	void Run();
};

