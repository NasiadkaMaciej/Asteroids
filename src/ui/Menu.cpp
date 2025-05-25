#include "Menu.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "../utils/sounds.hpp"

extern sf::RenderWindow window;
extern GameTime* delta;
extern sf::Font font;

Menu::Menu()
  : m_selectedIndex(0) {}

void Menu::addItem(const std::string& text, std::function<void()> action) {
	m_items.push_back(std::make_unique<MenuItem>(text, action, font));
	updateItemPositions();

	if (m_items.size() == 1) m_items[0]->setSelected(true);
}

void Menu::clearItems() {
	m_items.clear();
	m_selectedIndex = 0;
}

MenuItem* Menu::getItem(size_t index) {
	if (index < m_items.size()) return m_items[index].get();
	return nullptr;
}

size_t Menu::getItemCount() const {
	return m_items.size();
}

void Menu::selectItem(size_t index) {
	if (index < m_items.size() && index != m_selectedIndex) {
		if (m_selectedIndex < m_items.size()) m_items[m_selectedIndex]->setSelected(false);

		m_selectedIndex = index;
		m_items[m_selectedIndex]->setSelected(true);
	}
}

size_t Menu::getSelectedIndex() const {
	return m_selectedIndex;
}

void Menu::navigate(MenuDirection direction) {
	size_t newIndex = m_selectedIndex;

	switch (direction) {
	case UP:
		if (newIndex > 0)
			newIndex--;
		else
			newIndex = m_items.size() - 1;
		break;

	case DOWN:
		if (newIndex + 1 < m_items.size())
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
		if (m_selectedIndex < m_items.size()) {
			m_items[m_selectedIndex]->execute();
			playSound(&menuSound);
		}
		inputHandled = true;
	}

	delta->Menu = 0;
}

void Menu::handleMouseClick(const sf::Vector2f& position) {
	for (size_t i = 0; i < m_items.size(); i++) {
		if (m_items[i]->getBounds().contains(position)) {
			if (i == m_selectedIndex)
				m_items[i]->execute();
			else
				selectItem(i);
			playSound(&menuSound);
			break;
		}
	}
}

void Menu::update() {}

void Menu::draw(sf::RenderWindow& window) {
	for (const auto& item : m_items)
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
	if (m_items.empty()) return;

	float screenWidth = window.isOpen() ? window.getSize().x : static_cast<float>(gameSettings.resX);
	float screenHeight = window.isOpen() ? window.getSize().y : static_cast<float>(gameSettings.resY);
	float itemCount = static_cast<float>(m_items.size());

	for (size_t i = 0; i < m_items.size(); i++) {
		float y = screenHeight / (itemCount + 1.0f) * (i + 1.0f);
		m_items[i]->setPosition(sf::Vector2f(screenWidth / 2.0f, y));
	}
}