#pragma once
#include "GameObject.h"
#include "GameComponent.h"
#include "GC_Button.h"
#include <vector>
#include <iostream>
using namespace std;
class ScreenController
{
protected:
	GameComponent *background = NULL;
public:
	~ScreenController() {
		delete[] background;
	}
	void addBackGround(const char *path) {
		background = new GameComponent();
		background->textures = GE::GE_LoadImage(path);
		background->rect = { 0,0, GE::WINDOW_WIDTH, GE::WINDOW_HEIGHT };

	}
	virtual void Render() {
		if (background != NULL) {
			this->background->Render();
		}
	}
	vector<GameObject*> listOnClickListener;
	void regClickEvent(GameObject* obj) {
		listOnClickListener.push_back(obj);
	}
};

