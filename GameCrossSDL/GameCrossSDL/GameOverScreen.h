#pragma once
#include "ScreenController.h"
#include "GameEntity.h"

class GameOverScreen :
	public ScreenController
{
public:
	GE_Font* font;
	GameEntity text;
	GameOverScreen() {
		font = GE::GE_OpenFont("assets/font/editundo.ttf", 72);
		text.texture = GE::GE_LoadFont(font, "GAME OVER", 0, 0, 06);
		text.rect = { 300,100 ,500 , 200 };
		
	}
	void Render() {
		GE::GE_RenderCopy(text.texture, &text.rect);


	}
protected:
};

