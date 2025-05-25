#include "SaveScore.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "MenuUtils.hpp"
#include <algorithm>
#include <curl/curl.h>

extern sf::RenderWindow window;
extern ScoreBoard scoreBoard[10];
extern void setState(eStates state);
extern GameTime* delta;

SaveScore::SaveScore()
  : Menu()
  , name("")
  , points(0)
  , isSaving(false)
  , wasSaved(false) {

	addItem("Your score: 0", [this]() {
		if (!isSaving && !wasSaved) {
			isSaving = true;
			getItem(0)->setText("Your name: " + name);
		}
	});

	for (int i = 0; i < 10; i++)
		addItem("", nullptr);

	// Add menu navigation options at the bottom
	addItem("Global Leaderboard", []() { openInBrowser("https://asteroids.nasiadka.pl"); });

	addItem("Back to Menu", []() { setState(menuState); });
}

void SaveScore::update() {
	// Call the base class update for menu navigation
	Menu::update();

	// Handle input mode for the first item
	if (this->selectedIndex == 0) {
		if (CONTROL::isEnter() && !this->isSaving && !this->wasSaved && delta->Menu > 100) {
			this->isSaving = true;
			getItem(0)->setText("Your name: " + this->name);
			delta->Menu = 0;
		}

		if (this->isSaving && !this->wasSaved) {
			// Show input prompt
			if (!this->name.empty())
				getItem(0)->setText("Your name: " + this->name);
			else
				getItem(0)->setText("Your name: ");

			// Handle enter to save score
			if (CONTROL::isEnter() && !this->name.empty() && delta->Menu > 100) {
				saveScore();
				delta->Menu = 0;
			}
		}
	} else if (this->isSaving && !this->wasSaved) {
		this->isSaving = false;
		getItem(0)->setText("Your score: " + std::to_string(this->points));
	}
}

void SaveScore::show() {
	this->selectedIndex = 0;
	while (window.isOpen() && activeState == saveScreenState) {
		delta->update();

		// Handle events
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) window.close();

			// Menu navigation and button clicks
			if (delta->Menu > 100) {
				if (CONTROL::isESC()) {
					setState(menuState);
					delta->Menu = 0;
					return;
				}
				handleInput();
			}

			if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mouseEvent->button == sf::Mouse::Button::Left) {
					auto mousePos = sf::Mouse::getPosition(window);
					auto worldPos = window.mapPixelToCoords(mousePos);
					handleMouseClick(worldPos);
				}
			}

			if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
				if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
					if (mouseWheelScrolled->delta > 0)
						navigate(UP);
					else if (mouseWheelScrolled->delta < 0)
						navigate(DOWN);
				}
			}

			// Text entry handling for name input
			if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
				if (textEntered->unicode == 13) { // Enter key
					if (this->isSaving && !this->wasSaved && !this->name.empty()) saveScore();
				} else if (textEntered->unicode == 8) { // Backspace
					onTextEntered(8);
				} else if (textEntered->unicode < 128) { // ASCII only
					onTextEntered(static_cast<char>(textEntered->unicode));
				}
			}
		}

		update();

		window.clear();
		Menu::draw(window);
		window.display();
	}
}

void SaveScore::setScore(int points) {
	this->points = points;
	getItem(0)->setText("Your score: " + std::to_string(points));

	// Update the leaderboard display
	for (int i = 0; i < 10; i++) {
		std::string scoreText;
		if (scoreBoard[i].points > 0)
			scoreText = scoreBoard[i].toString();
		else
			scoreText = std::to_string(i + 1) + ". ---";
		getItem(i + 1)->setText(scoreText);
	}

	reset();
}

void SaveScore::saveScore() {
	if (this->wasSaved || this->name.empty()) return;

	// Create and insert new score
	ScoreBoard newScore;
	newScore.points = this->points;
	newScore.name = this->name;

	if (scoreBoard[9].points < newScore.points) scoreBoard[9] = newScore;

	// Sort and save to file
	std::sort(
	  scoreBoard, scoreBoard + 10, [](const ScoreBoard& a, const ScoreBoard& b) { return a.points > b.points; });
	writeScoreBoard();

	// Update display
	getItem(0)->setText("Your score: " + std::to_string(this->points));

	// Update the leaderboard display
	for (int i = 0; i < 10; i++) {
		std::string scoreText;
		if (scoreBoard[i].points > 0)
			scoreText = scoreBoard[i].toString();
		else
			scoreText = std::to_string(i + 1) + ". ---";
		getItem(i + 1)->setText(scoreText);
	}

	// Save score to online leaderboard, secret is only in built releases.
	std::string const secret = "";
	if (secret != "") {
		std::string name_copy = this->name;
		transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
		std::string request = "name=" + name_copy + "&points=" + std::to_string(this->points) + "&secret=" + secret;
		const char* cRequest = request.c_str();

		CURL* curl;
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "https://asteroids.nasiadka.pl/send");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cRequest);
			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
	}

	this->wasSaved = true;
	this->isSaving = false;
}

void SaveScore::reset() {
	this->name = "";
	this->wasSaved = false;
	this->isSaving = false;
}

void SaveScore::onTextEntered(char character) {
	if (this->isSaving && !this->wasSaved) {
		if ((character != 8 && this->name.length() < 18 && std::isalnum(character)) ||
			(character == 32 && this->name.length() < 18)) {
			this->name += character;
			getItem(0)->setText("Your name: " + this->name);
		} else if (character == 8 && !this->name.empty()) {
			this->name.pop_back();
			getItem(0)->setText("Your name: " + this->name);
		}
	}
}