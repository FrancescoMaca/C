/*
* Project name: I 5 Elementi
* Authors: Macaluso Francesco, Patrick Blancuzzi
* Date: 1.01.2021
* Version: 3.0
* Platform: Visual Studio 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>  // Libreria per caratteri strani (2 byte) --> grafica
#include <locale.h> // Imposta formato console --> grafica collegato con wchar
#include <conio.h>
#include <Windows.h>

#define DRAGO   5
#define FUOCO 	4
#define LEGNO 	3
#define ACQUA 	2
#define METALLO 1
#define TERRA 	0

/*
* Defining how many cards per type there are in the deck
*/
#define NORMAL_CARD 	2
#define DRAGON_CARD		1

/*
* Points for each game state
*/
#define WIN  3
#define DRAW 1
#define LOST 0

// Defining boolean values
#define true 1
#define false 0

/*
* Defining how many cards each player will have in their hands
*/
#define HAND_SIZE 2

/*
* This is a macro for defining a card that isn't there
*/
#define EMPTY -1

/*
* Giving values of the turns
*/
#define PLAYER_TURN 0
#define AI_TURN 1

/*
* Another macro to store the deck size based on how many cards per type
* and dragon cards are set
*/
#define DECK_SIZE ((5 * NORMAL_CARD) + DRAGON_CARD)

/*
* UI methods (Graphics not implemented yet)
*/
void title();
void displayRules();
void drawTable(int cardsLeft, int playerCardPlayed, int aiCardPlayed);
void drawPlayerDeck(int cardsLeft);
void drawAiDeck(int cardsLeft);
void drawBothCardsPlayed(int leftCard, int rightCard);
void drawAiCardPlayed(int card);
void drawPlayerCardPlayed(int card);

char drawMainMenu();

/*
* Logic methods
*/
void shuffleDeck(int arr[]);
void initDecks();
void initHands(int hand[], int turn);
void startGame();
void drawCard(int playerHand[], int aiHand[]);

int flipCoin();
int playCard(int hand[]);
int getWinner(int secondCard, int firstCard);
int aiMove(int hand[], int cardPlayed);
int playAgain();
int pointsManager(int firstCard, int secondCard, int turn);

/*
* Matrix with the card comparison
*/
const int results[6][6] = {
	{ DRAW, WIN , DRAW, DRAW, LOST, WIN },
	{ LOST, DRAW, WIN , DRAW, DRAW, WIN },
	{ DRAW, LOST, DRAW, WIN , DRAW, WIN },
	{ DRAW, DRAW, LOST, DRAW, WIN , WIN },
	{ WIN , DRAW, DRAW, LOST, DRAW, WIN },
	{ LOST, LOST, LOST, LOST, LOST, DRAW},
};

/*
* Array to store and display cards name
*/
const char cardType[6][10] = { "Terra", "Metallo", "Acqua", "Legno", "Fuoco", "Drago" };

/*
* Player stack
*/
int mazzoPlayer[DECK_SIZE];
int topPlayer = 0;

void pushPlayer(int val) {
	if (isFullPlayer()) {
		return;
	}

	mazzoPlayer[topPlayer] = val;
	topPlayer++;
}

int popPlayer() {
	if (isEmptyPlayer()) {
		return EMPTY;
	}

	return mazzoPlayer[--topPlayer];
}

int isFullPlayer() {
	if (topPlayer == DECK_SIZE) {
		return true;
	}

	return false;
}

int isEmptyPlayer() {
	if (topPlayer == 0) {
		return true;
	}

	return false;
}

// AI Stack
int mazzoAi[DECK_SIZE];
int topAi = 0;

void pushAi(int val) {
	if (isFullAi()) {
		return;
	}

	mazzoAi[topAi] = val;
	topAi++;
}

int popAi() {
	if (isEmptyAi()) {
		return EMPTY;
	}

	return mazzoAi[--topAi];
}

int isFullAi() {
	if (topAi == DECK_SIZE) {
		return true;
	}

	return false;
}

int isEmptyAi() {
	if (topAi == 0) {
		return true;
	}

	return false;
}


// Defining player variables
int points[2] = { 0, 0 };

// Variable for name
char name[15];

int main() {

	/*
	* Initialize the random seed to null
	*/
	srand(time(NULL));

	/*
	* Set the window format for the console
	*/
	setlocale(LC_ALL, ".UTF8");

	/*
	*  Intializes the decks
	*/
	initDecks();

	/*
	* Title input
	*/
	title();

	/*
	* Menu loop
	*/
	while (true) {
		char c = drawMainMenu();

		switch (c) {
		case '1':
			startGame();

			/*
			* At the end of the games checks which player has won the game
			*/
			if (points[PLAYER_TURN] > points[AI_TURN]) {
				printf("\t\t\033[94m%s\033[0m ha vinto la partita con %d punti. \033[33mL'intelligenza artificiale\033[0m invece ne ha %d!\n", name, points[PLAYER_TURN], points[AI_TURN]);
			}
			else if (points[PLAYER_TURN] == points[AI_TURN]) {
				printf("\t\tC'e' stato un pareggio con %d punti!\n", points[PLAYER_TURN]);
			}
			else {
				printf("\t\tL'\033[33mintelligenza artificiale\033[0m ha vinto la partita con %d punti. \033[94m%s\033[0m ne ha fatti %d!\n", points[AI_TURN], name, points[PLAYER_TURN]);
			}

			_getch();

			if (!playAgain()) {
				return 0;
			}

			break;
		case '2':

			displayRules();

			break;
		case '3':
			return 0;
		}
	}

	return 0;
}

/// <summary>
/// Shuffles an array.
/// Status: WORKING
/// </summary>
/// <param name="arr">The array you need to shuffle</param>
void shuffleDeck(int arr[]) {
	int i, j;
	int shuffleTimes = 100;

	// Shuffles items in the player's deck
	for (i = 0; i < shuffleTimes; i++) {
		int index1 = rand() % DECK_SIZE;
		int index2 = rand() % DECK_SIZE;

		int tmp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = tmp;
	}
}

/// <summary>
/// Puts the cards in the decks and then shuffles them.
/// Status: WORKING
/// </summary>
void initDecks() {
	int i;

	for (i = 0; i < NORMAL_CARD; i++) {
		pushPlayer(FUOCO);
		pushPlayer(LEGNO);
		pushPlayer(ACQUA);
		pushPlayer(METALLO);
		pushPlayer(TERRA);

		pushAi(FUOCO);
		pushAi(LEGNO);
		pushAi(ACQUA);
		pushAi(METALLO);
		pushAi(TERRA);
	}

	for (i = 0; i < DRAGON_CARD; i++) {
		pushPlayer(DRAGO);
		pushAi(DRAGO);
	}

	shuffleDeck(mazzoPlayer);
	shuffleDeck(mazzoAi);
}

/// <summary>
/// Randomly returns the starting player's turn 
/// </summary>
/// <returns>Returns the starting player's turn </returns>
int flipCoin() {
	if (rand() % 2) {
		return PLAYER_TURN;
	}

	return AI_TURN;
}

/// <summary>
/// Initializes the hands of both players.
/// Status: WORKING
/// </summary>
void initHands(int hand[], int turn) {
	int i;

	// il !isEmptyPlayer serve per vedere che il mazzo abbia carte a sufficienza 
	// in caso la mano sia piu grande delle carte totali del mazzo
	if (turn == PLAYER_TURN) {
		for (i = 0; i < HAND_SIZE && !isEmptyPlayer(); i++) {
			hand[i] = popPlayer();
		}
	}
	else {
		for (i = 0; i < HAND_SIZE && !isEmptyAi(); i++) {
			hand[i] = popAi();
		}
	}
}

/// <summary>
/// Displays the title to the user and asks for his name.
/// Status: WORKING
/// </summary>
void title() {
	printf("\n\n");
	wprintf(L"\033[31m       ██▓    ▄████▄   ██▓ ███▄    █   █████   █    ██ ▓█████    ▓█████  ██▓    ▓█████  ███▄ ▄███▓▓█████  ███▄    █ ▄▄▄█████▓ ██▓\n");
	wprintf(L"      ▓██▒   ▒██▀ ▀█  ▓██▒ ██ ▀█   █ ▒██▓  ██▒ ██  ▓██▒▓█   ▀    ▓█   ▀ ▓██▒    ▓█   ▀ ▓██▒▀█▀ ██▒▓█   ▀  ██ ▀█   █ ▓  ██▒ ▓▒▓██▒\n");
	wprintf(L"      ▒██▒   ▒▓█    ▄ ▒██▒▓██  ▀█ ██▒▒██▒  ██░▓██  ▒██░▒███      ▒███   ▒██░    ▒███   ▓██    ▓██░▒███   ▓██  ▀█ ██▒▒ ▓██░ ▒░▒██▒\n");
	wprintf(L"      ░██░   ▒▓▓▄ ▄██▒░██░▓██▒  ▐▌██▒░██  █▀ ░▓▓█  ░██░▒▓█  ▄    ▒▓█  ▄ ▒██░    ▒▓█  ▄ ▒██    ▒██ ▒▓█  ▄ ▓██▒  ▐▌██▒░ ▓██▓ ░ ░██░\n");
	wprintf(L"      ░██░   ▒ ▓███▀ ░░██░▒██░   ▓██░░▒███▒█▄ ▒▒█████▓ ░▒████▒   ░▒████▒░██████▒░▒████▒▒██▒   ░██▒░▒████▒▒██░   ▓██░  ▒██▒ ░ ░██░\n");
	wprintf(L"\033[91m      ░▓     ░ ░▒ ▒  ░░▓  ░ ▒░   ▒ ▒ ░░ ▒▒░ ▒ ░▒▓▒ ▒ ▒ ░░ ▒░ ░   ░░ ▒░ ░░ ▒░▓  ░░░ ▒░ ░░ ▒░   ░  ░░░ ▒░ ░░ ▒░   ▒ ▒   ▒ ░░   ░▓  \n");
	wprintf(L"       ▒ ░     ░  ▒    ▒ ░░ ░░   ░ ▒░ ░ ▒░  ░ ░░▒░ ░ ░  ░ ░  ░    ░ ░  ░░ ░ ▒  ░ ░ ░  ░░  ░      ░ ░ ░  ░░ ░░   ░ ▒░    ░     ▒ ░\n");
	wprintf(L"       ▒ ░   ░         ▒ ░   ░   ░ ░    ░   ░  ░░░ ░ ░    ░         ░     ░ ░      ░   ░      ░      ░      ░   ░ ░   ░       ▒ ░\n");
	wprintf(L"       ░     ░ ░       ░           ░     ░       ░        ░  ░      ░  ░    ░  ░   ░  ░       ░      ░  ░         ░           ░  \n");
	wprintf(L"\033[0m\n");
	printf("\t\t\t\t\t\tInserisci il tuo nome:  ");

	gets(name);
}

/// <summary>
/// Displays the main menu and asks for the user for an input.
/// Status: WORKING
/// </summary>
/// <returns>The player chocie</returns>
char drawMainMenu() {
	int i;

	system("cls");

	printf("\n\n");
	wprintf(L"\033[31m      ███▄ ▄███▓▓█████  ███▄    █  █    ██ \n");
	wprintf(L"     ▓██▒▀█▀ ██▒▓█   ▀  ██ ▀█   █  ██  ▓██▒\n");
	wprintf(L"     ▓██    ▓██░▒███   ▓██  ▀█ ██▒▓██  ▒██░\n");
	wprintf(L"     ▒██    ▒██ ▒▓█  ▄ ▓██▒  ▐▌██▒▓▓█  ░██░\n");
	wprintf(L"     ▒██▒   ░██▒░▒████▒▒██░   ▓██░▒▒█████▓ \n");
	wprintf(L"\033[91m     ░ ▒░   ░  ░░░ ▒░ ░░ ▒░   ▒ ▒ ░▒▓▒ ▒ ▒ \n");
	wprintf(L"     ░  ░      ░ ░ ░  ░░ ░░   ░ ▒░░░▒░ ░ ░ \n");
	wprintf(L"     ░      ░      ░      ░   ░ ░  ░░░ ░ ░ \n");
	wprintf(L"            ░      ░  ░         ░    ░     \033[0m\n\n");

	wprintf(L"╕\n");
	wprintf(L"│\n");
	wprintf(L"╘═══════╡ Ciao ");
	printf("%s", name);
	wprintf(L"! ╞═══════╕\n");

	// Gestisce la grafica una volta inserito il nome (lunghezza linee)
	wprintf(L"1) Inizia una partita    ");
	for (i = 0; i < strlen(name); i++) putchar(' ');
	wprintf(L"│\n");
	wprintf(L"2) Come si gioca??       ");
	for (i = 0; i < strlen(name); i++) putchar(' ');
	wprintf(L"│\n");
	wprintf(L"3) Esci                  ");
	for (i = 0; i < strlen(name); i++) putchar(' ');
	wprintf(L"│\n");
	wprintf(L"╒");

	for (i = 0; i < strlen(name) + 24; i++) putwchar(L'═');
	wprintf(L"╛\n┘");

	return _getch();
}

/// <summary>
/// Asks the player if he wants to play again
/// </summary>
/// <returns>Returns weather the player wants to restart the game or not</returns>
int playAgain() {

	printf("\t\tVuoi giocare di nuovo?? (y/n)\n>");

	char c = _getch();

	if (c == 'y') {

		/*
		* Resets all variables as if they were at the start of the game
		*/
		points[AI_TURN] = 0;
		points[PLAYER_TURN] = 0;

		topPlayer = DECK_SIZE;
		topAi = DECK_SIZE;

		/*
		* Re-shuffles the decks
		*/
		shuffleDeck(mazzoAi);
		shuffleDeck(mazzoPlayer);

		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// Displays the rules to the screen.
/// Status: WORKING
/// </summary>
void displayRules() {
	system("cls");

	printf("\n\n");
	wprintf(L"\t\t  \033[31m ▄████▄   ▒█████   ███▄ ▄███▓▓█████      ██████  ██▓     ▄████  ██▓ ▒█████   ▄████▄   ▄▄▄      \n");
	wprintf(L"\t\t  ▒██▀ ▀█  ▒██▒  ██▒▓██▒▀█▀ ██▒▓█   ▀    ▒██    ▒ ▓██▒    ██▒ ▀█▒▓██▒▒██▒  ██▒▒██▀ ▀█  ▒████▄    \n");
	wprintf(L"\t\t  ▒▓█    ▄ ▒██░  ██▒▓██    ▓██░▒███      ░ ▓██▄   ▒██▒   ▒██░▄▄▄░▒██▒▒██░  ██▒▒▓█    ▄ ▒██  ▀█▄  \n");
	wprintf(L"\t\t  ▒▓▓▄ ▄██▒▒██   ██░▒██    ▒██ ▒▓█  ▄      ▒   ██▒░██░   ░▓█  ██▓░██░▒██   ██░▒▓▓▄ ▄██▒░██▄▄▄▄██ \n");
	wprintf(L"\t\t  ▒ ▓███▀ ░░ ████▓▒░▒██▒   ░██▒░▒████▒   ▒██████▒▒░██░   ░▒▓███▀▒░██░░ ████▓▒░▒ ▓███▀ ░ ▓█   ▓██▒\n");
	wprintf(L"\t\t  \033[91m░ ░▒ ▒  ░░ ▒░▒░▒░ ░ ▒░   ░  ░░░ ▒░ ░   ▒ ▒▓▒ ▒ ░░▓      ░▒   ▒ ░▓  ░ ▒░▒░▒░ ░ ░▒ ▒  ░ ▒▒   ▓▒█░\n");
	wprintf(L"\t\t    ░  ▒     ░ ▒ ▒░ ░  ░      ░ ░ ░  ░   ░ ░▒  ░ ░ ▒ ░     ░   ░  ▒ ░  ░ ▒ ▒░   ░  ▒     ▒   ▒▒ ░\n");
	wprintf(L"\t\t  ░        ░ ░ ░ ▒  ░      ░      ░      ░  ░  ░   ▒ ░   ░ ░   ░  ▒ ░░ ░ ░ ▒  ░          ░   ▒   \n");
	wprintf(L"\t\t  ░ ░          ░ ░         ░      ░  ░         ░   ░           ░  ░      ░ ░  ░ ░            ░  ░\033[0m\n\n");
	wprintf(L"  ╒═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╕\n");
	wprintf(L"  │  Le regole dei \033[32m5 Elementi\033[0m (0.0v) sono semplici! Ci sono due giocatori, uno sei tu e l'altro e' l'intelligenza artificiale │\n");
	wprintf(L"  │  contro cui ti batterai. Ogni giocatore pesca la prima carta e si vede chi ha vinto. La seguente tabella raffigura quali  │\n");
	wprintf(L"  │  elementi vincono contro quali:                                                                                           │\n");
	wprintf(L"  ╞═════════════════════════════════╤═════════════════════════════════════════════════════════════════════════════════════════╛\n");
	wprintf(L"  │  Il \033[31mfuoco\033[0m batte il \033[93mlegno\033[0m        │\n");
	wprintf(L"  │  Il \033[93mlegno\033[0m batte l'\033[94macqua\033[0m         │\n");
	wprintf(L"  │  L'\033[94macqua\033[0m batte il \033[90mmetallo\033[0m       │\n");
	wprintf(L"  │  Il \033[90mmetallo\033[0m batte la \033[33mterra\033[0m      │\n");
	wprintf(L"  │  La \033[33mterra\033[0m batte il \033[31mfuoco\033[0m        │\n");
	wprintf(L"  ╞═════════════════════════════════╧══════════════════════════════════════════════════════════════════════════════════════╕\n");
	wprintf(L"  │ Quando si \033[32mvince\033[0m una mano si guadagna 3 punti, se si \033[33mpareggia\033[0m se ne vince 1 mentre se si \033[31mperde\033[0m non si guadagnano punti. │\n");
	wprintf(L"  ╘════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╛\n\n");
	_getch();
}

/// <summary>
/// Creates a new game and initializes all variables.
/// </summary>
void startGame() {


	int turn = flipCoin();

	/*
	* Defining the hands of each player
	*/
	int playerHand[HAND_SIZE];
	int aiHand[HAND_SIZE];

	/*
	* Initializing the hands of both player with the first two cards of each deck
	*/
	initHands(playerHand, PLAYER_TURN);
	initHands(aiHand, AI_TURN);

	/*
	* Declaring an array to store the cards played by the user
	*/
	int cardsPlayed[DECK_SIZE];
	int indexCardsPlayed = 0;

	/*
	* Initializing the cards deck
	*/
	int i;
	for (i = 0; i < DECK_SIZE; i++) {
		cardsPlayed[i] = EMPTY;
	}

	/*
	* Main game loop that checks if the hand is empty, if it is the game has ended
	*/
	int isHandEmpty = false;

	while (!isHandEmpty) {

		system("cls");
		drawTable(topPlayer, EMPTY, EMPTY);

		/*
		* Player's turn
		*/
		if (turn == PLAYER_TURN) {

			printf("\n");
			printf("\t\tE' il turno del player!\n");
			printf("\t\tScegli cosa fare:\n");
			printf("\t\t1) Gioca una carta\n");
			printf("\t\t2) \033[31mAbbandona\033[0m\n");

			char c = _getch();

			switch (c) {
			case '1': {

				/*
				* Refills the hand of both players
				*/
				drawCard(playerHand, aiHand);

				printf("\t\tScegli quale carta giocare: (%d rimaste)\n", topPlayer);

				/*
				* Prints player's hand
				*/
				int i;
				for (i = 0; i < HAND_SIZE; i++) {
					if (playerHand[i] != EMPTY) {
						printf("\t\t%d - Carta %s\n", i + 1, cardType[playerHand[i]]);
					}
				}

				/*
				* Requesting the card to the user
				*/
				int playerCard = playCard(playerHand);
				int aiCard = aiMove(aiHand, playerCard);

				system("cls");
				drawTable(topPlayer, playerCard, EMPTY);
				printf("\t\thai giocato una carta %s! \n", cardType[playerCard]);
				_getch();

				system("cls");
				drawTable(topPlayer, playerCard, aiCard);
				printf("\t\tL'intelligenza artificiale ha giocato una carta %s!\n", cardType[aiCard]);
				_getch();

				/*
				* Adding the played player's card to the array of cards played
				*/
				cardsPlayed[indexCardsPlayed] = playerCard;
				indexCardsPlayed++;

				/*
				* Calling thepointsManager points manager to give points to the players
				*/
				turn = pointsManager(playerCard, aiCard, turn);

				_getch();
				break;
			}
			case '2':
				return -1;
			default:
				printf("\t\tScelta invalida!\n");
			}
		}
		else {
			printf("\n");
			printf("\t\tE' il turno dell'intelligenza artificiale!\n");

			/*
			* Refills the hand of both players
			*/
			drawCard(playerHand, aiHand);
			_getch();

			int aiCard = aiMove(aiHand, EMPTY);
			system("cls");
			drawTable(topPlayer, EMPTY, aiCard);
			printf("\t\tL'intelligenza artificiale ha giocato una carta %s!\n", cardType[aiCard]);
			_getch();


			printf("\t\tScegli quale carta giocare: (%d rimaste)\n", topPlayer);

			/*
			* Prints player's hand
			*/
			int i;
			for (i = 0; i < HAND_SIZE; i++) {

				/*
				* Checks if the card is not empty to display it
				*/
				if (playerHand[i] != EMPTY) {
					printf("\t\t%d - Carta %s\n", i + 1, cardType[playerHand[i]]);
				}
			}

			/*
			* Asks the player for a card
			*/
			int playerCard = playCard(playerHand);

			system("cls");
			drawTable(topPlayer, playerCard, aiCard);

			/*
			* Adding the played player's card to the array of cards played
			*/
			cardsPlayed[indexCardsPlayed] = playerCard;
			indexCardsPlayed++;

			printf("\t\tHai giocato una carta %s!\n", cardType[playerCard]);

			/*
			* Calling the points manager to give points to the players
			*/
			turn = pointsManager(aiCard, playerCard, turn);


			_getch();
		}

		isHandEmpty = true;

		/*
		* If there is even only one card not EMPTY then the hand isn't empty and the game continues
		*/
		int i;
		for (i = 0; i < HAND_SIZE; i++) {
			if (playerHand[i] != EMPTY)
				isHandEmpty = false;
		}
	}
}

/// <summary>
/// Manage turn points and changes turn based on the results.
/// </summary>
/// <param name="firstCard">The card that the first player has played</param>
/// <param name="secondCard">The second card that has been played</param>
/// <param name="turn">The current turn</param>
/// <returns>Returns the new turn based on the result of the two cards</returns>
int pointsManager(int firstCard, int secondCard, int turn) {

	int opponentTurn = turn == 0 ? 1 : 0;

	if (getWinner(firstCard, secondCard) == WIN) {
		if (turn == PLAYER_TURN)
			printf("\t\tIl \033[94mplayer\033[0m ha vinto! Hai ricevuto %d punti!\n", WIN);
		else
			printf("\t\tL'\033[33mintelligenza artificiale\033[0m ha vinto! Ha ricevuto %d punti!\n", WIN);

		points[turn] += WIN;
		return turn;
	}
	else if (getWinner(firstCard, secondCard) == DRAW) {
		printf("\t\tC'e' stato un pareggio, entrambi i giocatori hanno vinto %d punto!\n", DRAW);

		points[turn] += DRAW;
		points[opponentTurn] += DRAW;
	}
	else {
		if (turn == PLAYER_TURN)
			printf("\t\tIl \033[94mplayer\033[0m ha perso! L'avversario ha ricevuto %d punti!\n", WIN);
		else
			printf("\t\t\033[33mL'intelligenza artificiale\033[0m ha perso! L'avversario ha ricevuto %d punti!\n", WIN);

		points[opponentTurn] += WIN;
	}

	return opponentTurn;
}

/// <summary>
/// Play the players card removing it after returning it
/// Status: WORKING
/// </summary>
/// <returns>The card the player chose</returns>
int playCard(int hand[]) {

	/*
	* Gets the player input as an index
	*/
	int index;

	/*
	* Keeps asking the player for a valid card
	*/
	do {
		index = _getch() - 49;
	} while (hand[index] == EMPTY || (index < 0 || index >= HAND_SIZE));


	int card = hand[index];

	/*
	* Removes the card from the hand
	*/
	hand[index] = EMPTY;

	return card;
}

/// <summary>
/// Returns the result of the first card against the second one.
/// Status: WORKING
/// </summary>
/// <param name="carta1">The first card</param>
/// <param name="carta2">The second card</param>
/// <returns>Wether the first card beats the second card</returns>
int getWinner(int firstCard, int secondCard) {
	return results[secondCard][firstCard];
}

/// <summary>
/// Moves a card from the deck to the hand.
/// Status: WORKING
/// </summary>
/// <param name="playerHand">The player hand that needs to be refilled</param>
/// <param name="aiHand">The ai hand that needs to be refilled</param>
void drawCard(int playerHand[], int aiHand[]) {
	int i;

	/*
	* Makes the player draw a card
	*/
	for (i = 0; i < HAND_SIZE; i++) {
		if (playerHand[i] == EMPTY) {

			/*
			* Draws a card from the deck
			*/
			playerHand[i] = popPlayer();

			/*
			* If the deck is empty then it doesnt print a message
			*/
			if (playerHand[i] != EMPTY) {
				printf("\t\tHai pescato una carta %s!\n", cardType[playerHand[i]]);
			}
			break;
		}
	}

	/*
	* Makes the AI draw a card
	*/
	for (i = 0; i < HAND_SIZE; i++) {
		if (aiHand[i] == EMPTY) {
			aiHand[i] = popAi();
			break;
		}
	}
}

/// <summary>
/// Finds the best card to play in hand against playerCard.
/// Status: WORKING
/// </summary>
/// <param name="playerCard">The card the opponent played</param>
/// <param name="hand">The hand you have to choose from</param>
/// <returns>Returns the best possible choice</returns>
int aiMove(int hand[], int cardPlayed) {

	int finalMove = 0;
	float bestValue = 0;

	int i, j;

	/*
	* Creates a copy of the player's deck
	*/
	int cardsLeft[DECK_SIZE];

	/*
	* Initializing the copy of the player's deck
	*/
	for (i = 0; i < DECK_SIZE; i++) {
		cardsLeft[i] = mazzoPlayer[i];
	}

	for (i = 0; i < HAND_SIZE; i++) {

		float cardValue = 0;

		/*
		* If the ai card is empty then skips the iteration
		*/
		if (hand[i] == EMPTY) {
			continue;
		}

		for (j = 0; j < topPlayer + HAND_SIZE; j++) {

			/*
			* If the card is EMPTY then skips the iteration
			*/
			if (cardsLeft[j] == EMPTY) {
				break;
			}

			/*
			* Checking if we have the card in our hand that can be the card played
			*/
			if (getWinner(hand[i], cardPlayed) == WIN && cardPlayed != EMPTY) {

				/*
				* Sets the variable to a very high value so that will be the played card
				*/
				cardValue = 100.0f;

				/*
				* Stores the current card
				*/
				finalMove = i;

				break;
			}

			cardValue += getWinner(hand[i], cardsLeft[j]);
		}

		cardValue /= (float)j;

		/*
		* If the new value is better than the best one then it replaces it
		*/
		if (cardValue > bestValue) {
			bestValue = cardValue;
			finalMove = i;
		}
	}

	/*
	* Stores the card in a variable so we can clear it
	*/
	int card = hand[finalMove];

	/*
	* Removes the card from the hand
	*/
	hand[finalMove] = EMPTY;

	return card;
}

/// <summary>
/// Manage all the graphic of the table and decks
/// </summary>
/// <param name="cardsLeft">the amount of cards left in the decks</param>
/// <param name="playerCardPlayed">the card the user played</param>
/// <param name="aiCardPlayed">the card the AI played</param>
void drawTable(int cardsLeft, int playerCardPlayed, int aiCardPlayed) {
	printf("\n");
	wprintf(L"\t\t┌─────────────────────────────────────┐                                     ┌──────────────────────┐\n");
	wprintf(L"\t\t│ Punti Intelligenza Artificiale: %3d │                                     │ Punti Giocatore: %3d │\n", points[AI_TURN], points[PLAYER_TURN]);
	wprintf(L"\t\t├─────────────────────────────────────┴─────────────────────────────────────┴──────────────────────┤\n");

	drawAiDeck(cardsLeft);

	// -1 = do NOT draw the hands
	if (playerCardPlayed == EMPTY && aiCardPlayed == EMPTY) {
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
	}
	else if (aiCardPlayed == EMPTY) {
		drawPlayerCardPlayed(playerCardPlayed);
	}
	else if (playerCardPlayed == EMPTY) {
		drawAiCardPlayed(aiCardPlayed);
	}
	else {
		drawBothCardsPlayed(aiCardPlayed, playerCardPlayed);
	}

	drawPlayerDeck(cardsLeft);

	wprintf(L"\t\t╘══════════════════════════════════════════════════════════════════════════════════════════════════╛\n");
}

/// <summary>
/// Draws the player's deck
/// </summary>
/// <param name="cardsLeft">the card left in the deck</param>
void drawPlayerDeck(int cardsLeft) {

	if (cardsLeft == 0) {
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
	}
	else {
		wprintf(L"\t\t│                                                                                      \033[94m╔╤╤╤╤╤╤╤╤╤╗\033[0m │\n");
		wprintf(L"\t\t│                                                                                      \033[94m╠╪╪╪╪╪╪╪╪╪╣\033[0m │\n");
		wprintf(L"\t\t│                                                                                      \033[94m╠╪╪╪╪╪╪╪╪╪╣\033[0m │\n");
		wprintf(L"\t\t│                                                                                      \033[94m╠╪╪╪╪╪╪╪╪╪╣\033[0m │\n");
		wprintf(L"\t\t│                                                                                      \033[94m╠╪╪╪╪╪╪╪╪╪╣\033[0m │\n");
		wprintf(L"\t\t│                                                                                      \033[94m╠╪╪╪╪╪╪╪╪╪╣\033[0m │\n");
		wprintf(L"\t\t│                                                                                      \033[94m╚╧╧╧╧╧╧╧╧╧╝\033[0m │\n");
	}
}

/// <summary>
/// Draws the ai's deck
/// </summary>
/// <param name="cardsLeft">the cards left in the deck</param>
void drawAiDeck(int cardsLeft) {

	if (cardsLeft == 0) {
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
		wprintf(L"\t\t│                                                                                                  │\n");
	}
	else {
		wprintf(L"\t\t│ \033[33m╔╤╤╤╤╤╤╤╤╤╗\033[0m                                                                                      │\n");
		wprintf(L"\t\t│ \033[33m╠╪╪╪╪╪╪╪╪╪╣\033[0m                                                                                      │\n");
		wprintf(L"\t\t│ \033[33m╠╪╪╪╪╪╪╪╪╪╣\033[0m                                                                                      │\n");
		wprintf(L"\t\t│ \033[33m╠╪╪╪╪╪╪╪╪╪╣\033[0m                                                                                      │\n");
		wprintf(L"\t\t│ \033[33m╠╪╪╪╪╪╪╪╪╪╣\033[0m                                                                                      │\n");
		wprintf(L"\t\t│ \033[33m╠╪╪╪╪╪╪╪╪╪╣\033[0m                                                                                      │\n");
		wprintf(L"\t\t│ \033[33m╚╧╧╧╧╧╧╧╧╧╝\033[0m                                                                                      │\n");
	}
}


/// <summary>
/// Draws both cards when played
/// </summary>
/// <param name="leftCard">the ai card</param>
/// <param name="rightCard">the player card</param>
void drawBothCardsPlayed(int leftCard, int rightCard) {

	const int wordMaxLength = 7;
	int i;

	wprintf(L"\t\t│                                                                                                  │\n");
	wprintf(L"\t\t│                                 \033[33m╔═════════╗\033[0m          \033[94m╔═════════╗\033[0m                                 │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m          \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m          \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                 \033[33m║ ");

	// Calculating the starting offset from the card border
	int offset = (wordMaxLength - strlen(cardType[leftCard])) / 2;

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	printf("%s", cardType[leftCard]);

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	wprintf(L" ║\033[0m          \033[94m║ ");

	// parola carta player
	offset = (wordMaxLength - strlen(cardType[rightCard])) / 2;

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	printf("%s", cardType[rightCard]);

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	wprintf(L" ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m          \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m          \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                 \033[33m╚═════════╝\033[0m          \033[94m╚═════════╝\033[0m                                 │\n");
}


/// <summary>
/// Draw only the AI's card 
/// </summary>
/// <param name="card">the card played</param>
void drawAiCardPlayed(int card) {
	const int wordMaxLength = 7;
	int i;

	wprintf(L"\t\t│                                                                                                  │\n");
	wprintf(L"\t\t│                                 \033[33m╔═════════╗\033[0m                                                      │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m                                                      │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m                                                      │\n");
	wprintf(L"\t\t│                                 \033[33m║ ");

	// Calculating the starting offset from the card border
	int offset = (wordMaxLength - strlen(cardType[card])) / 2;

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	printf("%s", cardType[card]);

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	wprintf(L" ║\033[0m                                                      │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m                                                      │\n");
	wprintf(L"\t\t│                                 \033[33m║         ║\033[0m                                                      │\n");
	wprintf(L"\t\t│                                 \033[33m╚═════════╝\033[0m                                                      │\n");
}

/// <summary>
/// Draw only the Player's card 
/// </summary>
/// <param name="card">the card played</param>
void drawPlayerCardPlayed(int card) {
	const int wordMaxLength = 7;
	int i;

	wprintf(L"\t\t│                                                                                                  │\n");
	wprintf(L"\t\t│                                                      \033[94m╔═════════╗\033[0m                                 │\n");
	wprintf(L"\t\t│                                                      \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                                      \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                                      \033[94m║ ");

	// Calculating the starting offset from the card border
	int offset = (wordMaxLength - strlen(cardType[card])) / 2;

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	printf("%s", cardType[card]);

	// Prints the offset as spaces
	for (i = 0; i < offset; i++)
		printf(" ");

	wprintf(L" ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                                      \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                                      \033[94m║         ║\033[0m                                 │\n");
	wprintf(L"\t\t│                                                      \033[94m╚═════════╝\033[0m                                 │\n");
}