#ifndef OBJDEFINE_H_
#define OBJDEFINE_H_

#include <SDL/SDL.h>

#include <string>
using std::string;

class ObjDefine {
	SDL_Rect card;
	SDL_Surface* cardSurface;
public:
	ObjDefine();
	ObjDefine(string, int, int, int, int);
	void ObjDraw(int, int, SDL_Surface*);
	void ObjTransp(int, int, int);
	virtual ~ObjDefine();
};

#endif
