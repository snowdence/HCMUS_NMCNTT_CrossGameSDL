#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "GELibrary.h"

class GameObject {
public:

	GE_Texture* texture; // text/img/... skin of this obj
	GE_Rect rect; // rect 
	GameObject() {

	}
	
	
	virtual void Render() {
		GE::GE_RenderCopy(this->texture, &this->rect, false);
	}
	virtual void OnClick() {
		cout << "Onclick involked" << endl;
	}
protected:
	
};
#endif