#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"


void SceneManager::Ini()
{
	//ビルド開始時のシーン
	Transition<GameScene>();
	

	sceneChangeGraph = TextureManager::GetInstance()->LoadGraph("white1×1.png");
}

void SceneManager::Update()
{
	currentScene->Update();

	
}

void SceneManager::Draw()
{
	//ゲーム描画
	currentScene->Draw();
}

std::unique_ptr<EmptyScene> SceneManager::currentScene = nullptr;

Sprite SceneManager::sceneChangeSprite;
Vector2  SceneManager::sceneChangePos;
uint32_t SceneManager::sceneChangeGraph;
int SceneManager::changeStartCount_;
int SceneManager::sceneNum_;
bool SceneManager::isNext_;
int SceneManager::continueScene = SceneNum::STAGE1_;
bool SceneManager::isDifferent = true;
int SceneManager::different = 0;