#pragma once
#include <algorithm>
#include <fstream>
#include <string>

struct ScoreBoard {
	unsigned int points;
	std::string name;
	std::string toString();
};

extern ScoreBoard scoreBoard[10];

void writeScoreBoard();
void loadScoreBoard();
void openInBrowser(const std::string& url);
std::string returnBool(int value);