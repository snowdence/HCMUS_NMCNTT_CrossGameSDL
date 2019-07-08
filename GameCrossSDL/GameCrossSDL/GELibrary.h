#ifndef GELibrary_H
#define GELibrary_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#define GE_DEBUG

#define GE_Motion GE::event.motion
#define GE_BUTTON_LEFT     1
#define GE_BUTTON_MIDDLE   2
#define GE_BUTTON_RIGHT    3
#define GE_BUTTON_X1       4
#define GE_BUTTON_X2       5

#define GE_Keyboard GE::event.key.keysym.sym
#define GE_Mouse GE::event.button.button
using namespace std;
/*
* Author 		: snowdence
* Email	 		: 18127027@student.hcmus.edu.vn
* Work			: Student - Ho Chi Minh University of Science (HCMUS)
* File	 		: GELibrary
* Description	: Feature in project
* Created	 	: 04/07/2019
* Reference		: SDL ver 2
* License		: MIT License
*/

typedef SDL_Rect GE_Rect; // rewrite type 
typedef SDL_Texture GE_Texture; //rewrite Texture
typedef TTF_Font GE_Font;
typedef Mix_Music GE_Music;
typedef Mix_Chunk GE_Sound;
typedef struct GE_RGB {
	//default white
	uint8_t r = 255;
	uint8_t g = 255;
	uint8_t b = 255;
};

enum EKeyboardMotion{
	GE_KEYDOWN = 0x300, /**< Key pressed */
	GE_KEYUP,                  /**< Key released */
	GE_TEXTEDITING,            /**< Keyboard text editing (composition) */
	GE_TEXTINPUT,              /**< Keyboard text input */
};
enum Direction { UP, DOWN, LEFT, RIGHT };

enum class EGameController { START, PLAY, PAUSE, CHOOSE_SKIN, SETTING, GAME_OVER, LEVEL_UP, QUIT  , OUT};

enum {
	GEK_RIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
	GEK_LEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
	GEK_DOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
	GEK_UP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),
};
typedef enum EMouseMotion {
	GE_MOUSEMOTION = 0x400, /**< Mouse moved */
	GE_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
	GE_MOUSEBUTTONUP,          /**< Mouse button released */
	GE_MOUSEWHEEL,             /**< Mouse wheel motion */
};
namespace GE {
	static int WINDOW_WIDTH = 1200;
	static int WINDOW_HEIGHT = 800;
	static SDL_Window* window = NULL;  //Window form
	static SDL_Renderer* renderer = NULL; // Surface 
	static SDL_Event event; //Eventd SDL 
	bool GE_InitSDL();

	bool GE_CreateWindow(const char* title, GE_Rect rect, GE_RGB rgb, int flag);

	void GE_Update();

	void GE_Destroy();

	uint32_t GE_GetTimerTick();

	void G_Delay(uint32_t s);

	SDL_Texture* GE_LoadImage(const char* file);

	void GE_RenderCopy(GE_Texture* texture, GE_Rect* src, GE_Rect* dst, bool fullscreen = false);

	void GE_RenderCopy(GE_Texture* texture, GE_Rect* dst, bool fullscreen = false);

	void GE_RenderCopyEx(GE_Texture* texture, GE_Rect* src, GE_Rect* dst, SDL_RendererFlip flip, bool fullscreen = false);

	void GE_RenderCopyEx(GE_Texture* texture, GE_Rect* dst, SDL_RendererFlip flip, bool fullscreen = false);


	
	int getCurrentEvent();

	SDL_Point getMousePosition();

	

}
#endif
