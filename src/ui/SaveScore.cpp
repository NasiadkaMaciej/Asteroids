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
  , m_name("")
  , m_points(0)
  , m_isSaving(false)
  , m_wasSaved(false) {

	addItem("Your score: 0", [this]() {
		if (!m_isSaving && !m_wasSaved) {
			m_isSaving = true;
			getItem(0)->setText("Your name: " + m_name);
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
	if (m_selectedIndex == 0) {
		if (CONTROL::isEnter() && !m_isSaving && !m_wasSaved && delta->Menu > 100) {
			m_isSaving = true;
			getItem(0)->setText("Your name: " + m_name);
			delta->Menu = 0;
		}

		if (m_isSaving && !m_wasSaved) {
			// Show input prompt
			if (!m_name.empty())
				getItem(0)->setText("Your name: " + m_name);
			else
				getItem(0)->setText("Your name: ");

			// Handle enter to save score
			if (CONTROL::isEnter() && !m_name.empty() && delta->Menu > 100) {
				saveScore();
				delta->Menu = 0;
			}
		}
	} else if (m_isSaving && !m_wasSaved) {
		m_isSaving = false;
		getItem(0)->setText("Your score: " + std::to_string(m_points));
	}
}

void SaveScore::show() {
	m_selectedIndex = 0;
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
					if (m_isSaving && !m_wasSaved && !m_name.empty()) saveScore();
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
	m_points = points;
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
	if (m_wasSaved || m_name.empty()) return;

	// Create and insert new score
	ScoreBoard newScore;
	newScore.points = m_points;
	newScore.name = m_name;

	if (scoreBoard[9].points < newScore.points) scoreBoard[9] = newScore;

	// Sort and save to file
	std::sort(
	  scoreBoard, scoreBoard + 10, [](const ScoreBoard& a, const ScoreBoard& b) { return a.points > b.points; });
	writeScoreBoard();

	// Update display
	getItem(0)->setText("Your score: " + std::to_string(m_points));

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
		std::string name_copy = m_name;
		transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
		std::string request = "name=" + name_copy + "&points=" + std::to_string(m_points) + "&secret=" + secret;
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

	m_wasSaved = true;
	m_isSaving = false;
}

void SaveScore::reset() {
	m_name = "";
	m_wasSaved = false;
	m_isSaving = false;
}

void SaveScore::onTextEntered(char character) {
	if (m_isSaving && !m_wasSaved) {
		if ((character != 8 && m_name.length() < 18 && std::isalnum(character)) ||
			(character == 32 && m_name.length() < 18)) {
			m_name += character;
			getItem(0)->setText("Your name: " + m_name);
		} else if (character == 8 && !m_name.empty()) {
			m_name.pop_back();
			getItem(0)->setText("Your name: " + m_name);
		}
	}
}