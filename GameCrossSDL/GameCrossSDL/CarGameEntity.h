#pragma once
#include "GameEntity.h"
class CarGameEntity : public GameEntity
{

public:
	GE_Texture* skin1, * skin2;
	CarGameEntity(const CarGameEntity& e) = delete;
	CarGameEntity() {
		texture = GE::GE_LoadImage("assets/image/car.png");
		skin1 = GE::GE_LoadImage("assets/image/car2.png");
		skin2 = GE::GE_LoadImage("assets/image/car3.png");

		rect = { 0,0,169,100 };
		isMoving = true;
		type = CAR;
	}
	void RandomSkin() {
		short int tex = rand() % 3;
		switch (tex)
		{
		case 1:
			texture = skin1;
			break;
		case 2:
			texture = skin2;
			break;
		}
	}
	
	void HandleOutBorder()   {
		if (direction == LEFT && rect.x < -rect.w) {
			rect.x = GE::WINDOW_WIDTH;
		}
		else if (direction == RIGHT && rect.x > GE::WINDOW_WIDTH) {
			rect.x = -rect.w;
		}
		
	}

	
	~CarGameEntity() {
		delete[] skin1;
		delete[] skin2;
	}
protected:
};

