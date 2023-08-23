#pragma once
#include "Sprite.h"
#include "IGauge.h"
class PlayerHPGauge : 
	public IGauge
{
private:
	std::unique_ptr<Sprite> hpBarMiddle_;	//�_���[�W������������Ԃ��\������
public:
	PlayerHPGauge();
	
	void Damage();
private:
	void OriginalUpdate() override;
	void OriginalFrontDraw() override {};
	void OriginalMiddleDraw()override;
	void OriginalBackDraw()override {};
};

class PlayerMPGauge :
	public IGauge
{
public:
	PlayerMPGauge();

private:
	void OriginalUpdate() override;
	void OriginalFrontDraw() override;
	void OriginalMiddleDraw()override {};
	void OriginalBackDraw()override {};
};

