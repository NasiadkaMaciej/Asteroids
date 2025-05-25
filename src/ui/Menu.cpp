#include "Menu.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "../utils/sounds.hpp"

extern sf::RenderWindow window;
extern GameTime* delta;
extern sf::Font font;

Menu::Menu()
  : selectedIndex(0) {}

void Menu::addItem(const std::string& text, std::function<void()> action) {
	items.push_back(std::make_unique<MenuItem>(text, action, font));
	updateItemPositions();

	if (items.size() == 1) items[0]->setSelected(true);
}

void Menu::clearItems() {
	items.clear();
	selectedIndex = 0;
}

MenuItem* Menu::getItem(size_t index) {
	if (index < items.size()) return items[index].get();
	return nullptr;
}

size_t Menu::getItemCount() const {
	return items.size();
}

void Menu::selectItem(size_t index) {
	if (index < items.size() && index != selectedIndex) {
		if (selectedIndex < items.size()) items[selectedIndex]->setSelected(false);

		selectedIndex = index;
		items[selectedIndex]->setSelected(true);
	}
}

size_t Menu::getSelectedIndex() const {
	return selectedIndex;
}

void Menu::navigate(MenuDirection direction) {
	size_t newIndex = selectedIndex;

	switch (direction) {
	case UP:
		if (newIndex > 0)
			newIndex--;
		else
			newIndex = items.size() - 1;
		break;

	case DOWN:
		if (newIndex + 1 < items.size())
			newIndex++;
		else
			newIndex = 0;
		break;
	}

	selectItem(newIndex);
}

void Menu::handleInput() {
	bool inputHandled = false;

	if (delta->Menu < 100) return;
	if (CONTROL::isUP()) {
		navigate(UP);
		playSound(&menuSound);
		inputHandled = true;
	} else if (CONTROL::isDOWN()) {
		navigate(DOWN);
		playSound(&menuSound);
		inputHandled = true;
	} else if (CONTROL::isEnter() || CONTROL::isSpace()) {
		if (selectedIndex < items.size()) {
			items[selectedIndex]->execute();
			playSound(&menuSound);
		}
		inputHandled = true;
	}

	delta->Menu = 0;
}

void Menu::handleMouseClick(const sf::Vector2f& position) {
	for (size_t i = 0; i < items.size(); i++) {
		if (items[i]->getBounds().contains(position)) {
			if (i == selectedIndex)
				items[i]->execute();
			else
				selectItem(i);
			playSound(&menuSound);
			break;
		}
	}
}

void Menu::update() {}

void Menu::draw(sf::RenderWindow& window) {
	for (const auto& item : items)
		item->draw(window);
}

void Menu::show() {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) window.close();

		if (delta->Menu > 100) {
			if (CONTROL::isESC()) {
				delta->Menu = 0;
				break;
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
				if (mouseWheelScrolled->delta > 0) {
					navigate(UP);
					playSound(&menuSound);
				} else if (mouseWheelScrolled->delta < 0) {
					navigate(DOWN);
					playSound(&menuSound);
				}
			}
		}
	}

	update();

	window.clear();
	draw(window);
	window.draw(newVersion);
	window.display();
}

void Menu::updateItemPositions() {
	if (this->items.empty()) return;

	float screenWidth = window.isOpen() ? window.getSize().x : static_cast<float>(gameSettings.resX);
	float screenHeight = window.isOpen() ? window.getSize().y : static_cast<float>(gameSettings.resY);
	float itemCount = static_cast<float>(this->items.size());

	for (size_t i = 0; i < this->items.size(); i++) {
		float y = screenHeight / (itemCount + 1.0f) * (i + 1.0f);
		this->items[i]->setPosition(sf::Vector2f(screenWidth / 2.0f, y));
	}
}