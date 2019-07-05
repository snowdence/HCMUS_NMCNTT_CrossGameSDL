#pragma once

#include "GELibrary.h" 
#include "GameObject.h"
#include <vector>
#include "ScreenController.h"
#include "StartGameScreen.h"
class GameController
{
public:
	ScreenController* screen = NULL;
	GameController() {

	}
	~GameController() {
		delete[] screen;
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
			cout << "Click event on this " << mouse_point.x << ":" << mouse_point.y << endl;
			return true;
		}
		return false;
	}
};

