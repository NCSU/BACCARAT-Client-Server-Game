#ifndef TEXTDEFINE_H_
#define TEXTDEFINE_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <string>
using namespace std;

class TextDefine {
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* textSurface;
	SDL_Rect text;
public:
	TextDefine();
	TextDefine(string, int, int, int, int, string);
	void textDraw(int, int, SDL_Surface*);
	virtual ~TextDefine();
};

#endif
