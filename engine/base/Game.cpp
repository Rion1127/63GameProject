#include "Game.h"

Game::~Game()
{
}

void Game::Init()
{
	Framework::Init();

	//�Q�[���V�[��������
	SceneManager::Ini();
}

void Game::Update()
{
	Framework::Update();
	//�Q�[���V�[���X�V
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

