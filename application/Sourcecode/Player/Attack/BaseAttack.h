#pragma once
#include "AttackEditor.h"
#include "Spline.h"
#include <vector>

class BaseAttack
{
public:
	struct AttackInput {
		AttackType type_;		//’ÊíUŒ‚‚©ƒtƒBƒjƒbƒVƒ…‹Z‚©
		std::vector<AttackEditor::AttackInfo> attackinfo;	//UŒ‚‚ÌÚ×
	};
private:
	float DamageCoolTime_;		//–³“GŠÔ
	AttackInput attackinput_;	//UŒ‚‚ÌÚ×
	Spline spline_;
public:
	BaseAttack(const AttackInput& input);

	void Update();

	void Draw();
};

