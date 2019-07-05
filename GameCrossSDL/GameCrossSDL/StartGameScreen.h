#pragma once
#include "ScreenController.h"
#include "GC_Button.h"
class StartGameScreen : public ScreenController
{
protected:
	GC_Button setting;
public: 
	StartGameScreen() {

		setting.rect = { 20,  800 - 100 , 125,100 };
		setting.textures = GE::GE_LoadImage("assets/image/settings.png");
		setting.Render();
		ScreenController::regClickEvent(&setting);
	}
	void Render() {
		setting.Render();
	}
	~StartGameScreen() {
		
	}
};

