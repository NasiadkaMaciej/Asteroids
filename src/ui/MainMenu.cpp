#include "MainMenu.hpp"
#include "../utils/GameSettings.hpp"
#include "MenuUtils.hpp"

extern void setState(eStates state);

MainMenu::MainMenu()
  : Menu() {
	addItem("Play", []() { setState(playState); });
	addItem("Leaderboard", []() { setState(leaderBoardState); });
	addItem("Settings", []() { setState(settingsState); });
	addItem("Info", []() { openInBrowser("https://github.com/NasiadkaMaciej/Asteroids"); });
	addItem("Exit", []() { exit(0); });
}