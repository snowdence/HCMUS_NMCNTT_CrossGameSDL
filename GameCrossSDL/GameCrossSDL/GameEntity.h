#pragma once
#include "GameObject.h"
enum ObjectType { PLAYER, CAR, TRAIN, STICK, LAMP, EAGLE, COIN };

class GameEntity :
	public GameObject
{
protected:
	
public:
	virtual void HandleOutBorder() {
	
	};
	virtual void HandleMove() {
		this->rect.x += moveSpeed;
	}
	virtual void RandomProperty() {
		
	}
	SDL_Point point;
	GE_Sound* sound = NULL;
	bool isMoving = false;
	ObjectType type;
	Direction direction;  //for rendering ... animation
	union { //All of Objects Need MoveSpeed Except Train. The Speed of Train Should Be Constant. Instead of Speed , it has a Timer to show when it should come!!!
		int moveSpeed;
		int timer;
	};
};

