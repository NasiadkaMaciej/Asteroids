#include "MenuUtils.hpp"
#include <iostream>

#ifdef _WIN32
static int platform = 1;
#elif _WIN64
static int platform = 1;
#elif __linux__
static int platform = 2;
#elif __APPLE__
static int platform = 3;
#else
static int platform = 0;
#endif

ScoreBoard scoreBoard[10];

std::string ScoreBoard::toString() {
	return std::to_string(points) + " - " + name;
}

// Write actual score board to file
void writeScoreBoard() {
	std::ofstream file("scoreBoard.dat");
	if (file.is_open()) {
		for (int i = 0; i < 10; i++) {
			file << scoreBoard[i].points << ":";
			file << scoreBoard[i].name;
			if (i < 9) file << "\n";
		}
	}
	file.close();
}

// Load score board from file if exists
void loadScoreBoard() {
	std::ifstream file("scoreBoard.dat");
	if (file.is_open()) {
		std::string tmpString;
		int i = 0;
		while (file.good()) {
			std::getline(file, tmpString, ':');
			if (tmpString.empty()) break;
			scoreBoard[i].points = std::stoi(tmpString);

			std::getline(file, tmpString);
			scoreBoard[i].name = tmpString;
			i++;
		}
		file.close();
	} else {
		for (int i = 0; i < 10; i++) {
			scoreBoard[i].points = 0;
			scoreBoard[i].name = "";
		}
		writeScoreBoard();
	}

	std::sort(
	  scoreBoard, scoreBoard + 10, [](const ScoreBoard& a, const ScoreBoard& b) { return a.points > b.points; });
}

// Open link in browser for every operating system
void openInBrowser(const std::string& url) {
	std::string str;
	if (platform) switch (platform) {
		case 1:
			str = "explorer";
			break;
		case 2:
			str = "xdg-open";
			break;
		case 3:
			str = "open";
			break;
		default:
			str = "Unknown operating system, for info enter: " + url;
		}
	std::system(str.append(" " + url).c_str());
}

std::string returnBool(int value) {
	if (value == 0)
		return "Off";
	else if (value == 1)
		return "On";
	return "Error";
}