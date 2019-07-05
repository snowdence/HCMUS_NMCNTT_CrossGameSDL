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
	
public:
	virtual void Render() {
		
	}
	vector<GameObject*> listOnClickListener;
	void regClickEvent(GameObject* obj) {
		listOnClickListener.push_back(obj);
	}
};

