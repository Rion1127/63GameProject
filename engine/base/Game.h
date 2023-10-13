#pragma once
#include "Framework.h"

/**
 * @file Game.h
 * @brief 
 */

class Game final :
	public Framework
{
public:
	~Game();

	void Init() override;

	void Update()override;

	void Finalize()override;

	void Draw()override;

private:
};

