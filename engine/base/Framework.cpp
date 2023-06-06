#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

void Framework::Init()
{
	//winAPI������
	winApi = WinAPI::GetInstance();
	winApi->Ini();

	//DirectX������
	directX = DirectXCommon::GetInstance();
	directX->Ini(winApi);

	//�e�N�X�`���}�l�[�W���[������
	TextureManager::GetInstance()->Ini(directX->GetDevice());
	//�C���v�b�g������
	//�L�[�{�[�h
	input_ = DirectXInput::GetInstance();
	input_->InputIni();
	//�R���g���[���[
	controller = Controller::GetInstance();
	controller->Ini();
	//�}�E�X
	mouse = MouseInput::GetInstance();
	mouse->MouseIni();
	//�T�E���h������
	sound_ = SoundManager::GetInstance();
	sound_->Init();

	PipelineManager::Ini();

	//imgui������
	imguiManeger_ = ImGuiManager::Getinstance();
	imguiManeger_->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();
}

void Framework::Finalize()
{
	// �E�B���h�E�N���X��o�^����
	winApi->ReleaseClass();
	//�T�E���h�֘A���
	sound_->ReleaseAllSounds();
	//imgui���
	imguiManeger_->Finalize();

	
}

void Framework::Update()
{
	// �Q�[�����[�v

	//imgui�J�n
	imguiManeger_->Begin();
	//�C���v�b�g�֘A�X�V
	input_->InputUpdata();
	controller->Update();
	mouse->Updata();
#ifdef _DEBUG
	//�f���E�B���h�E�̕\���I��
	ImGui::ShowDemoWindow();
#endif // DEBUG
}

void Framework::Run()
{
	//������
	Init();

	while (true) {
		if (winApi->MsgCheck()) {
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
	directX->PreDraw();
	//�Q�[���V�[���`��
	SceneManager::Draw();
	//imgui�I��
	imguiManeger_->End();
	//imgui�`��
	imguiManeger_->Draw();
	//�`��I��
	directX->PostDraw();
}
