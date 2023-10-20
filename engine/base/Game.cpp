#include "Game.h"

/**
 * @file Game.cpp
 * @brief ゲーム全体の処理をまとめたクラス
 */

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

