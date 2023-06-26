#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

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
	Controller::GetInstance()->Ini();
	//マウス
	MouseInput::GetInstance()->MouseIni();
	//サウンド初期化
	SoundManager::GetInstance()->Init();

	PipelineManager::Ini();

	//imgui初期化
	ImGuiManager::Getinstance()->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();

	loadManager_.LoadAllResources();

	bloom_ = std::move(std::make_unique<Bloom>());
	noise_ = std::move(std::make_unique<Noise>());
	gaussianBlur_ = std::move(std::make_unique<GaussianBlur>());
	radialBlur_ = std::move(std::make_unique<RadialBlur>());
	crossFilter_ = std::move(std::make_unique<CrossFilter>());
	multiRenderTexture_ = std::move(std::make_unique<MultiTexture>(2));
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
	Controller::GetInstance()->Update();
	MouseInput::GetInstance()->Updata();

	const char* postEffectName = "None";

	if (postEffectnum >= (size_t)PostEffectName::END)postEffectnum = 0;

	if (PostEffectName::Gaussian == PostEffectName(postEffectnum)) {
		gaussianBlur_->PUpdate();
		postEffectName = "Gaussian";
	}
	else if (PostEffectName::RadialBlur == PostEffectName(postEffectnum)) {
		radialBlur_->PUpdate();
		postEffectName = "RadialBlur";
	}
	else if (PostEffectName::Bloom == PostEffectName(postEffectnum)) {
		bloom_->Update();
		postEffectName = "Bloom";
	}
	else if (PostEffectName::Noise == PostEffectName(postEffectnum)) {
		noise_->PUpdate();
		postEffectName = "Noise";
	}
	else if (PostEffectName::CrossFilter == PostEffectName(postEffectnum)) {
		crossFilter_->Update();
		postEffectName = "Crossfilter";
	}else if (PostEffectName::MultiRenderTexture == PostEffectName(postEffectnum)) {
		multiRenderTexture_->PUpdate();
		postEffectName = "MultiRenderTexture";
	}

	ImGui::Begin("postEffect");
	if (ImGui::Button("Change PostEffect"))postEffectnum++;
	ImGui::Text(postEffectName);
	ImGui::End();
#ifdef _DEBUG
	//デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
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
	if (PostEffectName::Gaussian == PostEffectName(postEffectnum)) {
		gaussianBlur_->PreDraw();
	}
	else if (PostEffectName::RadialBlur == PostEffectName(postEffectnum)) {
		radialBlur_->PreDraw();
	}
	else if (PostEffectName::Bloom == PostEffectName(postEffectnum)) {
		bloom_->PreDraw();
	}
	else if (PostEffectName::Noise == PostEffectName(postEffectnum)) {
		noise_->PreDraw();
	}
	else if (PostEffectName::CrossFilter == PostEffectName(postEffectnum)) {
		crossFilter_->PreDraw();
	}
	else if (PostEffectName::MultiRenderTexture == PostEffectName(postEffectnum)) {
		//ゲームシーンにガウシアンブラーを掛ける
		gaussianBlur_->PreDraw();
		//マルチテクスチャ0番にガウシアンブラーをセット
		multiRenderTexture_->PreDrawSceneAssin(0);
		gaussianBlur_->Draw("Gaussian");
		multiRenderTexture_->PostDrawSceneAssin(0);
		//マルチテクスチャ1番に通常の描画をセット
		multiRenderTexture_->PreDrawSceneAssin(1);
		SceneManager::Draw();
		multiRenderTexture_->PostDrawSceneAssin(1);
	}

	//描画コマンド
	RDirectX::GetInstance()->PreDraw();
	//ゲームシーン描画
	if (PostEffectName::None == PostEffectName(postEffectnum)) {
		SceneManager::Draw();
	}
	else if (PostEffectName::Gaussian == PostEffectName(postEffectnum)) {
		gaussianBlur_->Draw("Gaussian");
	}
	else if (PostEffectName::RadialBlur == PostEffectName(postEffectnum)) {
		radialBlur_->Draw("RadialBlur");
	}
	else if (PostEffectName::Bloom == PostEffectName(postEffectnum)) {
		bloom_->Draw();
	}
	else if (PostEffectName::Noise == PostEffectName(postEffectnum)) {
		noise_->Draw("Noise");
	}
	else if (PostEffectName::CrossFilter == PostEffectName(postEffectnum)) {
		crossFilter_->Draw();
	}
	else if (PostEffectName::MultiRenderTexture == PostEffectName(postEffectnum)) {
		multiRenderTexture_->Draw("MultiRenderTexture");
	}
	//imgui終了
	ImGuiManager::Getinstance()->End();
	//imgui描画
	ImGuiManager::Getinstance()->Draw();
	//描画終了
	RDirectX::GetInstance()->PostDraw();
}
