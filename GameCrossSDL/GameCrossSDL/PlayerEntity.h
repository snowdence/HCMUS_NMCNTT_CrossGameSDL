#pragma once
#include "GameEntity.h"
class PlayerEntity :
	public GameEntity
{
private:
	uint32_t level = 0;
	uint32_t coin = 0;
public:
	PlayerEntity() {
		this->rect = { 10,10, 100,100 };
		this->textures = GE::GE_LoadImage("assets/image/player.png");
	}
	int playerMoveSpeed = 10;
	int dx = 10;
	int dy = 10;
};

