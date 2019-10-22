#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
#include <ctype.h>
#include <Windows.h>
#include <cstring>

struct shipCounter
{
	int aircraftCarrier = 5;
	int battleship = 4;
	int destroyer = 3;
	int submarine = 3;
	int patrolBoat = 2;
};

int menu();
int play(int _shots);
void rules();
void Place(char _shipGrid[10][10]);
void getInput(char _input[]);
bool checkShot(char _shipGrid[10][10], char _displayGrid[10][10], int _x, int _y, shipCounter *_counter);
void showGrid(char _Grid[10][10], char _shipGrid[10][10], shipCounter _counter);
void initMap(char _Grid[][10]);
bool placeHorrizontal(char _shipGrid[10][10], char _symbolOfShip, int _sizeOfShip, int _x, int _y);
bool placeVertical(char _shipGrid[10][10], char _symbolOfShip, int _sizeOfShip, int _x, int _y);

int main()
{
	menu();
	return 0;
}

int menu()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char Input = 0;
	do {
		system("CLS");
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "-----------------------------------------------BATTLESHIP------------------------------------------------------------\n";
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "1. Play\n" << "2. Rules\n" << "3. Demo Mode (Infinite Shots) \n";
		std::cin.get(Input);
		std::cin.ignore(100, '\n');
		if (Input == '1')
		{ 
			play(40); 
			return 0;
		}
		else if (Input == '2')
		{ 
			rules();			
		}
		else if (Input == '3')
		{
			play(41); //if the no. of shots are > 40, they won't decreased
			return 0;
		}
	} while (Input != 2);
}

int play(int _shots)
{
	//setup
	system("CLS"); //clears the console window (windows lib)
	std::cout << "Loading...";
	shipCounter counter;
	char shipGrid[10][10];
	initMap(shipGrid);
	char displayGrid[10][10];
	initMap(displayGrid);
	int noOfShots = _shots;
	Place(shipGrid);
	system("CLS");
	char userInput[100] = { "" };
	bool hasWon = false;
	//game
	while (noOfShots > 0)
	{

		std::cout << "\n\n\n";
		showGrid(displayGrid, shipGrid, counter);
		bool hadturn = false;
		while (hadturn == false)
		{
			if (noOfShots < 41)
			{
				std::cout << "\nShots Left: " << noOfShots << std::endl;
			}
			std::cout << "\nPlease insert your Y and X coordinates (e.g. F9): ";
			int x = 0, y = 0;
			userInput[0] = 0; //empty the cstring
			getInput(userInput);
			y = (userInput[0] - 65); //ascii value - offset
 			x = (strlen(userInput) == 3) ? x = 9  : x = ((userInput[1] - 48) - 1);
			hadturn = checkShot(shipGrid, displayGrid, x, y, &counter); //hadturn() returns true if the player input is valid
		}
		if (counter.aircraftCarrier == 0 && counter.battleship == 0 && counter.destroyer == 0 && counter.patrolBoat == 0 && counter.submarine == 0)
		{
			hasWon = true;
			noOfShots = 0;

		}
		if (_shots < 41)
		{
			noOfShots--;
		}
	}
	if (hasWon)
	{
		std::cout << "\n\n\nCongratulations, you win!";
		std::cout << "\n\nPress enter to exit the program...\n";
		std::cin.get();
		
	}
	else if (!hasWon)
	{
		std::cout << "\nYou Lost!\n";
		counter.aircraftCarrier = 0;
		counter.battleship = 0;
		counter.destroyer = 0;
		counter.patrolBoat = 0;
		counter.submarine = 0;
		std::cout << "Here is where the ships were placed: \n";
		showGrid(shipGrid, shipGrid, counter);
		std::cout << "\n\nPress enter to exit the program...\n";
		std::cin.get();
				
	}
	return 0;

}

void rules()
{
	
	system("CLS");
	std::cout << "The following ships are placed at random positions on the board:\n";
	std::cout << "1 Aircraft carrier, size 5.\n";
	std::cout << "1 Battleship, size 4.\n";
	std::cout << "1 Destroyer, size 3. \n";
	std::cout << "1 Submarine, size 3.\n";
	std::cout << "1 Patrol Boat, size 2.\n";
	std::cout << "You have 40 shots to destroy all the ships on the board.\n";
	std::cout << "Each turn you have to insert the coordinates for where on the board you want to shoot.\n\n\n";
	std::cout << "Press Enter to continue...\n";
	std::cin.get();

			
	
}

void Place(char _shipGrid[10][10])
{
	srand(time(NULL));
	// initialise that char in each slot
	int x = 0;
	int y = 0;
	int sizeOfShip = 0;
	bool placed = true;
	int horrizontal = 0;
	char symbolOfShip = 0;

	for (int i = 1; i <= 5;)
	{
		x = rand()%10;
		y = rand()%10;
		placed = true;
		horrizontal = rand() % 10; //<5 true, >5 false;

		switch (i)
		{
		case 1: 
			symbolOfShip = 'A';
			sizeOfShip = 5; 
			break;
		case 2:
			symbolOfShip = 'B';
			sizeOfShip = 4;
			break;
		case 3:
			symbolOfShip = 'P';
			sizeOfShip = 2;
			break;
		case 4:
			symbolOfShip = 'D';
			sizeOfShip = 3;
			break;
		case 5:
			symbolOfShip = 'S';
			sizeOfShip = 3;
			break;
		}

		if (horrizontal<=5)
		{
			placed = placeHorrizontal(_shipGrid, symbolOfShip, sizeOfShip, x, y);
			if (placed == true)
			{
				i++;
			}
		}
		else //vertical
		{
			placed = placeVertical(_shipGrid, symbolOfShip, sizeOfShip, x, y);
			if (placed == true)
			{
				i++;
			}
		}

	}
}

void getInput(char _input[])
{
	bool flag = false;
	while (flag == false)
	{
		std::cin.getline(_input, 1000, '\n');
		int  j = 0;
		for (int i = 0; i < strlen(_input); i++) //trim white space
		{
			if (_input[i] != ' ')
			{
				_input[j] = _input[i];
				j++;
			}
		}
		_input[j + 1] = '\0';
		if (strlen(_input) < 4 && strlen(_input) > 1)
		{
			if (_input[0] > 96 && _input[0] < 122) // lowercase to capital
			{
				_input[0] = _input[0] - 32;
			}
			if (_input[0] > 64 && _input[0] < 75) //check letter
			{
				if (_input[1] > 48 && _input[1] < 58) //Check number
				{
					if (strlen(_input) == 3 && _input[2] == 48) //only checks if its 0
					{
						flag = true;
					}
					else if (strlen(_input) == 2)
					{
						flag = true;
					}
				}
			}
		}
		if (flag == false)
		{
			std::cout << "Invalid input, please try again. \n";
		}
	}
}

bool checkShot(char _shipGrid[10][10], char _displayGrid[10][10], int _x, int _y, shipCounter *_counter)
{
	
	char shipSymbol = 0;
	bool hadTurn = false;
	if (_displayGrid[_y][_x] != 'H' && _displayGrid[_y][_x] != 'X') //check if the player has not called this coordinates before
	{
		system("CLS");
		if (_shipGrid[_y][_x] != ' ') //check if its a hit
		{		
			shipSymbol = _shipGrid[_y][_x];
			std::cout << "Hit! ";
			switch (_shipGrid[_y][_x]) //check what the player has hit
			{
			case 'A':
				(*_counter).aircraftCarrier -= 1;
				if ((*_counter).aircraftCarrier == 0)
				{
					std::cout << "\nYou destroyed my Aircraft Carrier";
				};
				break;

			case 'B':
				(*_counter).battleship -= 1;
				if ((*_counter).battleship == 0)
				{
					std::cout << "\nYou destroyed my Battleship";
				}
				break;

			case 'D':
				(*_counter).destroyer -= 1;
				if ((*_counter).destroyer == 0)
				{
					std::cout << "\nYou destroyed my Destroyer";
				}
				break;

			case 'P':
				(*_counter).patrolBoat -= 1;
				if ((*_counter).patrolBoat == 0)
				{
					std::cout << "\nYou destroyed my Patrol Boat";
				}
				break;

			case 'S':
				(*_counter).submarine -= 1;
				if ((*_counter).submarine == 0)
				{
					std::cout << "\nYou destroyed my Submarine";
				}
				break;

			}
			_displayGrid[_y][_x] = 'H'; 
		}
		else
		{
			std::cout << "Missed!";
			_displayGrid[_y][_x] = 'X'; 
		}
		hadTurn = true;
	}
	else
	{
		std::cout << " Invalid input (you selected a slot outside the grid or a slot where you already shot)";
	}
	return hadTurn;
}

void showGrid(char _Grid[10][10], char _shipGrid[10][10], shipCounter _counter)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << std::setw(11) << 1;
	for (int i = 2; i <= 10; i++)
	{
		std::cout << std::setw(10) << i;
	}
	std::cout << "\n \n";
	for (int i = 0; i < 10; i++)
	{
		std::cout << (char)(65 + i);
		for (int j = 0; j < 10; j++)
		{
			if (_Grid[i][j] == ' ')
			{
				std::cout << std::setw(10) <<'*';
			}
			else if (_Grid[i][j] == 'H' )
			{
				SetConsoleTextAttribute(hConsole, 6);
				if (_shipGrid[i][j] == 'A' && _counter.aircraftCarrier == 0)
				{
					std::cout << std::setw(10) << 'A';
				}
				else if (_shipGrid[i][j] == 'B' && _counter.battleship == 0)
				{
					std::cout << std::setw(10) << 'B';
				}
				else if (_shipGrid[i][j] == 'P' && _counter.patrolBoat == 0)
				{
					std::cout << std::setw(10) << 'P';
				}
				else if (_shipGrid[i][j] == 'D' && _counter.destroyer == 0)
				{
					std::cout << std::setw(10) << 'D';
				}
				else if (_shipGrid[i][j] == 'S' && _counter.submarine == 0)
				{
					std::cout << std::setw(10) << 'S';
				}
				else 
				{
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << std::setw(10) << _Grid[i][j];		
				}
				SetConsoleTextAttribute(hConsole, 7);
			}
			else if (_Grid[i][j] == 'X')
			{
				SetConsoleTextAttribute(hConsole, 12);
				std::cout << std::setw(10) << _Grid[i][j];
				SetConsoleTextAttribute(hConsole, 7);
			}
			else
			{
				std::cout << std::setw(10) << _Grid[i][j];
			}
		}
		std::cout << "\n \n";
	}
}

bool placeHorrizontal(char _shipGrid[10][10], char _symbolOfShip, int _sizeOfShip, int _x, int _y)
{
	srand(time(NULL));
	bool canPlaceLeft = true;
	bool canPlaceRight = true;
	bool hasPlaced = true;
	if ((_x + _sizeOfShip <= 10))
	{
		for (int i = 0; i < _sizeOfShip; i++)
		{

			if (_shipGrid[_y][_x + i] != ' ') // if its !empty
			{
				canPlaceRight = false;
			}
		}
	}
	else { canPlaceRight = false; }
	if ((_x + _sizeOfShip >= 0))
	{
		for (int i = 0; i < _sizeOfShip; i++)
		{
			if (_shipGrid[_y][_x - i] != ' ') // if its !empty
			{
				canPlaceLeft = false;
			}
		}
	}
	else { canPlaceLeft = false; }
	if (canPlaceRight && canPlaceLeft)
	{
		int temp = rand() % 10;
		if (temp <= 5)
		{
			canPlaceLeft = false;
		}
	}
	if (canPlaceRight)
	{
		for (int i = 0; i < _sizeOfShip; i++)
		{
			_shipGrid[_y] [_x + i] = _symbolOfShip;
		}
	}
	else if (canPlaceLeft)
	{
		for (int i = 0; i < _sizeOfShip; i++)
		{
			_shipGrid[_y] [_x - i] = _symbolOfShip;
		}
	}
	else if (!(canPlaceRight && canPlaceLeft))
	{
		hasPlaced = false;
	}

	return hasPlaced;
}

bool placeVertical(char _shipGrid[10][10], char _symbolOfShip, int _sizeOfShip, int _x, int _y)
{
	srand(time(NULL));
	bool canPlaceUp = true;
	bool canPlaceDown = true;
	bool hasPlaced = true;

	for (int i = 0; i < _sizeOfShip; i++)
	{

		if (_shipGrid[_y - i][_x] != ' ') // if its !empty
		{
			canPlaceUp = false;
		}
		if (_shipGrid[_y + i][_x] != ' ')
		{
			canPlaceDown = false;
		}
	}
	if (canPlaceDown && canPlaceUp)
	{
		int temp = rand() % 10;
		if (temp <= 5)
		{
			canPlaceUp = false;
		}
	}
	if (canPlaceUp)
	{
		for (int i = 0; i < _sizeOfShip; i++)
		{
			_shipGrid[_y - i][_x] = _symbolOfShip;
		}
	}
	else if (canPlaceDown)
	{
		for (int i = 0; i < _sizeOfShip; i++)
		{
			_shipGrid[_y + i][_x] = _symbolOfShip;
		}
	}
	else if(!(canPlaceDown && canPlaceUp))
	{
		hasPlaced = false;
	}
	return hasPlaced;
}

void initMap(char _Grid[][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_Grid[i][j] = ' ';
		}
	}
}
