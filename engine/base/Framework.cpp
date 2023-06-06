#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

void Framework::Init()
{
	//winAPI初期化
	winApi = WinAPI::GetInstance();
	winApi->Ini();

	//DirectX初期化
	directX = DirectXCommon::GetInstance();
	directX->Ini(winApi);

	//テクスチャマネージャー初期化
	TextureManager::GetInstance()->Ini(directX->GetDevice());
	//インプット初期化
	//キーボード
	input_ = DirectXInput::GetInstance();
	input_->InputIni();
	//コントローラー
	controller = Controller::GetInstance();
	controller->Ini();
	//マウス
	mouse = MouseInput::GetInstance();
	mouse->MouseIni();
	//サウンド初期化
	sound_ = SoundManager::GetInstance();
	sound_->Init();

	PipelineManager::Ini();

	//imgui初期化
	imguiManeger_ = ImGuiManager::Getinstance();
	imguiManeger_->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();
}

void Framework::Finalize()
{
	// ウィンドウクラスを登録解除
	winApi->ReleaseClass();
	//サウンド関連解放
	sound_->ReleaseAllSounds();
	//imgui解放
	imguiManeger_->Finalize();

	
}

void Framework::Update()
{
	// ゲームループ

	//imgui開始
	imguiManeger_->Begin();
	//インプット関連更新
	input_->InputUpdata();
	controller->Update();
	mouse->Updata();
#ifdef _DEBUG
	//デモウィンドウの表示オン
	ImGui::ShowDemoWindow();
#endif // DEBUG
}

void Framework::Run()
{
	//初期化
	Init();

	while (true) {
		if (winApi->MsgCheck()) {
			break;
		}

		//毎フレーム処理
		Update();
		
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}

void Framework::Draw()
{
	//描画コマンド
	directX->PreDraw();
	//ゲームシーン描画
	SceneManager::Draw();
	//imgui終了
	imguiManeger_->End();
	//imgui描画
	imguiManeger_->Draw();
	//描画終了
	directX->PostDraw();
}
