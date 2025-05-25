#include "LeaderBoard.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "MenuUtils.hpp"

extern void setState(eStates state);
extern ScoreBoard scoreBoard[10];

LeaderBoard::LeaderBoard()
  : Menu() {
	for (int i = 0; i < 10; i++)
		addItem(scoreBoard[i].toString());

	addItem("Global leaderboard", []() { openInBrowser("https://nasiadka.pl/asteroids/"); });

	addItem("Menu", []() { setState(menuState); });
}

void LeaderBoard::update() {
	Menu::update();

	for (int i = 0; i < 10; i++)
		getItem(i)->setText(scoreBoard[i].toString());

	if (CONTROL::isESC() && delta->Menu > 100) {
		setState(menuState);
		delta->Menu = 0;
	}
}