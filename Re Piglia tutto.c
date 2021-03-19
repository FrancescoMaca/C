/*
* Project name: Re Piglia Tutto
* Authors: Macaluso Francesco
* Date: 12.13.2020
* Version: 1.0
* Platform: Visual Studio 2019
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

#define deckSize 40
#define true 1
#define false 0

// Stack methods
void pushPlayer(int value);
void pushAi(int value);
void pushMain(int value);
int popPlayer();
int popAi();
int popMain();
int isPlayerDeckFull();
int isPlayerDeckEmpty();
int isAiDeckFull();
int isAiDeckEmpty();
int isMainDeckFull();
int isMainDeckEmpty();

// Logic methods
void initMainDeck();
int drawCard();

// UI methods
int printMenu(char name[]);
void printPlayerStack();
void printAiStack();

// Global Variables
int playerDeck[deckSize];
int aiDeck[deckSize];
int mainDeck[deckSize];

// Pointers to first free space
int playerTop = 0;
int aiTop = 0;
int mainTop = 0;

// Defining words
char seeds[5][7] = {
	"no_seed",
	"cuori",
	"fiori",
	"quadri",
	"picche"
};

int main() {

	srand(time(NULL));

	// Initializing main stack
	initMainDeck();

	int turn = 0;

	// Requesting name to the player
	char name[15];

	printf("Benvenuto in Re piglia tutto!\n");
	printf("Inserisci il tuo nome: ");
	gets(name);

	system("cls");

	int temp[36];

	int tempIndex = 0;

	// Game loop
	while (mainTop != 0) {
		if (turn == 0) {
			int choice = printMenu(name);

			switch (choice) {
			case 1:
				temp[tempIndex] = drawCard();

				// If the card is a king then empties the buffer in the current player's stack
				if (temp[tempIndex] % 10 == 0) {
					
					int j;

					// Pushing all the temp cards into players deck
					for (j = 0; j < tempIndex + 1; j++) {
						pushPlayer(temp[j]);
					}

					tempIndex = 0;
					printf("Ora hai %d carte nel mazzo!\n", playerTop);
				}
				else {
					// Incrementing temporary card index
					tempIndex++;
				}
				break;
			case 2:
				printf("Arrivederci %s!\n", name);
				return 0;
			default:
				printf("Immetti una scelta valida!\n");
				break;
			}

			// Passes the turn
			turn = 1;
		}
		else {
			printf("E' il turno di AI!!\n");

			// Draws a card
			temp[tempIndex] = drawCard();

			// If the card is a king then empties the buffer in the current player's stack
			if (temp[tempIndex] % 10 == 0) {
				int j;

				// Pushing all the temp cards into players deck
				for (j = 0; j < tempIndex + 1; j++) {
					pushAi(temp[j]);
				}

				tempIndex = 0;
				printf("Ora AI ha %d carte nel mazzo!\n", aiTop);
			}
			else {
				// Incrementing temporary card index
				tempIndex++;
			}

			// Passes the turn
			turn = 0;
		}

		_getch();
	}

	if (playerTop > aiTop) {
		printf("%s ha vinto la partita!!\n", name);
	}
	else {
		printf("AI ha vinto la partita!!\n");
	}
	printPlayerStack();
	printAiStack();
}

void printPlayerStack() {

	int i = 1;

	// Prints the cards the player has
	printf("Player: (%d)\n", playerTop);
	while (playerTop > 0) {
		// Converts number to string
		char val[3];
		_itoa_s(popPlayer(), val, 3, 10);

		printf("%2d - %c di %s\n", i, val[1], seeds[val[0] - 48]);
		i++;
	}
}

void printAiStack() {

	int i = 1;

	// Prints the cards the player has
	printf("AI: (%d)\n", aiTop);
	while(aiTop > 0) {
		// Converts number to string
		char val[3];
		_itoa_s(popAi(), val, 3, 10);

		printf("%2d - %c di %s\n", i, val[1], seeds[val[0] - 48]);
		i++;
	}
}

int drawCard() {
	int card = popMain();

	char _card[3];
	_itoa_s(card, _card, 3, 10);

	// If the card has a number and a 0 (ie 10, 20, 30...) then its a king
	switch (_card[0]) {
	case '1':
		if (_card[1] == '0') {
			printf("E' uscita il re di cuori!\n");
			break;
		}
		printf("E' uscita il %c di cuori!\n", _card[1]);
		break;
	case '2':
		if (_card[1] == '0') {
			printf("E' uscita il re di fiori!\n");
			break;
		}
		printf("E' uscita il %c di fiori!\n", _card[1]);
		break;
	case '3':
		if (_card[1] == '0') {
			printf("E' uscita il re di quadri!\n");
			break;
		}
		printf("E' uscita il %c di quadri!\n", _card[1]);
		break;
	case '4':
		if (_card[1] == '0') {
			printf("E' uscita il re di picche!\n");
			break;
		}
		printf("E' uscita il %c di picche!\n", _card[1]);
		break;
	}
	
	printf("Questa carta e' stata aggiunta al tavolo!\n\n");

	// Returns the card drawn
	return card;
}


int printMenu(char name[]) {
	printf("E' il tuo turno %s!!\n\n", name);
	printf("Carte rimanenti: %d\n\n", mainTop);
	printf("Scegli cosa fare:\n");
	printf("1 - Rivela carta\n");
	printf("2 - Esci\n\n");

	return _getch() - 48;
}

void initMainDeck() {
	
	int i = 0;
	int temp[40];
	int tempIndex = 0;

	// Initialising the array
	for (i = 10; i < 50; i++) {
		temp[i - 10] = i;
	}

	// Shuffles the deck (40 shuffles)
	for (i = 0; i < 40; i++) {
		// Choosing two random locations
		int first = rand() % 40;
		int second = rand() % 40;

		// Swapping these locations
		int tmp = temp[first];
		temp[first] = temp[second];
		temp[second] = tmp;
	}

	// Pushes the temp array to the stack
	for (i = 0; i < 40; i++) {
		pushMain(temp[i]);
	}
}

// ***************  STACK FUNCTIONS  *************** //
void pushPlayer(int value) {
	if (isPlayerDeckFull()) {
		printf("Lo stack player e' pieno!\n");
		return;
	}

	playerDeck[playerTop] = value;
	playerTop++;
}

void pushAi(int value) {
	if (isAiDeckFull()) {
		printf("Lo stack ai e' pieno!\n");
		return;
	}

	aiDeck[aiTop] = value;
	aiTop++;
}

void pushMain(int value) {
	if (isMainDeckFull()) {
		printf("Lo stack main e' pieno!\n");
		return;
	}

	mainDeck[mainTop] = value;
	mainTop++;
}

int popPlayer() {
	if (isPlayerDeckEmpty()) {
		printf("Lo stack player e' vuoto!\n");
		return;
	}

	return playerDeck[--playerTop];
}

int popAi()  {
	if (isAiDeckEmpty()) {
		printf("Lo stack Ai e' vuoto!\n");
		return;
	}

	return aiDeck[--aiTop];
}

int popMain() {
	if (isMainDeckEmpty()) {
		printf("Lo stack player e' vuoto!\n");
		return;
	}

	return mainDeck[--mainTop];
}

int isPlayerDeckFull()
{
	if (playerTop == deckSize) {
		return true;
	}

	return false;
}

int isPlayerDeckEmpty()
{
	if (playerTop == 0) {
		return true;
	}

	return false;
}

int isAiDeckFull()
{
	if (aiTop == deckSize) {
		return true;
	}

	return false;
}

int isAiDeckEmpty()
{
	if (aiTop == 0) {
		return true;
	}

	return false;
}

int isMainDeckFull()
{
	if (mainTop == deckSize) {
		return true;
	}

	return false;
}

int isMainDeckEmpty()
{
	if (mainTop == 0) {
		return true;
	}

	return false;
}