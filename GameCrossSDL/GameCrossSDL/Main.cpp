#include <iostream>
#include "GELibrary.h"
#include "GC_Button.h"
#include "GameController.h"
#include "PlayGameScreen.h"
#include <ctime>
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
	
	window_rect = { SDL_WINDOWPOS_UNDEFINED , SDL_WINDOWPOS_UNDEFINED , GE::WINDOW_WIDTH ,GE::WINDOW_HEIGHT };
	GE::GE_CreateWindow("Game crossing - Group 4 CLC1 - k18", window_rect, window_rgb, 0); 
	
	game.initGame();

	while (game.isQuit() == false) {
		startLoop = GE::GE_GetTimerTick();

		game.handleScreen();
		game.Update();
		game.onHandleMove();
		game.onHandleEvent();
		game.Render();
	
		endLoop = GE::GE_GetTimerTick() - startLoop;
		if (endLoop < delay) {
			GE::G_Delay(delay - endLoop);
		}

		GE::GE_Update();
	}
	GE::GE_Destroy();
	return 0; 
}