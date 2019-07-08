#pragma once

#include "GELibrary.h" 
#include "GameObject.h"
#include <vector>
#include "ScreenController.h"
#include "StartGameScreen.h"
#include "PlayerEntity.h"
class GameController
{
protected:
	

public:
	PlayerEntity* player;

	EGameController state = EGameController::START;
	ScreenController* screen = NULL;
	GameController() {
		 
	}
	~GameController() {
		delete[] screen;
	}
	void init() {
		player = new PlayerEntity();
	}
	void Update() {
		if(screen != NULL){ 
		screen->update();
		}
	}
	/*
	switch (Player->direction)
		{
		case UP:
			if (Player->rect.y <= map[Player->point.x][Player->point.y - 1].position.y) {
				Player->isMoving = false; 
				Player->point.y--;
				Player->rect.y = map[Player->point.x][Player->point.y].position.y;
			}
			break;
		case DOWN:
			if (Player->rect.x >= map[Player->point.x + 1][Player->point.y].position.x) {
				Player->isMoving = false;
				Player->point.x++;
				Player->rect.x = map[Player->point.x][Player->point.y].position.x;
			}
			break;
		case LEFT:
			break;
		case RIGHT:
			break;

		}*/
	
	void processPlayerListener() {
	
		if (this->player->isMoving) {
			switch (this->player->direction)
			{
			case UP:
				player->rect.y -= player->playerMoveSpeed;
				player->isMoving = false;
				break;
			case DOWN:
				player->isMoving = false;
				player->point.y++;
				player->rect.y += player->dy;
				break;
			case LEFT:
				player->isMoving = false;
				player->point.x--;
				player->rect.x -= player->dx;
				break;
			case RIGHT:
				player->isMoving = false;
				player->point.x++;
				player->rect.x += player->dx;
				break;
			}

		}
	}
	

	void onPlayerListener() {
		processPlayerListener();
		if ( GE::event.type == GE_KEYDOWN && !this->player->isMoving) {
			switch (GE_Keyboard)
			{
			case GEK_UP:
				this->player->isMoving = true;
				this->player->direction = UP;
				break;
			case GEK_DOWN:
				this->player->isMoving = true;
				this->player->direction = DOWN;
				break;
			case GEK_RIGHT:
				this->player->isMoving = true;
				this->player->direction = RIGHT;
				break;
			case GEK_LEFT:
				this->player->isMoving = true;
				this->player->direction = LEFT;
				break;
			default:
				break;
			}
		}
	}
	void Render() {
		this->screen->Render();
		//this->player->Render();
	}
	void onClickEventListener() {
		for (int i = 0; i < screen->listOnClickListener.size(); i++)
		{
			if (clickOnButton(&screen->listOnClickListener[i]->rect) == true) {
				screen->listOnClickListener[i]->OnClick();
			}
		}
	}
protected:
	
	bool clickOnButton(GE_Rect* r) {
		SDL_Point mouse_point = { GE_Motion.x , GE_Motion.y };
		int cur_event = GE::getCurrentEvent();
		if (SDL_PointInRect(&mouse_point, r)) {
			cout << "Click event on [" << mouse_point.x << ":" << mouse_point.y << "]" << endl;
			return true;
		}
		return false;
	}
};

