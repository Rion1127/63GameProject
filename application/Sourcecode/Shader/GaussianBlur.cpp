#include "GaussianBlur.h"
#include "Util.h"
#include "RRandom.h"
#include "SceneManager.h"

void GaussianBlur::PreDraw()
{
	//�����_�[�e�N�X�`���ւ̕`��
	PreDrawScene();
	SceneManager::Draw();
	PostDrawScene();
}

void RadialBlur::PreDraw()
{
	//�����_�[�e�N�X�`���ւ̕`��
	PreDrawScene();
	SceneManager::Draw();
	PostDrawScene();
}
