#include "GaussianBlur.h"
#include "Util.h"
#include "RRandom.h"
#include "SceneManager.h"

void GaussianBlur::PreDraw()
{
	//レンダーテクスチャへの描画
	PreDrawScene();
	SceneManager::Draw();
	PostDrawScene();
}

void RadialBlur::PreDraw()
{
	//レンダーテクスチャへの描画
	PreDrawScene();
	SceneManager::Draw();
	PostDrawScene();
}
