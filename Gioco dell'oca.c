/*
* Project name: Gioco dell'Oca
* Authors: Macaluso Francesco
* Date: 11.09.2020
* Version: 1.0
* Platform: Visual Studio 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#pragma region _DEBUG_

#define PLAYERS 4

#define RELESE

// Game seed (set to -1 to be random)
const unsigned int seed = -1;

// Defining porbability
const int PushCellPercentage = 40;
const int PullCellPercentage = 40;
const int PushMaxVal = 5;
const int PullMaxVal = 5;

// Defining constants
const int size = 100;

#pragma endregion


const char names[27][15] = { "Harry","Ross",
						"Bruce","Cook",
						"Carolyn","Morgan",
						"Albert","Walker",
						"Randy","Reed",
						"Larry","Barnes",
						"Lois","Wilson",
						"Jesse","Campbell",
						"Ernest","Rogers",
						"Theresa","Patterson",
						"Henry","Simmons",
						"Michelle","Perry",
						"Frank","Butler",
						"Shirley" 
};

// Defining UI methods
void Draw();
void ShowMenu();

// Initialization methods
void InitBoard();
void InitPlayers();

// Logic methods
int RollDice(int numberDice);
int getPlayerPosition(int player);
int CheckWinner();
void FixGridPowerup();
void MovePlayer(int qty, int player);
void AIMove(int AIPlayer);

// Contains players name array
char players[PLAYERS][15];

// Defining board main struct
struct Cell {

	// Powerup that the cell has
	int powerup;

	// Holds all the player in a particular cell
	int playersInCell[PLAYERS];

} board[100];

int main5() {

	// Ask the player for his name 
	InitPlayers();

	// Initializes the board
	InitBoard();

	// Ask the player for moves
	while (1) {
		Draw();
		ShowMenu();
	}

	return 0;
}

void InitPlayers() {
	
	printf("    ______   __                                            __            __  __                                   \n");
	printf("   /      \\ |  \\                                          |  \\          |  \\|  \\                                  \n");
	printf("  |  \033[33m$$$$$$\033[0m\\ \\\033[33m$$\033[0m  ______    _______   ______          ____| \033[33m$$\033[0m  ______  | \033[33m$$\033[0m| \033[33m$$\033[0m        ______    _______ ______  \n");
	printf("  | \033[33m$$\033[0m __\\\033[33m$$\033[0m|  \\ /      \\  /       \\ /      \\        /      \033[33m$$\033[0m /      \\ | \033[33m$$\033[0m| \033[33m$$\033[0m       /      \\  /       \\      \\ \n");
	printf("  | \033[33m$$\033[0m|    \\| \033[33m$$\033[0m|  \033[33m$$$$$$\033[0m\|  \033[33m$$$$$$$\033[0m|  \033[33m$$$$$$\033[0m\\       |  \033[33m$$$$$$$\033[0m|  \033[33m$$$$$$\033[0m\\| \033[33m$$\033[0m| \033[33m$$\033[0m      |  \033[33m$$$$$$\033[0m\\|  \033[33m$$$$$$$\033[0m\\\033[33m$$$$$$\033[0m\\\n");
	printf("  | \033[33m$$\033[0m \\\033[33m$$$$\033[0m| \033[33m$$\033[0m| \033[33m$$\033[0m  | \033[0m\033[33m$$\033[0m| \033[33m$$\033[0m      | \033[33m$$\033[0m  | \033[33m$$\033[0m      | \033[33m$$\033[0m  | \033[33m$$\033[0m| \033[33m$$\033[0m    \033[33m$$\033[0m| \033[33m$$\033[0m| \033[33m$$\033[0m      | \033[33m$$\033[0m  | \033[33m$$\033[0m| \033[33m$$\033[0m     /      \033[33m$$\033[0m\n");
	printf("  | \033[33m$$\033[0m__| \033[33m$$\033[0m| \033[33m$$\033[0m| \033[33m$$\033[0m__/ \033[33m$$\033[0m| \033[33m$$\033[0m_____ | \033[33m$$\033[0m__/ \033[33m$$\033[0m      | \033[33m$$\033[0m__| \033[33m$$| \033[33m$$$$$$$$\033[0m| \033[33m$$\033[0m| \033[33m$$\033[0m      | \033[33m$$\033[0m__/ \033[33m$$\033[0m| \033[33m$$\033[0m____|  \033[33m$$$$$$$\033[0m\n");
	printf("   \\\033[33m$$    $$\033[0m| \033[33m$$\033[0m \\\033[33m$$    \033[33m$$\033[0m \\\033[33m$$\033[0m     \\ \\\033[33m$$\033[0m    \033[33m$$\033[0m       \\\033[33m$$\033[0m    \033[33m$$\033[0m \\\033[33m$$\033[0m     \\| \033[33m$$\033[0m| \033[33m$$\033[0m       \\\033[33m$$\033[0m    \033[33m$$\033[0m \\\033[33m$$\033[0m     \\\033[33m$$\033[0m    \033[33m$$\033[0m\n");
	printf("    \\\033[33m$$$$$$\033[0m  \\\033[33m$$\033[0m  \\\033[33m$$$$$$\033[0m   \\\033[33m$$$$$$$\033[0m  \\\033[33m$$$$$$\033[0m         \\\033[33m$$$$$$$\033[0m  \\\033[33m$$$$$$$\033[0m \\\033[33m$$\033[0m \\\033[33m$$\033[0m        \\\033[33m$$$$$$\033[0m   \\\033[33m$$$$$$$\033[0m\\\033[33m$$$$$$$\033[0m\n");
	printf("                                                                                                                \n");
#ifdef DEBUG
	printf("\t\t\t\t\t\t\t\t\t\t\t\tDebug mode active\n");
#endif
	int oppIndex = 0;
	printf("Inserisci il tuo nome: (max 15 char)\n>");
	fflush(stdin);
	gets(players[0]);

	printf("Benvenuto %s!! Stai per iniziare a giocare al gioco dell'oca!\n", players[0]);

	_getch();

	system("cls");
}

void InitBoard() {

	// Initializes the random seed
	if (seed != -1)
		srand(seed);
	else
		srand(time(NULL));

	// Defines local variable
	int i, j;
	
	// Initializes the cell's status
	for (i = PullMaxVal; i < size; i++) {

		// Spawns the good cells
		if (rand() % 100 < PushCellPercentage) {
			board[i].powerup = rand() % PushMaxVal;
		}

		// Spawns the good cells
		else if (rand() % 100 < PullCellPercentage) {
			board[i].powerup = (rand() % PushMaxVal) * -1;
		}

		// If the cell is not goor nor bad, its neutral
		else {
			board[i].powerup = 0;
		}
	}

	board[99].powerup = 0;

	// Fix possible mistakes made by the initializer
	FixGridPowerup();
	_getch();

	// Initializes all player cell to -1 (empty)
	for (i = 0; i < size; i++) {
		for (j = 0; j < PLAYERS; j++) {
			board[i].playersInCell[j] = -1;
		}
	}

	// Initializes player status in the board
	for (j = 0; j < PLAYERS; j++) {

		// Defining in which cell the game starts
		int startingCell = 0;

		// The first one will always be the user, that is the first item in the list (0)
		int nameIndex = 0;

		// If there is a player without a name, it creates a name for him
		if (players[j][0] == NULL) {

			// Gets a random number to choose the name
			nameIndex = rand() % 27;

			// Sets the random name to the first free position in 'players'
			strcpy_s(players[j], 15, names[nameIndex]);
		}

		// Inserts the name into the board struct
		board[startingCell].playersInCell[j] = j;
	}
}

void ShowMenu() {
	int currentTurn = 0;  // Current turn matches the index of the name of the player

	printf("Azioni:\n");
	printf("- Premi 'r' per lanciare i dadi!\n");
	printf("- Premi 'q' per abbandonare la partita!\n");
#ifdef DEBUG
	printf("- Premi 'd' per feedback di debug\n");
#endif
	printf("\nGiocatori:\n");

	// Printing all players
	int i;
	for (i = 0; i < PLAYERS; i++) {
		printf("- %s (%d)\n", players[i], getPlayerPosition(i));
	}

	while (currentTurn == 0) {
		char input = _getch();
		
		if (input == 'r') {

			// Calculates the dice result
			int val = RollDice(2);

			printf("Hai rollato un %d!\n", val);

			// Moves the player and increases the turn
			MovePlayer(val, currentTurn);

			printf("Sei nella casella numero %d!\n", getPlayerPosition(0));

			// Increases the turn
			currentTurn = NextTurn(currentTurn);
			break;
		}

		// Exits the match
		if (input == 'q') {
			printf("\nIl giocatore %s ha abbandonato la partita!!\n", players[0]);
			exit(0);
		}

#ifdef DEBUG
		// Prints the debug
		if (input == 'd') {
			int i, j;
			for (i = 0; i < 100; i++) {
				printf("Position: %d\t", i);
				for (j = 0; j < PLAYERS; j++) {
					printf("'%s' | ", players[board[i].playersInCell[j]]);
				}

				for (j = 0; j < PLAYERS; j++) {
					printf(" '%d' | ", board[i].playersInCell[j]);
				}
				printf("\n");
			}
		}
#endif
	}

	// AI turns
	while (currentTurn != 0) {
		printf("\nOra e' il turno di %s!\n", players[currentTurn]);
		_getch();

		// Makes the current ai player move
		AIMove(currentTurn);
		currentTurn = NextTurn(currentTurn);
	}
}

int NextTurn(int currentTurn) {
	int nextTurn = currentTurn;

	nextTurn++;

	if (nextTurn == PLAYERS) {
		nextTurn = 0;
	}

	return nextTurn;
}

void Draw() {
	const int columns = 15;
	const int rows = 7;

	int i, j, k, cnt = 0;

	// Clear consoles
	system("cls");

	// Prints the new board
	printf("\n");
	for (i = 0; i < rows; i++) {
		printf("\t");
		for (j = 0; j < columns; j++) {

			printf("%5d(%d)  ", (i * 15) + j + 1, board[cnt].powerup);
			cnt++;

			// When the loop reaches the max cell value it breaks
			if (((i * 15) + j + 1) >= size) {
				break;
			}
		}
		printf("\n\n");
	}
}

int RollDice(int numberDice) {
	int res = 0;
	int i;
	for(i = 0; i < numberDice; i++) {
		res += (rand() % 6) + 1;
	}
	return res;
}

void AIMove(int AIPlayer) {

	// If its the player's turn exits the method
	if (AIPlayer == 0) {
		return;
	}

	int playerPos = getPlayerPosition(AIPlayer);
	int diceRoll = RollDice(2);

	printf("Il giocatore %s ha rollato un %d!\n", players[AIPlayer], diceRoll);

	MovePlayer(diceRoll, AIPlayer);

	printf("Il giocatore %s si e' spostato dalla casella %d alla casella %d!\n", players[AIPlayer], playerPos, getPlayerPosition(AIPlayer));

	_getch();
}

int getPlayerPosition(int player) {
	int i, j;

	// Loops through all 100 cells
	for (i = 0; i < size; i++) {

		// Loops through all the possible dimension of the cell's player array
		for (j = 0; j < PLAYERS; j++) {

			// If the player matches the player in the cell, returns cell index
			if (board[i].playersInCell[j] == player) {
				return i;
			}
		}
	}
}

void MovePlayer(const int qty, const int player) {

	const int playerPos = getPlayerPosition(player);
#ifdef DEBUG
	printf("DEBUG: Starting player position %d\n", playerPos);
#endif
	int i = 0;

	// Calculates the old position in 'players' array and clears the old position
	while (1) {
		if (board[playerPos].playersInCell[i] == player) {
			board[playerPos].playersInCell[i] = -1;
			break;
		}
		else {
			i++;
		}
	}

	i = 0;

	//Player next position
	int destPosition = 0;

	// Calculates the new position in the players array and writes the new position
	while (1) {

		// Checks if the result of the position is bigger than 100 and sets destPosition to a valid position
		if ((playerPos + qty) >= size) {

			/* Example:
			 * CurrentPos = 96
			 * DiceRoll = 10
			 * Valid Result = 94
			 * Board size = 100
			 * Operation = BoardSize - (DiceRoll - (BoardSize - CurrentPos)) == 94
			 */
			destPosition = size - (qty - (size - playerPos));
		}
		else {
			destPosition = playerPos + qty;
		}

		//Checks if the player has won
		if (destPosition == 100) {
			printf("Player %s e' arrivato all ultima casella ed ha vinto la partita!\n\n\n", players[player]);
			exit(0);
		}

		if (board[destPosition].playersInCell[i] == -1) {
			board[destPosition].playersInCell[i] = player;
			break;
		}
		else {
			i++;
		}
	}

	// Checking if the cell has any powerup
	if (board[destPosition - 1].powerup != 0) {
		if (board[destPosition - 1].powerup > 0) {
			printf("%s e' atterrato su una casella %d che lo fa spostare di %d caselle in avanti!\n", players[player], destPosition, abs(board[destPosition - 1].powerup));
		}
		else {
			printf("%s e' atterrato su una casella %d che lo fa spostare di %d caselle indietro!\n", players[player], destPosition, abs(board[destPosition - 1].powerup));
		}

		MovePlayer(board[destPosition - 1].powerup, player);
	}
}

// Fixes the possible loops that can be made by the random powerup generator
// cell: 8; powerup: +3;   cell: 11; powerup: -3; ERROR: endless loop
void FixGridPowerup() {
	int i;
	for (i = 0; i < size; i++) {

		// Checks if the cell has the corrisponding negative powerup
		if ((board[i].powerup == (board[i + board[i].powerup].powerup * -1)) && board[i].powerup != 0)
		{
#ifdef DEBUG 
			printf("DEBUG: The cell %d (pu: %d) is in conflict with the cell %d (pu: %d) --ERROR\n", i, board[i].powerup, i + board[i].powerup, board[i + board[i].powerup].powerup);
#endif
			board[i + board[i].powerup].powerup = 0;
		}
		else {
#ifdef DEBUG 
			printf("DEBUG: The cell %d (pu: %d) is good with cell %d (pu: %d) --OK\n", i, board[i].powerup, i + board[i].powerup, board[i + board[i].powerup].powerup);
#endif
		}
	}
}
int CheckWinner() {
	int i;
	for (i = 0; i < PLAYERS; i++) {
		if (board[size - 1].playersInCell[i] != -1) {
			return i;
		}
	}

	return -1;
}
