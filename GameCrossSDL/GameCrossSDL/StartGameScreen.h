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

		setting.rect = { 20, 640 - 100 , 125,100 };
		setting.textures = GE::GE_LoadImage("assets/image/settings.png");
		setting.Render();
		Background();
		ScreenController::regClickEvent(&setting);
	}
	void Background() {
		background.rect = { 0,0, 1200, 640 };
		background.textures = GE::GE_LoadImage("assets/bg.png");
		background.Render();
	}
	void Render() {
		background.Render();
		setting.Render();
		
	}
	~StartGameScreen() {
		
	}
};

