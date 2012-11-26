#include "TextDefine.h"

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include <string>
using std::string;

TextDefine::TextDefine() {
}
TextDefine::~TextDefine() {
}
TextDefine::TextDefine(string tFont, int tFontSize, int cRed, int cGreen, int cBlue, string caption){
	font = TTF_OpenFont(tFont.c_str(), tFontSize);
	color.r = cRed;
	color.g = cGreen;
	color.b = cBlue;
	textSurface = TTF_RenderText_Solid(font, caption.c_str(), color);
}
void TextDefine::textDraw(int x, int y, SDL_Surface* scr){
	text.x = x;
	text.y = y;
	SDL_BlitSurface(textSurface, NULL, scr, &text);
}
