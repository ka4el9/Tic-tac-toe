#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <random>
#include <chrono>
#include <stdlib.h>


//-----------------------------------------------------------------

enum TCell : char
{
	CROSS = 'X',
	ZERO = '0',
	EMPTY = '-'
};

enum TProgress
{
	IN_PROGRESS,
	WON_HUMAN,
	WON_COMPUTER,
	DRAW
};

struct 	TCoordinates
{
	size_t y;
	size_t x;
};

struct TGAME
{
	TCell **ppField{nullptr};
	const size_t size{3}; //3x3
	TCell human{CROSS};
	TCell computer{ZERO};
	size_t turn{ 0 }; //четные человек, нечетные компьютер
	TProgress progress{IN_PROGRESS};
};


//-----------------------------------------------------------------


void instructions()
{
	std::cout << "Welcome to the ultimate man-machine showdown! Tic-Tac-Toe \n";
	std::cout << "Where human brain is pit against silicon processor! \n\n";
	std::cout << "Prepare yourself, human. The battle is about to begin.\n\n";
}

void askNamePlayer()
{
	std::string name;
	std::cout << "<< Hello, human. A SUPERCOMPYTER is talking to you. >> \n";
	std::cout << "<< Take the fight or leave!!! >> \n";
	std::cout << "<< Tell me your name human!? >> \n";
	std::cin >> name;
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	std::cout << '\n' << "<< HA-HA-HA " << (name) 
	<< "!!! " << "Great! So, will we begin!!! >> \n";
}

//-----------------------------------------------------------------


void clearScr() //очитска экрана
{
	std::cout << "\x1B[2J\x1B[H";
}


int32_t getRandomNum(int32_t min, int32_t max) 
{ 
	const static auto seed = std::chrono::system_clock::now().time_since_epoch().count(); 
	static std::mt19937_64 generator(seed); 
	std::uniform_int_distribution<int32_t> dis(min, max); 
	return dis(generator); 
} 

//-----------------------------------------------------------------


void initGAME(TGAME &g) 
{
	g.ppField = new TCell *[g.size];

	for (size_t i = 0; i < g.size; ++i)
	{
		g.ppField[i] = new TCell[g.size];
	}

	for (size_t y = 0; y < g.size; ++y)
	{
		for (size_t x = 0; x < g.size; ++x)
		{
			g.ppField[y][x] = EMPTY;
		}
	}

	if (getRandomNum(0, 1000) > 500)
	{
		g.human = CROSS;
		g.computer = ZERO;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.computer = CROSS;
		g.turn = 1;
	}
}


void deinitGAME(TGAME &g)
{

	for (size_t i = 0; i < g.size; ++i)
	{
		delete [] g.ppField[i];
	}
	delete [] g.ppField;
	g.ppField = nullptr;
}



void printGame(const TGAME &g) 
{
	std::cout << "    ";
	for (size_t x = 0; x < g.size; ++x)
		std::cout << " " << x + 1 << "  ";
		std::cout << std::endl;

	for (size_t y = 0; y < g.size; ++y)
	{
		std::cout << " " << y + 1 << " | ";

		for (size_t x = 0; x < g.size; ++x)
		{
			std::cout << g.ppField[y][x] << " | " ;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << " Human: " << g.human << std::endl
			<< " Computer: " << g.computer << std::endl;
	std::cout << '\n';
}



TProgress getWon(const TGAME &g)
{	
	//выйгрыш в строках
	for (size_t y = 0; y < g.size; ++y)
	{
		if ((g.ppField[y][0]) == (g.ppField[y][1]) && (g.ppField[y][0]) == (g.ppField[y][2]))
		{
			if (g.ppField[y][0] == g.human)
			{
				return WON_HUMAN;
			}

			if (g.ppField[y][0] == g.computer)
			{
				return WON_COMPUTER;
			}
		}
	}

	//выйгрыш в столцах
	for (size_t x = 0; x < g.size; ++x)
	{
		if ((g.ppField[0][x]) == (g.ppField[1][x]) && (g.ppField[0][x]) == (g.ppField[2][x]))
		{
			if (g.ppField[0][x] == g.human)
			{
				return WON_HUMAN;
			}

			if (g.ppField[0][x] == g.computer)
			{
				return WON_COMPUTER;
			}
		}
	}

	//выйгрыш диагонали
	if ((g.ppField[0][0]) == (g.ppField[1][1]) && (g.ppField[0][0]) == (g.ppField[2][2]))
	{
		if (g.ppField[0][0] == g.human)
		{
			return WON_HUMAN;
		}

		if (g.ppField[0][0] == g.computer)
		{
			return WON_COMPUTER;
		}
	}


	if ((g.ppField[2][0]) == (g.ppField[1][1]) && (g.ppField[1][1]) == (g.ppField[0][2]))
	{
		if (g.ppField[1][1] == g.human)
		{
			return WON_HUMAN;
		}

		if (g.ppField[1][1] == g.computer)
		{
			return WON_COMPUTER;
		}
	}


	//ничья

	bool draw{true};

	for (size_t y = 0; y < g.size; ++y)
		{

			for (size_t x = 0; x < g.size; ++x)
			{
				if (g.ppField[y][x] == EMPTY)
				{
					draw = false;
					break;
				}
			}
			if (!draw)
				break;
		}

	if (draw)
	{
		return DRAW;
	}

	return IN_PROGRESS;
}


TCoordinates getHumanCoord(const TGAME &g)
{
	TCoordinates c;	
	do {
		std::cout << "Enter X (1..3): ";
		std::cin >> c.x;

		std::cout << "Enter Y (1..3): ";
		std::cin >> c.y;
		c.x--;
		c.y--;
	} while (c.x > 2 || c.y > 2 || g.ppField[c.y][c.x] != EMPTY);
	return c;
}


TCoordinates getComputerCoord(TGAME &g)
{
	//1 PRE WIN SITUATION
	for (size_t y = 0; y < g.size; ++y)
	{
		for (size_t x = 0; x < g.size; ++x)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.computer;
				if (getWon(g) == WON_COMPUTER)
				{
					g.ppField[y][x] = EMPTY;
					return { y, x };
				}
				g.ppField[y][x] = EMPTY;
			}	
			
		}
	}

	//2 PRE FAIL SITUATION

	for (size_t y = 0; y < g.size; ++y)
	{
		for (size_t x = 0; x < g.size; ++x)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.human;
				if (getWon(g) == WON_HUMAN)
				{
					g.ppField[y][x] = EMPTY;
					return { y, x };
				}
				g.ppField[y][x] = EMPTY;
			}	
			
		}
	}

	//3 - ходы по приоритетам + рандом по ним
	
	//цент
	if (g.ppField[1][1] == EMPTY)
	{
		return {1, 1};
	}
	
	//углы
	TCoordinates buf[4];
	size_t num{ 0 };
	if (g.ppField[0][0] == EMPTY)
	{
		buf[num] = { 0, 0 };
		num++;
	}
	if (g.ppField[2][2] == EMPTY)
	{
		buf[num] = { 2, 2 };
		num++;
	}
	if (g.ppField[0][2] == EMPTY)
	{
		buf[num] = { 0, 2 };
		num++;
	}
	if (g.ppField[2][0] == EMPTY)
	{
		buf[num] = { 2, 0 };
		num++;
	}
	if (num > 0)
	{
		const size_t index = rand() % num;
		return buf[index];
	}	

	//не углы
	num = 0;
	if (g.ppField[0][1] == EMPTY)
	{
		buf[num] = { 0, 1 };
		num++;
	}
	if (g.ppField[1][0] == EMPTY)
	{
		buf[num] = { 1, 0 };
		num++;
	}
	if (g.ppField[1][2] == EMPTY)
	{
		buf[num] = { 1, 2 };
		num++;
	}
	if (g.ppField[2][1] == EMPTY)
	{
		buf[num] = { 2, 1 };
		num++;
	}

	if (num > 0)
	{	const size_t index = rand() % num;
		return buf[index];
	}

	return { 0, 0 };
}

void congrats(const TGAME &g)
{
	if (g.progress == WON_HUMAN)
		std::cout << "Human won!!! =) \n" << std::endl;
		
	else if (g.progress == WON_COMPUTER)
		std::cout << "SUPERCOMPYTER won!!! =/ \n" << std::endl;
		
	else if (g.progress == DRAW)
		std::cout << "DRAW =( \n" << std::endl;

	std::cout << "The game Tic-Tac-Toe made by Brel \n\n";
}


//-----------------------------------------------------------------

int main(int argc, char const *argv[])
{
	clearScr();
	instructions();
	askNamePlayer();
	
	TGAME g;
	initGAME(g);


	printGame(g);

	do {
		if (g.turn % 2 == 0)
		{
			//ходит человек
			TCoordinates c = getHumanCoord(g);
			g.ppField[c.y][c.x] = g.human;
		}
		else
		{
			//ходит компьютер
			TCoordinates c = getComputerCoord(g);
			g.ppField[c.y][c.x] = g.computer;
		}

		//clearScr();
		printGame(g);
		g.turn++;
		g.progress = getWon(g);

	} while (g.progress == IN_PROGRESS);

	congrats(g);
	deinitGAME(g);
	
}



//-----------------------------------------------------------------

