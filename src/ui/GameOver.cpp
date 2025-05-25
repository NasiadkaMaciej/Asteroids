#include "GameOver.hpp"
#include "../utils/GameSettings.hpp"

extern void setState(eStates state);

GameOver::GameOver()
  : Menu() {
	addItem("Your score: 0", []() { setState(saveScreenState); });

	addItem("New game", []() { setState(playState); });

	addItem("Menu", []() { setState(menuState); });
}

void GameOver::setScore(int points) {
	getItem(0)->setText("Your score: " + std::to_string(points));
}