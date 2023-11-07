#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "GameSpeed.h"

/**
 * @file Framework.ｃｐｐ
 * @brief エンジン部分の処理の流れをまとめたクラス
 */

void Framework::Init()
{
	//winAPI初期化
	WinAPI::GetInstance()->Ini();

	//DirectX初期化
	RDirectX::GetInstance()->Ini(WinAPI::GetInstance());

	//テクスチャマネージャー初期化
	TextureManager::GetInstance()->Ini();
	//インプット初期化
	//キーボード
	Key::InputIni();
	//コントローラー
	Controller::Ini();
	//マウス
	MouseInput::GetInstance()->MouseIni();
	//サウンド初期化
	SoundManager::GetInstance()->Init();

	PipelineManager::Ini();

	//imgui初期化
	ImGuiManager::Getinstance()->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();
	Spline::StaticInit();

	loadManager_.LoadAllResources();

	bloom_ = std::make_unique<Bloom>();
	test_ = std::make_unique<HighLumi>();
	test_->SetVerTex(IPostEffect::VertName::LB,Vector2(-1.0f,0.5f));
	test_->SetVerTex(IPostEffect::VertName::LT,Vector2(-1.0f,1.0f));
	test_->SetVerTex(IPostEffect::VertName::RB,Vector2(-0.5f,0.5f));
	test_->SetVerTex(IPostEffect::VertName::RT,Vector2(-0.5f,1.0f));
	isPostEffect_ = true;

}

void Framework::Finalize()
{
	// ウィンドウクラスを登録解除
	WinAPI::GetInstance()->ReleaseClass();
	//サウンド関連解放
	SoundManager::GetInstance()->ReleaseAllSounds();
	//imgui解放
	ImGuiManager::Getinstance()->Finalize();
}

void Framework::Update()
{
	// ゲームループ
	//imgui開始
	ImGuiManager::Getinstance()->Begin();
	//インプット関連更新
	Key::InputUpdata();
	Controller::Update();
	MouseInput::GetInstance()->Updata();
	SoundManager::Update();

	GameSpeed::Update();

	bloom_->Update();

#ifdef _DEBUG
	//デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
	if (ImGui::Button("PostEffect"))
	{
		isPostEffect_ = (isPostEffect_ == true) ? false : true;
	}
#endif // DEBUG
}

void Framework::Run()
{
	//初期化
	Init();

	while (true) {
		if (WinAPI::GetInstance()->MsgCheck()) {
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
	if (SceneManager::GetSceneName() == SceneName::AttackEditor)
	{
		test_->PreDrawScene();
		SceneManager::DrawRenderTexture();
		test_->PostDrawScene();
	}

	if(isPostEffect_) bloom_->PreDraw();
	//描画コマンド
	RDirectX::GetInstance()->PreDraw();

	
	//ゲームシーン描画
	if (isPostEffect_ == false)SceneManager::Draw();
	if (isPostEffect_)bloom_->Draw();

	if (SceneManager::GetSceneName() == SceneName::AttackEditor)
	{
		test_->DrawImGui();
		test_->Draw("PostEffect");
	}
	
	//imgui終了
	ImGuiManager::Getinstance()->End();
	//imgui描画
	ImGuiManager::Getinstance()->Draw();
	//描画終了
	RDirectX::GetInstance()->PostDraw();
}
