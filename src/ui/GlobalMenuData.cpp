#include "GlobalMenuData.hpp"
#include "../utils/GameSettings.hpp"
#include "MenuUtils.hpp"

const int menuEntriesCount = 5;
const int gameOverEntriesCount = 3;
const int settingEntriesCount = 9;
const int saveScoreEntriesCount = 13;
const int leaderBoardEntriesCount = 12;

std::string menuEntries[menuEntriesCount]{ "Play", "Leaderboard", "Settings", "Info", "Exit" };

std::string gameOverEntries[gameOverEntriesCount]{ "Your score", "New game", "Menu" };

std::string settingEntries[settingEntriesCount]{ "Frame rate limit: 60",
												 "VSync: On",
												 "Fullscreen: On",
												 "Sound: On",
												 "Music: On",
												 "Background: On",
												 "Resolution: 1920x1080",
												 "Antialiasing: 0",
												 "Menu" };

std::string saveScoreEntries[saveScoreEntriesCount];
std::string leaderBoardEntries[leaderBoardEntriesCount];