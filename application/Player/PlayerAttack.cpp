#include <cmath>
#include "Util.h"
#include "PlayerAttack.h"

void PlayerAttack::Init()
{
	controler_ = Controller::GetInstance();
	model_ = std::move(std::make_unique<Object3d>());
	model_->SetModel(Model::CreateOBJ_uniptr("cube", false));
}