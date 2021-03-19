/*
* Project name: Convertitore Basi
* Authors: Macaluso Francesco
* Date: 12.27.2020
* Version: 1.0
* Platform: Visual Studio 2019
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Prints the output based on the base
void PrintConversion(const char value[], int base);
int checkInputBase(const char number[], int base);
int myAtoi(const char str[]);

// Defining function to decimal
int HexToDec(const char Base16Value[]);
int OctToDec(const char Base8Value[]);
int BinToDec(const char Base2Value[]);
int RomToDec(const char BaseRomValue[]);

// Defining function to binary / roman number
void DecToBin(const char Base10Value[], char* output);
void DecToRom(const char BaseRomValue[], char* output);

int main1() {

	while (1) {
		
		char value[32];
		char _base[3];
		int base;

		printf("Convertitore di sistema di numerazione\n");
		printf("Scegli una base: (2, 8, 10, 16)\n");
		printf("2 - Binario\n");
		printf("8 - Ottale\n");
		printf("10 - Decimale\n");
		printf("16 - Esadecimale\n");
		printf("0  - Romano\n\n");
		printf("-1 - Esci\n");

		// Converting the base from char[] to int
		gets(_base);
		base = myAtoi(_base);

		// If the user wants to quit the application
		if (base == -1) return 0;

		fflush(stdin);
		printf("Inserisci il valore (usa la base scelta): \n");
		gets(value);

		printf("I risultati della conversione di %s sono:\n", value);

		PrintConversion(value, base);
	}
}

/* ------------------ Bin to Dec ------------------ */

/// <summary>
/// Converts the binary number to integer.
/// </summary>
/// <param name="Base2Value">The binary string you want to convert.</param>
/// <returns>The integer value of the binary string.</returns>
int BinToDec(const char Base2Value[]) {
	int result = 0, i;
	int length = strlen(Base2Value);

	// Calculates the decimal value
	for (i = 0; i < length; i++) {
		// Casting the character to string
		char charVal[] = { Base2Value[(length - 1) - i], '\0' };

		result += myAtoi(charVal) * pow(2, i);
	}

	return result;
}

/* ------------------ Oct to Dec ------------------ */

/// <summary>
/// Converts the octal number to integer.
/// </summary>
/// <param name="Base2Value">The octal string you want to convert.</param>
/// <returns>The integer value of the octal string.</returns>
int OctToDec(const char Base8Value[]) {
	int result = 0, i;
	int length = strlen(Base8Value);

	// Calculates the decimal value
	for (i = 0; i < length; i++) {
		// Casting the character to string
		char charVal[] = { Base8Value[(length - 1) - i], '\0' };

		result += myAtoi(charVal) * pow(8, i);
	}

	return result;
}

/* ------------------ Hex to Dec ------------------ */

/// <summary>
/// Converts the hexadecimal number to integer.
/// </summary>
/// <param name="Base2Value">The hexadecimal string you want to convert.</param>
/// <returns>The integer value of the hexadecimal string.</returns>
int HexToDec(const char Base16Value[]) {
	int res = 0;
	int i;
	int length = strlen(Base16Value);

	for (i = length - 1; i >= 0; i--) {

		// If the number is a letter
		if (isalpha(Base16Value[i])) {

			double charVal = 0;

			// Converts the char to an uppercase char
			char c = toupper(Base16Value[i]);

			// Calculates the value subtracting the lowest letter value 10 = (10 + 'A' - 'A'), 13 = (10 + 'D' - 'A')
			charVal = 10.0 + (c -'A');

			// The current value of that char in that position is made by the char value itself multiplied by the position value
			// F1 = (   F(15) * 16(base) ^ 1(position) + 1 * 16(base) ^ 0(position) ) 
			res += charVal * pow(16.0, (double)length - (i + 1.0));
		}
		else {
			char charVal[] = { Base16Value[i] , '\0' };
			res += myAtoi(charVal) * pow(16.0, (double)length - (i + 1.0));
		}
	}

	return res;
}

/* ------------------ Dec to Bin ------------------ */

/// <summary>
/// Converts a decimal number to binary string.
/// </summary>
/// <param name="Base2Value">The decimal you want to convert to binary.</param>
/// <returns>The binary value of decimal.</returns>
void DecToBin(const char Base10Value[], char* output) {
	
	int tmp[32];

	unsigned int val = myAtoi(Base10Value);
	int length = 0;
    int i;

	while (val != 0) {
		tmp[length] = ((val % 2) + (int)'0');
		val /= 2;
		length++;
	}

	tmp[length] = '\0';

	for (i = 0; i < length; i++)
		output[i] = tmp[(length - 1) - i];

	output[length] = '\0';
}

/// <summary>
/// Converts a number from base 10 to roman digits.
/// </summary>
/// <param name="Base10Value">The number you want to convert</param>
/// <param name="output">The value in roman digits.</param>
void DecToRom(const char Base10Value[], char* output) {

	// Converting string to number
	unsigned int val = myAtoi(Base10Value);

	// Defining dictionary
	char romanDigitsLetter[][3] = { "I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M" };
	int romanDigitsValue[] = { 1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000 };

	int current;
	int index = 0;

	// The loop doesnt stop until the value is 0
	while (val != 0) {

		// Searches for the biggest roman value that is smaller than 'val'
		for (current = 12; current >= 0; current--) {
			if (val >= romanDigitsValue[current]) {

				// it removes the value from 'val'
				val -= romanDigitsValue[current];

				// Writes the characters to the string
				int j;
				for (j = 0; j < strlen(romanDigitsLetter[current]); j++) {
					output[index] = romanDigitsLetter[current][j];
					index++;
				}

				//Breaks the loop
				break;
			}
		}
	}

	// Appends EOS
	output[index] = '\0';
}

int RomToDec(const char BaseRomValue[]) {

	// Defining dictionary
	char romanDigitsLetter[] = { 'I', 'V', 'X', 'L', 'C', 'D', 'M' };
	int romanDigitsValue[] = { 1, 5, 10, 50, 100, 500, 1000 };
	int result = 0;

	int i;

	// Checks if the sring is valid
	for (i = 0; i < strlen(BaseRomValue); i++) {

		int j, isLegal = 1;

		for (j = 6; j >= 0; j--) {
			if (BaseRomValue[i] == romanDigitsLetter[j]) {
				isLegal = 0;
				break;
			}
		}

		if (isLegal == 1) {
			return -1;
		}
	}

	// Loops throughout the string
	for (i = 0; i < strlen(BaseRomValue); i++) {

		int j, nextIndex;
		
		// -1 is assigned as end of string
		int currentVal = 0, nextVal = 0;
		
		// Calculates the current digit value
		for (j = 0; j < 7; j++) {
			if (BaseRomValue[i] == romanDigitsLetter[j]) {
				currentVal = romanDigitsValue[j];
				break;
			}
		}

		// Calculates the next digit value
		for (j = 0; j < 7; j++) {
			if (BaseRomValue[i + 1] == romanDigitsLetter[j]) {
				nextVal = romanDigitsValue[j];
				break;
			}
		}

		if (currentVal < nextVal) {

			//Increasing the index by one since we've counted 2 digits
			i++;

			// Setting the result
			result += (nextVal - currentVal);
		}
		else {

			// Setting the result
			result += currentVal;
		}
	}

	printf("Result: %d\n", result);
	return result;
}

/// <summary>
/// Prints an output based on the given base.
/// </summary>
/// <param name="base">The base you want to find an output for.</param>
void PrintConversion(char value[], int base) {
	switch (base) {

		// Roman format
		case 0: 
		{
			char binaryResult[32];				// Stores the binary result
			char romanResult[32];				// Stores the roman result

			// Stores the decimal result both as char[] and int, so it can be used in DecToBin()
			char decimalResChar[32];
			int decimalResult = RomToDec(value);

			// Checks if an error occurred
			if (decimalResult == -1) {
				printf("La stringa %s contiene un carattere non valido nel sistema di numerazione romano!\n", value);
				gets();
				break;
			}

			// Initializes the decimal character variable
			sprintf_s(decimalResChar, 32, "%d", decimalResult);

			//Saves into the result of the conversion to binary
			DecToBin(decimalResChar, binaryResult);

			printf("Binario: %s\n", binaryResult);
			printf("Ottale: %o\n", decimalResult); // %o automatically convers the value to octal 
			printf("Decimale: %d\n", decimalResult);
			printf("Esadecimale: %X\n", decimalResult); // %X automatically convers the value to hexadecimal 
			printf("Romano: %s\n\n", value);

			gets();
			fflush(stdin);
			break;
		}
		case 2:
		{
			// Checks if the format is correct
			if (checkInputBase(value, base) != 0) {
				printf("Il numero %s non e' valido con la base %d!\n", value, base);
				gets();
				break;
			}

			char binaryResult[32];				// Stores the binary result
			char romanResult[32];				// Stores the roman result

			// Stores the decimal result both as char[] and int, so it can be used in DecToBin()
			char decimalResChar[32];
			int decimalResult = BinToDec(value);

			// Initializes the decimal character variable
			sprintf_s(decimalResChar, 32, "%d", decimalResult);

			// Converting to roman
			DecToRom(decimalResChar, romanResult);

			printf("Binario: %s\n", value);
			printf("Ottale: %o\n", decimalResult); // %o automatically convers the value to octal 
			printf("Decimale: %d\n", decimalResult);
			printf("Esadecimale: %X\n", decimalResult); // %X automatically convers the value to hexadecimal 
			printf("Romano: %s\n\n", romanResult);

			gets();
			fflush(stdin);
			break;
		}
		case 8:
		{
			// Checks if the format is correct
			if (checkInputBase(value, base) != 0) {
				printf("Il numero %s non e' valido con la base %d!\n", value, base);
				gets();
				break;
			}

			char binaryResult[32];				// Stores the binary result
			char romanResult[32];				// Stores the roman result

			// Stores the decimal result both as char[] and int, so it can be used in DecToBin()
			char decimalResChar[32];
			int decimalResult = OctToDec(value);

			// Initializes the decimal character variable
			sprintf_s(decimalResChar, 32, "%d", decimalResult);

			//Saves into the result of the conversion to binary
			DecToBin(decimalResChar, binaryResult);

			//Saves into the result of the conversion to roman
			DecToRom(decimalResChar, romanResult);

			printf("Binario: %s\n", binaryResult);
			printf("Ottale: %o\n", decimalResult); // %o automatically convers the value to octal 
			printf("Decimale: %d\n", decimalResult);
			printf("Esadecimale: %X\n", decimalResult); // %X automatically convers the value to hexadecimal 
			printf("Romano: %s\n\n", romanResult);

			gets();
			fflush(stdin);
			break;
		}
		case 10:
		{
			// Checks if the format is correct
			if (checkInputBase(value, base) != 0) {
				printf("Il numero %s non e' valido con la base %d!\n", value, base);
				gets();
				break;
			}

			char binaryResult[32];			// Stores the binary result
			char romanResult[32];			// Stores the roman result

			// Stores the decimal result both as char[] and int, so it can be used in DecToBin()
			int decimalResult = myAtoi(value);

			//Saves into the result of the conversion to binary
			DecToBin(value, binaryResult);

			//Saves into the result of the conversion to roman
			DecToRom(value, romanResult);

			printf("Binario: %s\n", binaryResult);
			printf("Ottale: %o\n", decimalResult); // %o automatically convers the value to octal 
			printf("Decimale: %d\n", decimalResult);
			printf("Esadecimale: %X\n", decimalResult); // %X automatically convers the value to hexadecimal 
			printf("Romano: %s\n\n", romanResult);

			gets();
			fflush(stdin);
			break;
		}
		case 16:
		{
			// Checks if the format is correct
			if (checkInputBase(value, base) != 0) {
				printf("Il numero %s non e' valido con la base %d!\n", value, base);
				gets();
				break;
			}

			char binaryResult[32];			// Stores the binary result
			char romanResult[32];			// Stores the roman result

			// Stores the decimal result both as char[] and int, so it can be used in DecToBin()
			char decimalResChar[32];
			int decimalResult = HexToDec(value);
			
			// Initializes the decimal character variable
			sprintf_s(decimalResChar, 32, "%d", decimalResult);

			//Saves into the result of the conversion to binary
			DecToBin(decimalResChar, binaryResult);

			//Saves into the result of the conversion to roman
			DecToRom(decimalResChar, romanResult);

			printf("Binario: %s\n", binaryResult);
			printf("Ottale: %o\n", decimalResult); // %o automatically convers the value to octal 
			printf("Decimale: %d\n", decimalResult);
			printf("Esadecimale: %X\n", decimalResult); // %X automatically convers the value to hexadecimal 
			printf("Romano: %s\n\n", romanResult);

			gets();
			fflush(stdin);
			break;
		}
		default:
			printf("Scegli una base valida!\n");
			gets();
			break;
	}
}

/// <summary>
/// Checks if a number match with the given base.
/// </summary>
/// <param name="number">The number in char[] format.</param>
/// <param name="base">The base of the number.</param>
/// <returns>Returns 0 if no error were met, 1 if there are errors in the input.</returns>
int checkInputBase(const char number[], int base) {

	int i = 0, length = 0;

	// Calculates the length
	for (length = 0; number[length] != '\0'; length++);

	// If the base is roman then there cannot be digits in the input
	for (i = 0; i < length; i++) {
		if (isdigit(number[i]) && base == 0) {
			return 1;
		}
	}

	for (i = 0; i < length; i++) {

		// Casting char to string so myAtoi works
		char charVal[] = { number[i], '\0' };

		if (base == 16 && isalpha(number[i]) && (number[i] < 65 || number[i] > 70))
		{
			printf("the char '%c'(%d) is not into the rage of 65 and 70!\n", number[i], number[i]);
			return 1;
		}
		
		if (myAtoi(charVal) >= base)
			return 1;
	}
			

	return 0;
}

/// <summary>
/// Convert a string to an integer.
/// </summary>
/// <param name="str">The number you want to convert.</param>
/// <returns>Returns the converted integer.</returns>
int myAtoi(const char str[]) {

	int i = 0, output = 0;
	int sign = 1;
	int length;

	// Calculates the length and checks for string integrity
	for (length = 0; str[length] != '\0'; length++) {

		// Checks if any of the values are letters
		if (isalpha(str[length]))
			if (str[length] != '-')
				return 0;
	}

	// Checks if the number is long 0
	if (length == 0)
		return 0;
	
	// Checks for the sign
	if (str[i] == '-') {
		sign = -1;
		i++;
	}

	for (; i < length; i++) {
		output = (output * 10) + str[i] - '0';
	}

	return output * sign;
}

/*
	unsigned int val = myAtoi(BaseRomValue);

	int outputIndex = 0;

	while (val != 0) {
		int max = 0;
		int rest = 1;
		int current;

		// Checking if the number can be subtracted to any of the roman values
		// from the bigger to the smallest
		for (current = 6; current >= 0; current--) {

			// Checks what is the biggest roman number that not overflows the value
			if (val >= romanDigitsValue[current]) {

				// Checks for the operation (subtraction or addition)
				// If the index goes out of range, it returns the current current index
				int previous = (current - 1 == -1) ? 0 : (current - 1);
				int next = (current + 1 == 8) ? 7 : (current + 1);

				// Max value that can be made with 3 digits (ex. CCC -> 300 then goes for CLLL('C' index - 1 = 'L')
				// 960
				if ((romanDigitsValue[current] + (romanDigitsValue[previous] * 3)) <= val) {
					// If the current letter's value multiplied by 3 is still less than the value that the user has inserted
					// a subtraction will be made
					int diff = romanDigitsValue[next] - val;

				}
				else {

				}
				break;
			}
			else {
				printf("No!\n");
			}
		}
	}
	output[outputIndex] = '\0';


*/
