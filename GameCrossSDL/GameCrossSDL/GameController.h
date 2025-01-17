#pragma once
#include "GameOverScreen.h"
#include "GELibrary.h" 
#include "GameObject.h"
#include <vector>
#include "ScreenController.h"
#include "StartGameScreen.h"
#include "PlayGameScreen.h"
#include "PlayerEntity.h"
class GameController
{
protected:

public:
	PlayerEntity* player;

	EGameController state = EGameController::START;
	ScreenController* screen = nullptr;
	GE_Font* font;
	GameController() {
		//init();
	}
	~GameController() {
		
		delete[] screen;
	}
	void initGame() {
		state = EGameController::PLAY;
		player = new PlayerEntity();
	}
	void playGame() {
		//cout << "Play Game" << endl;
		if(screen == nullptr){
			screen = new PlayGameScreen(player,&state);
		}
	}
	void gameOVer() {
		if (screen != nullptr) {
			screen = new GameOverScreen();
		}
	}
	void Update() {
		if(screen!= nullptr){
			screen->update();
		}
	}
	void RenderScreen() {
		screen->Render();
		player->Render();
	}
	void onHandleMove() {
		screen->handleMove();
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