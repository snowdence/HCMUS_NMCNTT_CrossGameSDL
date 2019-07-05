#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "GELibrary.h"

class GameObject {
public:
	GE_Texture* textures; // text/img/... skin of this obj
	GE_Rect rect; // rect 
	virtual void Render() {
		GE::GE_RenderCopy(this->textures, &this->rect, false);
	}
	virtual void OnClick() {
		cout << "Onclick involked" << endl;
	}
protected:
	
};
#endif