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
	ScreenController* screen;

	EGameController state = EGameController::PLAY;
	EGameController _prestate = state;
	PlayerEntity* player;

	bool quit = false;
public:
	bool fChangeState = false;
	void initGame() {
		player = new PlayerEntity();
		screen = new PlayGameScreen(player, &state);
	}
	EGameController getState() {
		return state;
	}

	bool isQuit() {
		return quit;
	}

	void handleScreen()
	{
		if (_prestate == state) {
		}
		else {
			switch (state)
			{
			case EGameController::PLAY:
				screen = new PlayGameScreen(player, &state);
				//game.playGame();
				//game.onHandleMove();
				break;
			case EGameController::GAME_OVER:
				screen = new GameOverScreen();
#ifdef GE_DEBUG
				cout << "DEBUG GAME_OVEr" << endl;
#endif
				break;
			default:
				break;
			}
			_prestate = state;
		}
	}


	void onHandleEvent() {
		if (SDL_PollEvent(&GE::event)) {
			if (SDL_QUIT == GE::event.type) {
				cout << "quit" << endl;
				quit = true;
			}
			else if (GE::event.type == GE_MOUSEBUTTONDOWN) {

				cout << "Button LEFT x=" << GE_Motion.x << "y=" << GE_Motion.y << endl;
				onClickEventListener();
			}
		}
	}
	void onHandleMove() {
		screen->handleMove();
	}

	void Update() {
		if (screen != nullptr) {
			screen->Update();
		}
	}
	void Render() {
		if (screen != nullptr) {
			screen->Render();
		}
	}
	void onClickEventListener() {
		/*for (int i = 0; i < screen->listOnClickListener.size(); i++)
		{
			if (clickOnButton(&screen->listOnClickListener[i]->rect) == true) {
				screen->listOnClickListener[i]->OnClick();
			}
		}*/
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