#include <iostream>
#include "GELibrary.h"
#include "GC_Button.h"
#include "GameController.h"
#include "PlayGameScreen.h"
#define GAME_FPS 60

const int delay = 1000 / GAME_FPS;

using namespace std;

GE_RGB window_rgb;
GE_Rect window_rect; 


GameController game;

uint32_t startLoop, endLoop;


int main(int argc, char* args[])
{
	if (GE::GE_InitSDL()) {
		cout << "init ok" << endl;
	}
	window_rgb = { 255,255,255 };
	GE::WINDOW_WIDTH = 1200;
	GE::WINDOW_HEIGHT = 640;
	window_rect = { SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED , GE::WINDOW_WIDTH ,GE::WINDOW_HEIGHT };
	GE::GE_CreateWindow("Game crossing Snowdence", window_rect, window_rgb, 0); 
	
	game.init();
	while (game.state != EGameController::QUIT) {
		//auto generated switch code
		startLoop = GE::GE_GetTimerTick();
		game.Update();

		switch (game.state)
		{
		case EGameController::START:
			if(game.screen == NULL){
				game.screen = new PlayGameScreen(&game.state, game.player);
			}
			break;
		case EGameController::PLAY:
			cout << "Game Playing" << endl;
			break;
		case EGameController::PAUSE:
			cout << "Game pause" << endl; 
			break;
		case EGameController::CHOOSE_SKIN:
			cout << "Game choose skin" << endl; 
			break;
		case EGameController::SETTING:
			cout << "Game setting" << endl; 
			break;
		case EGameController::GAME_OVER:
			cout << "GAme over" << endl; 
			break;
		case EGameController::LEVEL_UP:
			cout << "Game level_up" << endl;
			break;
		case EGameController::QUIT:
			cout << "GAme quit" << endl;
			break;
		default:
			break;
		}
		game.Render();
		game.onPlayerListener();
		if (SDL_PollEvent(&GE::event)) {
			if (SDL_QUIT == GE::event.type) {
				game.state = EGameController::QUIT;
				break;
			}
			else if (GE::event.type == GE_MOUSEBUTTONDOWN) {
				
				cout << "Button LEFT x="<< GE_Motion.x << "y=" << GE_Motion.y << endl;
				
				game.onClickEventListener();
			}
		}
		

		endLoop = GE::GE_GetTimerTick() - startLoop;
		if (endLoop < delay) {
			GE::G_Delay(delay - endLoop);
		}
		
		GE::GE_Update();
	}
	GE::GE_Destroy();
	return 0; 
}