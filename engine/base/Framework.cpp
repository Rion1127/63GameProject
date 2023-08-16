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
	Controller::Ini();
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
	Controller::Update();
	MouseInput::GetInstance()->Updata();
	SoundManager::Update();
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
	//�`��R�}���h
	RDirectX::GetInstance()->PreDraw();
	//�Q�[���V�[���`��
	SceneManager::Draw();
	//imgui�I��
	ImGuiManager::Getinstance()->End();
	//imgui�`��
	ImGuiManager::Getinstance()->Draw();
	//�`��I��
	RDirectX::GetInstance()->PostDraw();
}
