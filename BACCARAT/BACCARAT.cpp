#ifndef SDL_H
#define SDL_H
#include <SDL/SDL.h>
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_net.h"
#endif

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <string>
using namespace std;

#include "ObjDefine.h"
#include "Rules.h"
#include "TextDefine.h"
#include "Player.h"
#include "Network.h"

#define W_WIDTH 640
#define W_HEIGTH 480
#define cSizeX 73 //Card from file width
#define cSizeY 98 //Card from file height
const char* title = "Baccarat";
const string serverIp ="localhost";

string NumToText(double); //Converts number to string
void CardsMod(int&, int&); //Gets cards quantity by rules (%10)
int MouseImg(Player&, int, int, int); //Sets mouse image by in-game choice - pointer or chip
void Zone(Player&, int, int, int&, int&, int&, int, int, int, int); //Check the bet and put chips on the table
void DrawChips(int, int, int, int, int, int, int, int, int, SDL_Surface*); //Draw chips on the table
bool GetCard(TCPsocket, int*, int*, int*, int*, int, int&); //Get card from the server

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Surface *screen = 0;
	screen = SDL_SetVideoMode(W_WIDTH, W_HEIGTH, 0, SDL_SWSURFACE);
	SDL_WM_SetCaption(title, 0);
	SDL_ShowCursor(SDL_DISABLE);

	//Define mouse image and text font
	string chipImg = "Chips.bmp";
	string font = "C:/WINDOWS/Fonts/TREBUCIT.TTF";

	//Define text for winner and background
	TextDefine win(font, 25, 255, 255, 255, "Win");
	TextDefine tie(font, 25, 255, 255, 255, "Tie");
	ObjDefine background("Background.bmp", 0, 0, 640, 480);

	if(SDLNet_Init() == -1) {
	    printf("SDLNet_Init: %s\n", SDLNet_GetError());
	    exit(2);
	}

	TCPsocket socket = ConnectToServer(serverIp);
	// If this fails, then the server is not started
	if ( !Send("hi", 3, socket) ){
		return 1;
	}

	srand(time(0));
	//Define main class objects
	Rules rlz;
	//Player player("Player", 1000, 0);
	Player player;
	double cash, bet;

	// If this fails,there is error with the server
	if ( !Recieve(&cash, sizeof(cash), socket) || !Recieve(&bet, sizeof(bet), socket) ){
		return 1;
	}

	player.setCash(cash);
	player.setBet(bet);

	//Timing for cards show on screen
	int CardDrawTimer = 0;
	//Card number (2 - Ace)
	int face = 0;

	//3 cards for player, 3 for banker define random
	int cNumX[6] = {0, 0, 0, 0, 0, 0};
	int cNumY[6] = {0, 0, 0, 0, 0, 0};

	//Card's places on screen
	int cOnScrX[6] = {20, 46, 650, 259, 285, 650};
	const int cOnScrY = 20;

	//Card coordinates in file (.bmp)
	int cFilePosX[13];
	for (int i = 0; i < 950; i += 73){
		cFilePosX[i/73] = i;
	}
	int cFilePosY[4];
	for (int i = 0; i < 392; i += 98){
		cFilePosY[i/98] = i;
	}

	//Chip image for tie coordinates
	int chipTieX = 0;
	int chipTieY = 0;
	//Chip image for bank coordinates
	int chipBankX = 0;
	int chipBankY = 0;
	//Chip image for player coordinates
	int chipPlayerX = 0;
	int chipPlayerY = 0;

	//Count of player cards, banker cards and result (1-3) after checking rules
	int playerCards = 0;
	int bankCards = 0;
	int result = 0;

	//Count of the current bet in the current zone
	int tieZone = 0;
	int bankerZone = 0;
	int playerZone = 0;

	//Mouse pointer image
	int chipImgX = 149;
	const int chipImgW = 36;
	const int chipImgH = 36;
	//Checks game's current state (0-2)
	int inARow = 0;

	//Game running
	SDL_Event event;
	while(true){
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				break;
			}
		}
		//Mouse current position
		int x = event.motion.x;
		int y = event.motion.y;
		//Mouse image (source file(.bmp), xPos, yPos, width, height)
		ObjDefine mousePointer(chipImg, chipImgX, 0, chipImgW, chipImgH);
		//set transparent color (red, green, blue)
		mousePointer.ObjTransp(255, 0, 255);

		//Define text to show (font, size, red, green, blue, "text")
		TextDefine chipTxt(font, 13, 255, 255, 255, "0");
		TextDefine cashTxt(font, 13, 255, 255, 255, NumToText(player.getCash()));

		//Gets current bet state
		int betQ = tieZone + playerZone + bankerZone;
		TextDefine betTxt(font, 13, 255, 255, 255, NumToText(betQ));

		//Game's first state (check bets)
		if (inARow == 0){
			//Changes mouse picture depends of choice
			if (event.button.type == SDL_MOUSEBUTTONDOWN){
				chipImgX = MouseImg(player, x, y, chipImgX);

				//Set zone position and chip images for each zone
				Zone(player, x, y, tieZone, chipTieX, chipTieY, 193, 454, 259, 302);
				Zone(player, x, y, bankerZone, chipBankX, chipBankY, 163, 482, 302, 357);
				Zone(player, x, y, playerZone, chipPlayerX, chipPlayerY, 129, 515, 357, 424);

				//if pressed "Deal" go to next state
				if (x > 346 && x < 392 && y > 425 && y < 468 && (betQ) > 0){
					stringstream ss;
					string tieZoneString, bankerZoneString, playerZoneString;
					ss << tieZone;
					ss >> tieZoneString;
					ss.clear();
					ss << bankerZone;
					ss >> bankerZoneString;
					ss.clear();
					ss << playerZone;
					ss >> playerZoneString;
					string sendString = (string("deal") + " " + tieZoneString + " " + bankerZoneString
							                            + " " + playerZoneString);
					Send(sendString.c_str(), sendString.length()+1, socket);

					inARow = 1;
				}
				event.button.type = SDL_MOUSEBUTTONUP; // releases mouse button
			}
		}

		//Game's second state
		else if (inARow == 1){
			//Deals 2 cards for player and banker
			for(int i = 0; i < 2; i++){
				GetCard(socket, cNumX, cNumY, cFilePosX, cFilePosY, i, playerCards);
				GetCard(socket, cNumX, cNumY, cFilePosX, cFilePosY, i+3, bankCards);
			}
			CardsMod(playerCards, bankCards);
			rlz.getCards(playerCards, bankCards);
			result = rlz.checkWinner();

			//If not natural
			if (playerCards < 8 && bankCards < 8){
				//Check rules if needed to draw third card to player
				if (rlz.drawthirdplayer() == true){
					GetCard(socket, cNumX, cNumY, cFilePosX, cFilePosY, 2, face);
					//If so - moves card on screen
					cOnScrX[2] = 72;
					//Refreshes information about player's third card
					rlz.getThirdPlayer(face);
					playerCards += face;
				}
				//Check rules if needed to draw third card to banker
				if (rlz.drawthirdbanker() == true){
					GetCard(socket, cNumX, cNumY, cFilePosX, cFilePosY, 5, face);
					//Moves card on screen
					bankCards += face;
					cOnScrX[5] = 311;
				}
				CardsMod(playerCards, bankCards);
				rlz.getCards(playerCards, bankCards);
				result = rlz.checkWinner();
			}

			//Sets winnings by rules
			switch(result){
			case 1: player.setCash(playerZone * 2); break;
			case 2: player.setCash(bankerZone * 1.95); break;
			case 3: player.setCash(tieZone * 9); break;
			default:break;
			}
			//Card show on screen timer
			CardDrawTimer = SDL_GetTicks();
			//Go to third state
			inARow = 2;
		}
		//Game's third state
		else if (inARow == 2){
			double winPts = 0; // "Win" text information
			switch (result){ // Switch winner
			case 1:
				if (playerZone > 0) // Checks where the bet is made
					winPts = playerZone * 2;
				break;
			case 2:
				if (bankerZone > 0)
					winPts = bankerZone * 1.95;
				break;
			case 3:
				if (tieZone > 0)
					winPts = tieZone * 9;
				break;
			}
			//Information text about win and scores after deal
			TextDefine chipTxt(font, 13, 255, 255, 255, NumToText(winPts));
			TextDefine scorePlayer(font, 14, 255, 255, 255, NumToText(playerCards));
			TextDefine scoreBanker(font, 14, 255, 255, 255, NumToText(bankCards));

			//Defines 3 cards for player and 3 for banker
			ObjDefine pCard1("CardsDeck.bmp", cNumX[0], cNumY[0], cSizeX, cSizeY);
			pCard1.ObjTransp(255, 0, 255); // set transparent color for image

			ObjDefine pCard2("CardsDeck.bmp", cNumX[1], cNumY[1], cSizeX, cSizeY);
			pCard2.ObjTransp(255, 0, 255);

			ObjDefine pCard3("CardsDeck.bmp", cNumX[2], cNumY[2], cSizeX, cSizeY);
			pCard3.ObjTransp(255, 0, 255);

			ObjDefine bCard1("CardsDeck.bmp", cNumX[3], cNumY[3], cSizeX, cSizeY);
			bCard1.ObjTransp(255, 0, 255);

			ObjDefine bCard2("CardsDeck.bmp", cNumX[4], cNumY[4], cSizeX, cSizeY);
			bCard2.ObjTransp(255, 0, 255);

			ObjDefine bCard3("CardsDeck.bmp", cNumX[5], cNumY[5], cSizeX, cSizeY);
			bCard3.ObjTransp(255, 0, 255);

			//Draws the background
			background.ObjDraw(0, 0, screen);

			//Cards deal timer in order
			if( (SDL_GetTicks() - CardDrawTimer) > 500 )   {
				pCard1.ObjDraw(cOnScrX[0], cOnScrY, screen);
			}
			if( (SDL_GetTicks() - CardDrawTimer) > 1000 )   {
				bCard1.ObjDraw(cOnScrX[3], cOnScrY, screen);
			}
			if( (SDL_GetTicks() - CardDrawTimer) > 1500 )   {
				pCard2.ObjDraw(cOnScrX[1], cOnScrY + 7, screen);
			}
			if( (SDL_GetTicks() - CardDrawTimer) > 2000 )   {
				bCard2.ObjDraw(cOnScrX[4], cOnScrY + 7, screen);
			}
			if( (SDL_GetTicks() - CardDrawTimer) > 2500 && cOnScrX[2] == 72)   {
				pCard3.ObjDraw(cOnScrX[2], cOnScrY + 14, screen);
			}
			if( (SDL_GetTicks() - CardDrawTimer) > 3000 && cOnScrX[5] == 311)   {
				bCard3.ObjDraw(cOnScrX[5], cOnScrY + 14, screen);
			}
			if( (SDL_GetTicks() - CardDrawTimer) > 3500 )   {
				scorePlayer.textDraw(152, 18, screen);
				scoreBanker.textDraw(391, 18, screen);
			}
			ObjDefine winChips("WinChips.bmp", 0, 0, 60, 75);
			winChips.ObjTransp(255, 0, 255);
			if ((SDL_GetTicks() - CardDrawTimer) > 3500 ){
				if (playerZone > 0 && result == 1)
					winChips.ObjDraw(200, 335, screen);
				else if (bankerZone > 0 && result == 2)
					winChips.ObjDraw(225, 270, screen);
				else if (tieZone > 0 && result == 3)
					winChips.ObjDraw(250, 220, screen);
			}

			//Shows "Win" text in order after cards deal
			if( (SDL_GetTicks() - CardDrawTimer) > 3800 )   {
				switch (result){
				case 1: win.textDraw(60, 122, screen); break;
				case 2: win.textDraw(297, 122, screen); break;
				case 3: tie.textDraw(185, 69, screen); break;
				default:break;
				}
				//Draw information (bottom of the screen)
				chipTxt.textDraw(256, 463, screen);
				cashTxt.textDraw(58, 463, screen);
				betTxt.textDraw(179, 463, screen);
			}
			DrawChips(chipTieX, chipTieY, chipBankX, chipBankY,
				tieZone, playerZone, chipPlayerX, chipPlayerY, bankerZone, screen);
			//Draws the mouse image
			mousePointer.ObjDraw(x, y, screen);
			SDL_Flip(screen);
			//If mouse button pressed after game - game restart
			if (event.button.type == SDL_MOUSEBUTTONDOWN && (SDL_GetTicks() - CardDrawTimer) > 4000){
				//Null all variables
				playerZone = 0;
				bankerZone = 0;
				tieZone = 0;
				playerCards = 0;
				bankCards = 0;
				face = 0;
				rlz.getCards(0,0);
				rlz.getThirdPlayer(0);
				cOnScrX[2] = 650;
				cOnScrX[5] = 650;
				inARow = 0;
			}
		}
		//If mouse pointer on "Deal" button, change cursor
		if (x > 272 && x < 394 && y > 426){
			chipImgX = 149;
			player.setBet(0);
		}
		//Draw everything while not in game's third part (dealing)
		if (inARow != 2){
			background.ObjDraw(0, 0, screen);
			mousePointer.ObjDraw(x, y, screen);
			//Text information (bottom of screen)
			chipTxt.textDraw(256, 463, screen);
			cashTxt.textDraw(58, 463, screen);
			betTxt.textDraw(179, 463, screen);
			//Draw chips on the table
			DrawChips(chipTieX, chipTieY, chipBankX, chipBankY,
					tieZone, playerZone, chipPlayerX, chipPlayerY, bankerZone, screen);
			SDL_Flip(screen);
			SDL_FillRect(SDL_GetVideoSurface(), NULL, 0);
		}
	}
	SDL_FreeSurface(screen);
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();
	SDL_Quit();
	TTF_Quit();
    return 0;
}//end of main
//Converts digit to text
string NumToText(double num){
	stringstream sSNum;
	sSNum << num;
	string strNum = sSNum.str();
	return strNum.c_str();
}
//Convert cards by rules (cards sum % 10)
void CardsMod(int &pSum, int &bSum){
	pSum = pSum%10;
	bSum = bSum%10;
}
//Detects and changes the mouse cursor image (with chips)
int MouseImg(Player &player, int mX, int mY, int imgX){
	if (mY > 432){
		if (mX > 464 && mX < 504){
			player.setBet(1);
			imgX = 0;
		}
		else if (mX > 509 && mX < 549){
			player.setBet(5);
			imgX = 37;
		}
		else if (mX > 552 && mX < 593){
			player.setBet(10);
			imgX = 75;
		}
		else if (mX > 596 && mX < 637){
			player.setBet(50);
			imgX = 112;
		}
	}
	return imgX;
}
//Draw chips on the table
void Zone(Player &player,int mX, int mY, int &zone, int &chipX, int &chipY, int fX, int sX, int fY, int sY){
	if (mX > fX && mX < sX && mY > fY && mY < sY &&
			(zone + player.getBet()) <= 100 &&
			player.getCash() - player.getBet() >= 0){
		zone += player.getBet();
		player.setCash(-player.getBet());
		if (zone%10 == 0)
			chipX = 9*60;
		else chipX = ((zone%10)-1)*60;
		if (zone%10 != 0)
			chipY = (zone/10)*75;
		else chipY = ((zone/10)-1)*75;
	}
}
//Get card from server
bool GetCard(TCPsocket socket,int *fromFileX, int *fromFileY, int *cardX, int *cardY, int num, int& score){
	char data[5];
	if ( !Recieve( data, 5, socket) ){
		return false;
	}
	if ( string(data) != "card" ){
		return false;
	}

	int face, suit;
	if ( !Recieve( &face, sizeof(face), socket) ){
		return false;
	}
	if ( !Recieve( &suit, sizeof(suit), socket) ){
		return false;
	}
	fromFileX[num] = cardX[face-1];
	fromFileY[num] = cardY[suit];

	if ( face < 10 )
		score += face;

	return true;
}
//Chips picture on the table
void DrawChips(int chipTieX, int chipTieY, int chipBankX, int chipBankY,
		int tieZone, int playerZone,int chipPlayerX, int chipPlayerY,
		int bankerZone, SDL_Surface* screen){

	ObjDefine tieBetChip("AllChips.bmp", chipTieX, chipTieY, 60, 75);
	tieBetChip.ObjTransp(255, 0, 255);

	ObjDefine bankerBetChip("AllChips.bmp", chipBankX, chipBankY, 60, 75);
	bankerBetChip.ObjTransp(255, 0, 255);

	ObjDefine playerBetChip("AllChips.bmp", chipPlayerX, chipPlayerY, 60, 75);
	playerBetChip.ObjTransp(255, 0, 255);

	//Draw chips on board
	if (tieZone > 0){
		tieBetChip.ObjDraw(304, 230, screen);
	}
	if (playerZone > 0){
		playerBetChip.ObjDraw(285, 350, screen);
	}
	if (bankerZone > 0){
		bankerBetChip.ObjDraw(295, 283, screen);
	}
}


