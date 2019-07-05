#pragma once
#include "ScreenController.h"
#include "GC_Button.h"
class StartGameScreen : public ScreenController
{
protected:
	GC_Button setting;
	GameComponent background;
public: 
	StartGameScreen() {

		setting.rect = { 20, GE::WINDOW_HEIGHT - 100 , 125,100 };
		setting.textures = GE::GE_LoadImage("assets/image/settings.png");
		setting.Render();
		addBackGround("assets/bg.png");
		ScreenController::regClickEvent(&setting);
	}

	void Render() {
		ScreenController::Render();
		setting.Render();
		
	}
	~StartGameScreen() {
		
	}
};

