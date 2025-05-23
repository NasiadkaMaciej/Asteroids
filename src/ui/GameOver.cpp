#include "GameOver.hpp"
#include "../utils/GameSettings.hpp"

extern void setState(eStates state);

GameOver::GameOver(int entriesCount, std::string entries[])
  : Menu(entriesCount, entries) {}

void GameOver::click() {
	switch (activeEntry) {
	case 0:
		setState(saveScreenState);
		break;
	case 1:
		setState(playState);
		break;
	case 2:
		setState(menuState);
		break;
	}
}

void GameOver::setScore(int points) {
	entries[0] = "Your score " + std::to_string(points);
	move(0);
}