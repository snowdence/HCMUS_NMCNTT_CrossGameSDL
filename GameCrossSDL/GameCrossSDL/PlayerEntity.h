#pragma once
#include "GameEntity.h"
class PlayerEntity :
	public GameEntity
{
private:
	
public:
	PlayerEntity() {
		this->rect = { 10,10, 100,100 };
		this->textures = GE::GE_LoadImage("assets/image/player.png");
	}

	int dx = 10;
	int dy = 10;
};

