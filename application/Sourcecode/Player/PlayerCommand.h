#pragma once
#include "Sprite.h"
#include "AttackManager.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include "Timer.h"
#include "MagicManager.h"

struct CommandSprite
{
private:
	std::unique_ptr<Sprite> commandTitle_;
	std::vector<std::unique_ptr<Sprite>> frame_;
	std::vector<std::unique_ptr<Sprite>> commandTex_;

	std::vector<Vector2> basePos_;
	std::vector<Color> baseColor_;

	Timer easeTimer_;
	Timer magicMenuEaseTimer_;

	uint16_t currentNum_;

	bool isVisible_;
	bool isTranslucent_;//半透明フラグ
public:
	CommandSprite(Color color,Vector2 basePos,uint32_t menuNum);
	void SpriteUpdate();

	void DrawSprite();

	void ResetEase();
public:
	void SetTitleTex(Texture* texture) { commandTitle_->SetTexture(texture); };
	void SetFrameTex(Texture* texture);	//フレームテクスチャ
	void SetCharaTex(Texture* texture);	//文字テクスチャ
	void SetCurrentNum(uint16_t num) { currentNum_ = num; }
	void SetIsVisible(bool flag) { isVisible_ = flag; }
	void SetTranslucent(bool flag) { isTranslucent_ = flag; }
};

struct Player;

class PlayerCommand
{
private:
	enum class Command {
		Attack,
		Magic,
		END,
	};
private:
	std::unique_ptr<CommandSprite> mainCommandSprite_;
	std::unique_ptr<CommandSprite> magicCommandSprite_;
private:
	IEnemy* lockOnEnemy_;
	Player* player_;
	
	AttackManager attackManager_;
	MagicManager magicManager_;

	Command selectCommand_;
	int16_t commandNum_;

	bool isMagicMenu_;
	MagicType magicType_;
	int16_t magicNum_;
public:
	PlayerCommand();

	void Update();

	void Draw();
	void DrawSprite();

public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
	void SetPlayer(Player* player) {
		player_ = player;
		magicManager_.SetPlayer(player);
	}
public:
	AttackManager* GetAttackManager() { return &attackManager_; }
	MagicManager* GetMagicManager() { return &magicManager_; }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
	bool GetIsMagicMenu() { return isMagicMenu_; }

};

