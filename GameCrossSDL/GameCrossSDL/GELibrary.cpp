#include "GELibrary.h"
#include <stdint.h>



namespace GE {
	bool GE_InitSDL() {
		if ((SDL_Init(SDL_INIT_EVERYTHING) == 0) && (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0) && (TTF_Init() == 0))
		{
			return true;
		}
#ifdef GE_DEBUG
		cout << "Init has a problem" << endl;
#endif // GE_DEBUG
		return false;
	}

	bool GE_CreateWindow(const char * title, GE_Rect rect, GE_RGB rgb, int flag = 0) {
		window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, flag);
		if(window != 0)
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) {
				SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, 255); // transparent for 255 
				return true;
			}
		}
#ifdef GE_DEBUG
		cout << "Created Window error" << endl;
#endif
		return false;
	}
	void GE_Update() {
		//update by renderer
		SDL_RenderPresent(GE::renderer);
		SDL_RenderClear(GE::renderer);
	}
	void GE_Destroy() {
#ifdef GE_DEBUG
		cout << "Destroy SDL" << endl;
#endif
		SDL_DestroyRenderer(GE::renderer);
		SDL_DestroyWindow(GE::window);
		SDL_Quit();
	}
	uint32_t GE_GetTimerTick() {
		return SDL_GetTicks();
	}
	void G_Delay(uint32_t s)
	{
		//delay for second
		SDL_Delay(s);
	}
	
	SDL_Texture* GE_LoadImage(const char* file) {
		SDL_Surface* objSurface = IMG_Load(file); 
		if (objSurface != NULL) {
			SDL_Texture* texture = SDL_CreateTextureFromSurface(GE::renderer, objSurface);
			SDL_FreeSurface(objSurface);
			if (texture != NULL) {
				return texture;
			}
		}
#ifdef GE_DEBUG
		cout << "GE_LoadImg failed : " << file << endl;
#endif // GE_DEBUG

		return nullptr;
	}
	void GE_RenderCopy(GE_Texture* texture, GE_Rect* src, GE_Rect* dst, bool fullscreen)
	{
		if (!fullscreen)
			SDL_RenderCopy(renderer, texture, src, dst);
		else
			SDL_RenderCopy(renderer, texture, src, 0);
	}
	void GE_RenderCopy(GE_Texture* texture, GE_Rect* dst, bool fullscreen )
	{
		if (!fullscreen)
			SDL_RenderCopy(renderer, texture, 0, dst);
		else
			SDL_RenderCopy(renderer, texture, 0, 0);
	}
	void GE_RenderCopyEx(GE_Texture* texture, GE_Rect* src, GE_Rect* dst, SDL_RendererFlip flip, bool fullscreen )
	{
		if (!fullscreen)
			SDL_RenderCopyEx(renderer, texture, src, dst, 0, 0, flip);
		else
			SDL_RenderCopyEx(renderer, texture, src, 0, 0, 0, flip);
	}
	void GE_RenderCopyEx(GE_Texture* texture, GE_Rect* dst, SDL_RendererFlip flip, bool fullscreen )
	{
		if (!fullscreen)
			SDL_RenderCopyEx(renderer, texture, 0, dst, 0, 0, flip);
		else
			SDL_RenderCopyEx(renderer, texture, 0, 0, 0, 0, flip);
	}



	int getCurrentEvent() {
		if (SDL_PollEvent(&event) != 0)
		{
			return event.type;
		}
		return 0;
	}
	SDL_Point getMousePosition() {
		SDL_Point mouse_point = { GE_Motion.x , GE_Motion.y };
		return mouse_point;
	}


	//font
	TTF_Font* GE_OpenFont(const char* file, int fontSize)
	{
		return TTF_OpenFont(file, fontSize);
	}
	SDL_Texture* GE_LoadFont(GE_Font* font, const  char* title, Uint8 r, Uint8 g, Uint8 b)
	{
		SDL_Color color;
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = 255;
		SDL_Surface* surface = TTF_RenderText_Solid(font, title, color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}

}