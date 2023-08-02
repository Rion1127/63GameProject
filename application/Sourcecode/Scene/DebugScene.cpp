#include "DebugScene.h"

#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "EnemyLoader.h"

#include "JsonLoader.h"
#include <imgui.h>

DebugScene::~DebugScene()
{

}

void DebugScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	lightManager_ = std::make_shared<LightManager>();
	colManager_ = std::make_unique<CollisionManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	operationUI_ = std::make_unique<UIOperation>();
	colosseumSystem_ = std::make_unique<ColosseumSystem>();

	stage_ = std::move(std::make_unique<Stage>());

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());

	IEnemy::SetPlayer(player_.get());
	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(stage_.get());
	colManager_->SetEnemys(enemyManager_.get());
	AttackManager::SetPlayer(player_.get());
	enemyManager_->SetPlayer(player_.get());
	colosseumSystem_->SetPlayer(player_.get());
	colosseumSystem_->SetEnemy(enemyManager_.get());

	JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
	JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), "Debug", 1);

	
}

void DebugScene::Update()
{
#ifdef _DEBUG
	if (Key::TriggerKey(DIK_R))
	{
		JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
		JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	}

#endif // _DEBUG
	colosseumSystem_->Update();

	CameraUpdate();
	//�����蔻��O�X�V
	stage_->Update();
	player_->PreUpdate();
	enemyManager_->PreUpdate();

#ifdef _DEBUG
	LoadEnemyImGui();
#endif // _DEBUG
	//�����蔻��
	colManager_->Update();
	//�����蔻���X�V
	enemyManager_->PostUpdate();
	player_->PostUpdate();
	//���̑�
	lightManager_->DebugUpdate();
	ParticleManager::GetInstance()->Update();
	//UI�X�V
	operationUI_->Update();
	//�v���C���[�����񂾂�V�[���ύX
	if (player_->GetIsAlive() == false)
	{
		SceneManager::SetChangeStart(SceneName::GameOver);
	}

	if (Key::TriggerKey(DIK_P))
	{
		ParticleManager::GetInstance()->
			AddParticle("EnemyDead", 9, 80, { 0,2,0 }, { 0.5f,0.5f, 0.5f }, 2.f);
	}


}

void DebugScene::Draw()
{

	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	////////////////
	//3D�I�u�W�F�N�g//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	stage_->Draw();
	player_->Draw();
	enemyManager_->Draw();

	PipelineManager::PreDraw("Toon", TRIANGLELIST);


	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	////////////
	//�X�v���C�g//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteDraw();
	player_->DrawSprite();
	operationUI_->Draw();
	colosseumSystem_->DrawSprite();

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
}

void DebugScene::CameraUpdate()
{
	static bool isDebug = true;
	if (Key::TriggerKey(DIK_Q))
	{
		isDebug = isDebug == false ? true : false;
	}
	if (isDebug)
	{
		gameCamera_.Update(CameraMode::LookAT);
		Camera::scurrent_ = gameCamera_.GetCamera();
	}
	else
	{
		//�J�����X�V
		debugCamera_.Update();
		Camera::scurrent_ = debugCamera_.GetCamera();
	}
	Camera::scurrent_->Update(CameraMode::LookAT);
}

void DebugScene::LoadEnemyImGui()
{
	std::string fileName_;
	ImGui::Begin("EnemyRound");

	//�f�[�^�v�[���̃n�b�V���l��fileName�ɑ��
	std::vector<std::string> fileName;
	auto datasize = EnemyLoader::GetInstance()->GetAllData();
	for (auto itr = datasize.begin(); itr != datasize.end(); ++itr)
	{
		fileName.push_back(itr->first);
	}
	//�v���_�E�����j���[�œǂݍ��񂾃t�@�C����I���ł���悤�ɂ���
	static std::string s_currentItem;
	if (ImGui::BeginCombo("Combo", s_currentItem.c_str()))
	{
		for (int i = 0; i < fileName.size() - 1; ++i)
		{
			//�I���������̂ƃn�b�V���l����v������s_currentItem�Ƀn�b�V���l����
			const bool is_selected = (s_currentItem == fileName[i]);
			if (ImGui::Selectable(fileName[i].c_str(), is_selected))
			{
				s_currentItem = fileName[i].c_str();
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	//�ǂݍ��񂾃t�@�C���̃��E���h�������
	size_t RoundMax = EnemyLoader::GetInstance()->GetEnemyData(s_currentItem).size();
	static int roundNum = 0;
	ImGui::DragInt("Round", &roundNum, 1.f, 1, (int)RoundMax);
	//��L�̓��e�Ńt�@�C���������[�h����
	if (ImGui::Button("RoundLoad"))
	{
		EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), s_currentItem, roundNum);
	}
	ImGui::End();
}
