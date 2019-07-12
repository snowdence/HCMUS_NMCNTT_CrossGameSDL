#pragma once
#include "ScreenController.h"
#include "GELibrary.h"
using namespace std;
class GamePauseScreen : public ScreenController
{

public:

	GE_Font* font;
	GameComponent pause;
	GamePauseScreen() {
		font = GE::GE_OpenFont("assets/font/editundo.ttf", 72);
		pause.texture = GE::GE_LoadFont(font, "PAUSE", 0, 0, 06);
		pause.rect = { 300,100 ,500 , 200 };

	}

	void Render() override {
		pause.Render();
	}

protected:

	// Inherited via ScreenController
	virtual void Update() override;
};

