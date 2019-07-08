#pragma once
#include "GameComponent.h"
class GC_Button : public GameComponent
{
private:
public:
	GC_Button() {
		this->rect = { 0, 0, 0, 0 }; 
		this->texture = NULL;
	}
	GC_Button(GE_Rect _rect, const char * img_path) {
		this->rect = _rect;
		this->texture = GE::GE_LoadImage(img_path);
	}
	
	~GC_Button() {

	}
};

