#include "ObjDefine.h"

#ifndef SDL_H
#define SDL_H
#include "SDL/SDL.h"
#endif

ObjDefine::ObjDefine() {
}
ObjDefine::~ObjDefine() {
	SDL_FreeSurface(cardSurface);
}
ObjDefine::ObjDefine(string fileName, int aX, int aY, int aW, int aH){
	cardSurface = SDL_LoadBMP(fileName.c_str());
	card.x = aX;
	card.y = aY;
	card.w = aW;
	card.h = aH;
}
void ObjDefine::ObjDraw(int pX, int pY, SDL_Surface *destination)
{
	if(cardSurface != NULL){
		SDL_Rect tmpRect;
		tmpRect.x = pX;
		tmpRect.y = pY;
		tmpRect.w = card.w;
		tmpRect.h = card.h;
		SDL_BlitSurface(cardSurface, &card, destination, &tmpRect);
	}
}
void ObjDefine::ObjTransp(int red, int green, int blue){
	if (cardSurface != NULL){
		SDL_SetColorKey(cardSurface, SDL_SRCCOLORKEY, SDL_MapRGB(cardSurface->format, red, green, blue));
	}
}
