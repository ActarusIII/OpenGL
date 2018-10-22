#include <vector>
#include <iostream>
#include "Game.h"


Case::Case(int numero2, int caseSuivante2) : numero(numero2), caseSuivante(caseSuivante2), type(NORMAL) {}

std::vector<Case> Case::createCases(int nombreCases) {
	std::vector<Case> myCases;
	for (int i = 0; i < nombreCases; i++) {
		myCases.push_back(Case(i, i + 1));
	}
	return myCases;
}

Game::Game(int nombre) : nombreCases(nombre) {
	cases = Case::createCases(nombreCases);
	cases.push_back(NullCase);
	for (int i = 0; i < nombre; i++) {
		if (i%5==0) {
			cases[i].type = MEAN;
		}
	}
}


void Player::checkForWins() {
	if (currentCase.caseSuivante==game.nombreCases){
		hasWon = true;
	}
}

void Player::move(int nombre) {
	if (currentCase.numero >= game.cases.size()) {
		currentCase = game.NullCase;
	}
	else if (currentCase.numero < 0) {
		currentCase = game.cases[0];
	}
	else {
		currentCase = game.cases[currentCase.numero + nombre];
	}
}