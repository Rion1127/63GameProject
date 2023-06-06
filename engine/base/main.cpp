#include <Windows.h>
#include "Game.h"
#include "Framework.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//Game* game = new Game;

	Framework* game = new Game;

	//更新
	game->Run();

	delete game;

	return 0;
}