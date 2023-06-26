#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

void Framework::Init()
{
	//winAPI������
	WinAPI::GetInstance()->Ini();

	//DirectX������
	RDirectX::GetInstance()->Ini(WinAPI::GetInstance());

	//�e�N�X�`���}�l�[�W���[������
	TextureManager::GetInstance()->Ini();
	//�C���v�b�g������
	//�L�[�{�[�h
	Key::InputIni();
	//�R���g���[���[
	Controller::GetInstance()->Ini();
	//�}�E�X
	MouseInput::GetInstance()->MouseIni();
	//�T�E���h������
	SoundManager::GetInstance()->Init();

	PipelineManager::Ini();

	//imgui������
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
	// �E�B���h�E�N���X��o�^����
	WinAPI::GetInstance()->ReleaseClass();
	//�T�E���h�֘A���
	SoundManager::GetInstance()->ReleaseAllSounds();
	//imgui���
	ImGuiManager::Getinstance()->Finalize();
}

void Framework::Update()
{
	// �Q�[�����[�v
	//imgui�J�n
	ImGuiManager::Getinstance()->Begin();
	//�C���v�b�g�֘A�X�V
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
	//�f���E�B���h�E�̕\���I��
	//ImGui::ShowDemoWindow();
#endif // DEBUG
}

void Framework::Run()
{
	//������
	Init();

	while (true) {
		if (WinAPI::GetInstance()->MsgCheck()) {
			break;
		}
		//���t���[������
		Update();
		//�`��
		Draw();
	}
	//�Q�[���̏I��
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
		//�Q�[���V�[���ɃK�E�V�A���u���[���|����
		gaussianBlur_->PreDraw();
		//�}���`�e�N�X�`��0�ԂɃK�E�V�A���u���[���Z�b�g
		multiRenderTexture_->PreDrawSceneAssin(0);
		gaussianBlur_->Draw("Gaussian");
		multiRenderTexture_->PostDrawSceneAssin(0);
		//�}���`�e�N�X�`��1�Ԃɒʏ�̕`����Z�b�g
		multiRenderTexture_->PreDrawSceneAssin(1);
		SceneManager::Draw();
		multiRenderTexture_->PostDrawSceneAssin(1);
	}

	//�`��R�}���h
	RDirectX::GetInstance()->PreDraw();
	//�Q�[���V�[���`��
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
	//imgui�I��
	ImGuiManager::Getinstance()->End();
	//imgui�`��
	ImGuiManager::Getinstance()->Draw();
	//�`��I��
	RDirectX::GetInstance()->PostDraw();
}
