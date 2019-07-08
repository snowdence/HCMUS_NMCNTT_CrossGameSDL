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
	EGameController* state;
public:
	ScreenController() {

	}
	ScreenController(EGameController *_state ) {
		state = _state;
	}
	~ScreenController() {
		if (background != nullptr) {
			delete[] background;
		}
	}
	virtual void update() {

	}
	void addBackGround(const char *path) {
		background = new GameComponent();
		background->texture = GE::GE_LoadImage(path);
		background->rect = { 0,0, GE::WINDOW_WIDTH, GE::WINDOW_HEIGHT };

	}
	virtual void Render() {
		if (background != nullptr) {
			this->background->Render();
		}
	}
	vector<GameObject*> listOnClickListener;
	void regClickEvent(GameObject* obj) {
		listOnClickListener.push_back(obj);
	}
};

