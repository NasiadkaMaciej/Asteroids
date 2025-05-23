#include "LeaderBoard.hpp"
#include "../utils/GameSettings.hpp"
#include "MenuUtils.hpp"

extern void setState(eStates state);
extern ScoreBoard scoreBoard[10];

LeaderBoard::LeaderBoard(int entriesCount, std::string entries[])
  : GameOver(entriesCount, entries) {}

void LeaderBoard::click() {
	switch (activeEntry) {
	case 10:
		openInBrowser("https://nasiadka.pl/asteroids/");
		break;
	case 11:
		setState(menuState);
		break;
	}
}

void LeaderBoard::setScore() {
	for (int i = 0; i < 10; i++)
		entries[i] = scoreBoard[i].toString();
	entries[10] = "Global leaderboard";
	entries[11] = "Menu";
	move(0);
}