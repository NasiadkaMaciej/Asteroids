#include "SaveScore.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "../utils/sounds.hpp"
#include "MenuUtils.hpp"
#include <algorithm>
#include <cctype>
#include <curl/curl.h>

extern sf::RenderWindow window;
extern ScoreBoard scoreBoard[10];
extern void setState(eStates state);
extern GameTime* delta;

SaveScore::SaveScore(int entriesCount, std::string entries[])
  : Menu(entriesCount, entries) {}

void SaveScore::click() {
	switch (activeEntry) {
	case 0:
		saveScore();
		break;
	case 11:
		openInBrowser("https://asteroids.nasiadka.pl/");
		break;
	case 12:
		setState(menuState);
		break;
	}
}

void SaveScore::setScore(int points) {
	for (int i = 1; i < 11; i++)
		entries[i] = scoreBoard[i - 1].toString();

	this->points = points;
	entries[0] = "Your score " + std::to_string(points);
	entries[11] = "Global leaderboard";
	entries[12] = "Menu";
	name = "";
	move(0);
}

void SaveScore::saveScore() {
	move(0);
	if (!wasSaved) {
		entries[0] = "Your name: ";
		name = "";
		move(0);

		while (isSaving) {
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) window.close();
				if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
					// only ASCII, up to 18 chars, or backspace (8)
					auto unicode = textEntered->unicode;
					if ((unicode != 8 && name.length() < 18 && std::isalnum(unicode)) ||
						(unicode == 32 && name.length() < 18)) {
						char c = static_cast<char>(unicode);
						name += c;
						entries[0] += c;
					} else if (unicode == 8 && !name.empty()) {
						name.pop_back();
						entries[0].pop_back();
					}
					move(0);
				}
				if (CONTROL::isEnter() && name != "" && !wasSaved) {
					// If last entry in leaderboard is lower, replace it with current score
					if (scoreBoard[9].points < points) {
						scoreBoard[9] = { points, name };
						isSaving = false;
						wasSaved = true;
						std::sort(
						  scoreBoard, scoreBoard + 10, [&](ScoreBoard a, ScoreBoard b) { return a.points > b.points; });
					}
					// Show 10 scores and "Your score" at the top
					for (int i = 1; i < 11; i++)
						entries[i] = scoreBoard[i - 1].toString();
					entries[0] = "Your score " + std::to_string(points);
					move(0);

					// Save score to online leaderboard, secret is only in built releases.
					std::string const secret = "";
					if (secret != "") {
						transform(name.begin(), name.end(), name.begin(), ::tolower);
						std::string request =
						  "name=" + name + "&points=" + std::to_string(points) + "&secret=" + secret;
						const char* cRequest = request.c_str();
						CURL* curl;
						curl_global_init(CURL_GLOBAL_ALL);
						curl = curl_easy_init();
						CURLcode res;
						if (curl) {
							curl_easy_setopt(curl, CURLOPT_URL, "https://asteroids.nasiadka.pl/send");
							curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cRequest);
							res = curl_easy_perform(curl);
							if (res != CURLE_OK)
								fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
							curl_easy_cleanup(curl);
						}
						curl_global_cleanup();
					}
					isSaving = false;
					wasSaved = true;
				}
			}
			window.clear();
			draw(window);
			window.display();
		}
	}
	writeScoreBoard();
}

void SaveScore::reset() {
	Menu::reset();
	wasSaved = false;
	isSaving = true;
}