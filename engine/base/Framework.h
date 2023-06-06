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

	//���s
	void Run();
private:
	WinAPI* winApi = nullptr;
	//DirectX������
	DirectXCommon* directX = nullptr;
	//�C���v�b�g������
	//�L�[�{�[�h
	DirectXInput* input_ = nullptr;
	//�R���g���[���[
	Controller* controller = nullptr;
	//�}�E�X
	MouseInput* mouse = nullptr;
	//�T�E���h������
	SoundManager* sound_ = nullptr;
	//imgui������
	ImGuiManager* imguiManeger_ = nullptr;
};

