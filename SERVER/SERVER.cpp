#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_net.h"

#include "Player.h"
#include "Network.h"
#include "CardDeal.h"
#include "Decks.h"
#include "Rules.h"

#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

vector<Player> players;

Card DealCard(Deck&, int&);// Draws cards from a deck and calculates it's score
void SendCard(Player&, Card&);// Send card to the player
bool AllPlayersReady(); // Check if all players pressed "Deal"

int main(int argc, char* argv[]) {
	if(SDLNet_Init() == -1) {
	    printf("SDLNet_Init: %s\n", SDLNet_GetError());
	    exit(2);
	}
	srand(SDL_GetTicks());
	TCPsocket server = CreateServer();

	cout << "Waiting 10 seconds for players" << endl;

	int startTime = SDL_GetTicks();
	char data[512];
	while( (players.size() < 7 && (SDL_GetTicks() - startTime) < 15000) || players.size() == 0 ){
		TCPsocket sock = ListenForClient(server);
		if ( Recieve( data, 512, sock) ){
			if ( string(data) == "hi" ){
				string playerName;
				stringstream ss;
				ss << "Player" << players.size();
				ss >> playerName;
				cout << playerName << " connected" << endl;

				Player p(playerName, 1000,0);
				p.socket = sock ;
				players.push_back(p);
			}
		}
	}

	cout << "Game has started with " << players.size() << " players" << endl;
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(players.size());
	vector<SDL_Thread*> threads(players.size());
	for(unsigned i = 0; i < players.size();i++){
		double cash = players[i].getCash();
		double bet  = players[i].getBet();
		if ( !Send(&cash, sizeof(double), players[i].socket) || !Send(&bet, sizeof(double), players[i].socket) ){
			cout << players[i].getName() << " has dropped" << endl;
		}
		SDLNet_TCP_AddSocket(socketSet, players[i].socket);
	}

	bool gameActive = true;
	while(gameActive){

		cout << "New round starting - waiting 30 seconds for all players to press deal" << endl;
		// Start counting to 30 seconds. If a player is not ready at this time he is kicked out of the game,and the game continues
		int roundStarted = SDL_GetTicks();
		while( SDL_GetTicks() - roundStarted < 30000 || !AllPlayersReady() ){
			if( SDLNet_CheckSockets(socketSet, 0) ){
				for(unsigned i = 0; i  < players.size(); i++){
					if ( SDLNet_SocketReady(players[i].socket) ){
						if (Recieve(data, 512, players[i].socket)){
							string str(data);

							// Clients send the deal command when they press deal,along with the zones info
							if (  str.find("deal") != string::npos ){
								if ( !players[i].dealed ){
									string trimed = str.substr(4);
									stringstream ss(trimed);
									double betOnTie, betOnBank, betOnPlayer;
									ss >> betOnTie >> betOnBank >> betOnPlayer;

									players[i].tieZone    = betOnTie;
									players[i].bankerZone = betOnBank;
									players[i].playerZone = betOnPlayer;
									players[i].playerCards = 0;
									cout << players[i].getName() << " dealed" << betOnTie << betOnBank << betOnPlayer << endl;
									players[i].dealed = true;
								}
								else{
									cout << players[i].getName() <<  " tried to deal twice in the same round!" << endl;
								}
							}
						}
					}
				}
			}
		}

		Deck deck;
		deck.shuffle();
		Rules rlz;
		vector<Card> playerCards;
		vector<Card> bankerCards;
		// Value of cards
		int playerCardsScore = 0;
		int bankerCardsScore = 0;
		int thirdPlayer = 0;

		// Give 2 cards to each player and the banker.
		for (int i = 0; i < 2; i++){
			playerCards.push_back( DealCard(deck, playerCardsScore) );
			bankerCards.push_back( DealCard(deck, bankerCardsScore) );
		}
		rlz.getCards(playerCardsScore, bankerCardsScore);

		// Now check if we should give a third card to the players or the banker
		if (playerCardsScore < 8 && bankerCardsScore < 8){
			//Check rules if needed to draw third card to player
			if (rlz.drawthirdplayer() == true){
				playerCards.push_back(DealCard(deck, thirdPlayer));
				rlz.getThirdPlayer(thirdPlayer);
			}
			if (rlz.drawthirdbanker() == true){
				bankerCards.push_back(DealCard(deck, bankerCardsScore));
			}
		}

		// Send each card to the players
		for(unsigned p = 0; p < players.size(); p++){
			for (unsigned i = 0; i < playerCards.size(); i++){
				SendCard(players[p], playerCards[i]);
			}
			for (unsigned i = 0; i < bankerCards.size(); i++){
				SendCard(players[p], bankerCards[i]);
			}
		}
		playerCardsScore = 0;
		bankerCardsScore = 0;
		while (!playerCards.empty())
			playerCards.pop_back();
		while (!bankerCards.empty())
			bankerCards.pop_back();

		// Clear info by the end of the round
		for(unsigned p = 0; p < players.size(); p++ ){
			players[p].dealed = false;
		}
	}

	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	return 0;
}
// Draws cards from a deck and calculates it's score
Card DealCard(Deck& deck, int& cards){
	Card c = deck.dealOneCard();
	int face =  c.getFace();
	if ( face < 10 )
		cards += c.getFace();
	return c;
}
// Send card to the player
void SendCard(Player& player, Card& card){
	int face = card.getFace();
	int suit = card.getSuit();

	Send("card", 5, player.socket);
	Send(&face, sizeof(face), player.socket);
	Send(&suit, sizeof(suit), player.socket);
}
//Check if all players pressed "deal"
bool AllPlayersReady(){
	for(unsigned i = 0; i < players.size(); i++){
		if ( !players[i].dealed )
			return false;
		}
	return true;
}
