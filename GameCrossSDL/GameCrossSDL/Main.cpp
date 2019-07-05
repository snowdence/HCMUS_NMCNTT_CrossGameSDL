#include <iostream>
#include "GELibrary.h"
#include "GC_Button.h"
#include "GameController.h"
#define GAME_WIDTH 1200
#define GAME_HEIGHT 800
#define GAME_FPS 60

const int delay = 1000 / GAME_FPS;

using namespace std;

GE_RGB window_rgb;
GE_Rect window_rect; 
EGameController state;

GameController game;

uint32_t startLoop, endLoop;


int main(int argc, char* args[])
{
	if (GE::GE_InitSDL()) {
		cout << "init ok" << endl;
	}
	window_rgb = { 255,255,255 };
	window_rect = { SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED , 1200 ,800 };
	GE::GE_CreateWindow("Game crossing Snowdence", window_rect, window_rgb, 0); 
	

	game.screen = new StartGameScreen();
	while (state != EGameController::QUIT) {
		//auto generated switch code
		/*startLoop = GE::GE_GetTimerTick();
		*/
		
		game.screen->Render();

		switch (state)
		{
		case EGameController::START:

			break;
		case EGameController::PLAY:
			break;
		case EGameController::PAUSE:
			break;
		case EGameController::CHOOSE_SKIN:
			break;
		case EGameController::SETTING:
			break;
		case EGameController::GAME_OVER:
			break;
		case EGameController::LEVEL_UP:
			break;
		case EGameController::QUIT:
			break;
		default:
			break;
		}

		if (SDL_PollEvent(&GE::event)) {
			if (SDL_QUIT == GE::event.type) {
				state = EGameController::QUIT;
				break;
			}
			else if (GE::event.type == GE_MOUSEBUTTONDOWN) {
				
				cout << "Button LEFT" << endl;
				game.onClickEventListener();
			}
		}
		
/*
		endLoop = GE::GE_GetTimerTick() - startLoop;
		if (endLoop < delay) {
			GE::G_Delay(delay - endLoop);
		}*/
		
		GE::GE_Update();
	}
	GE::GE_Destroy();
	return 0; 
}