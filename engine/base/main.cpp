#include <Windows.h>
#include "Game.h"
#include "Framework.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<Framework> game = std::make_unique<Game>();

	//更新
	game->Run();

	return 0;
}