#pragma once
#include "GameObject.h"

class GameEntity :
	public GameObject
{
protected:
public:
	SDL_Point point;
	GE_Sound* sound = NULL;
	bool isMoving = false;
	Direction direction;  //for rendering ... animation
};

