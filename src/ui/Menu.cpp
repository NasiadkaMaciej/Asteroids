#include "Menu.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "../utils/sounds.hpp"
#include "MenuUtils.hpp"
#include <SFML/Graphics.hpp>

extern sf::Font font;
extern char activeState;
extern void setState(eStates state);
extern GameTime* delta;

Menu::Menu(int entriesCount, std::string entries[])
  : entriesCount(entriesCount) {
	entryText.clear();
	entryText.reserve(entriesCount);

	this->entries.clear();
	this->entries.reserve(entriesCount);
	for (int i = 0; i < entriesCount; ++i)
		this->entries.push_back(entries[i]);

	for (int i = 0; i < entriesCount; ++i) {
		entryText.emplace_back(font, this->entries[i], 30);
		entryText[i].setFillColor(sf::Color::White);

		auto bounds = entryText[i].getGlobalBounds();
		entryText[i].setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
		entryText[i].setPosition(
		  sf::Vector2f(gameSettings.resX / 2.f, gameSettings.resY / (entriesCount + 1.f) * (i + 1.f)));
	}

	entryText[activeEntry].setString("> " + entries[activeEntry] + " <");
	entryText[activeEntry].setFillColor(sf::Color::Red);

	auto bounds = entryText[activeEntry].getGlobalBounds();
	entryText[activeEntry].setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
}

void Menu::draw(sf::RenderWindow& window) {
	for (auto& txt : entryText)
		window.draw(txt);
}

void Menu::reset() {
	do
		move(UP);
	while (activeEntry != 0);
}

void Menu::move(int direction) {
	// Actually move
	switch (direction) {
	case UP:
		if (activeEntry - 1 >= 0)
			activeEntry--;
		else
			activeEntry = entriesCount - 1;
		break;
	case DOWN:
		if (activeEntry + 1 < entriesCount)
			activeEntry++;
		else
			activeEntry = 0;
		break;
	default:
		break;
	}

	// Format and color
	for (int i = 0; i < entriesCount; ++i) {
		entryText[i].setFillColor(sf::Color::White);
		entryText[i].setString(entries[i]); // Normal entries
		auto b = entryText[i].getGlobalBounds();
		entryText[i].setOrigin(sf::Vector2f(b.size.x / 2.f, b.size.y / 2.f));
	}

	entryText[activeEntry].setString("> " + entries[activeEntry] + " <");
	entryText[activeEntry].setFillColor(sf::Color::Red);

	// Update origin again since text width changed
	auto bounds = entryText[activeEntry].getGlobalBounds();
	entryText[activeEntry].setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
}

void Menu::show() {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) window.close();

		bool clicked = false;
		if (delta && delta->Menu > 100) { // disallow too quick movement and prevent double clicks
			if (CONTROL::isESC()) {
				if (activeState == menuState)
					setState(playState);
				else if (activeState == saveScreenState)
					setState(gameoverState);
				else
					setState(menuState);
				clicked = true;
			}
			if (CONTROL::isUP()) {
				move(UP);
				clicked = true;
			} else if (CONTROL::isDOWN()) {
				move(DOWN);
				clicked = true;
			}
			if (CONTROL::isEnter()) {
				click();
				clicked = true;
			}
			delta->Menu = 0;
		}

		if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
			clicked = mouseEvent->button == sf::Mouse::Button::Left;
		if (clicked) {
			mouseClick();
			playSound(&menuSound);
		}

		if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
			if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
				if (mouseWheelScrolled->delta > 0)
					move(UP);
				else if (mouseWheelScrolled->delta < 0)
					move(DOWN);
			}
		}
	}

	window.clear();
	draw(window);
	window.draw(newVersion);
	window.display();
}

void Menu::click() {
	switch (activeEntry) {
	case 0:
		setState(playState);
		break;
	case 1:
		setState(leaderBoardState);
		break;
	case 2:
		setState(settingsState);
		break;
	case 3:
		openInBrowser("https://github.com/NasiadkaMaciej/Asteroids");
		break;
	case 4:
		window.close();
		break;
	}
}

void Menu::mouseClick() {
	auto mouse_pos = sf::Mouse::getPosition(window);
	auto translated_pos = window.mapPixelToCoords(mouse_pos);

	for (int i = 0; i < entriesCount; i++) {
		if (entryText[i].getGlobalBounds().contains(translated_pos)) {
			if (activeEntry == i)
				click();
			else
				do
					move(UP);
				while (activeEntry != i);
		}
	}
}