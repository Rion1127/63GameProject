#include "Game.h"

Game::~Game()
{
}

void Game::Init()
{
	Framework::Init();

	//ゲームシーン初期化
	SceneManager::Ini();
}

void Game::Update()
{
	Framework::Update();
	//ゲームシーン更新
	SceneManager::Update();
}

void Game::Draw()
{
	Framework::Draw();
}

void Game::Finalize()
{
	Framework::Finalize();
}

