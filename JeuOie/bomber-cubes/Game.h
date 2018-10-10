#include <vector>
#include <iostream>


enum TypeCase{
	NORMAL, MEAN
};

class Case {
public:
	TypeCase type;
	int numero;
	int caseSuivante;
	Case(int numero2, int caseSuivante2);
	static std::vector<Case> createCases(int nombreCases);
};

class Game {
public:
	int nombreCases;
	std::vector<Case> cases;
	Case NullCase = Case(nombreCases, -1); //Valeur arbitraire de -1
	Game(int nombre);
};

class Player {
public:
	Game game;
	Case currentCase;
	bool hasWon;
	Player(Game myGame) : game(myGame), currentCase(myGame.cases[0]), hasWon(false) {}
	void checkForWins();
	void move(int nombre);
}; 

