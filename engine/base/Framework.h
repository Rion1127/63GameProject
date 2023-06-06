#pragma once
#include "Framework.h"
#include "mInput.h"
#include "Texture.h"
#include "Util.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "GameScene.h"
#include "mSound.h"
#include "SceneManager.h"
#include "PipelineManager.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include <imgui.h>
class Framework
{
public:
	virtual ~Framework() = default;

	virtual void Init();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	//実行
	void Run();
private:
	WinAPI* winApi = nullptr;
	//DirectX初期化
	DirectXCommon* directX = nullptr;
	//インプット初期化
	//キーボード
	DirectXInput* input_ = nullptr;
	//コントローラー
	Controller* controller = nullptr;
	//マウス
	MouseInput* mouse = nullptr;
	//サウンド初期化
	SoundManager* sound_ = nullptr;
	//imgui初期化
	ImGuiManager* imguiManeger_ = nullptr;
};

