#include "MenuItem.hpp"
#include <SFML/System/Vector2.hpp>

extern sf::Font font;

MenuItem::MenuItem(const std::string& text, std::function<void()> action, const sf::Font& font)
  : m_text(text)
  , m_action(action)
  , m_selected(false)
  , m_displayText(font, "") {
	m_displayText.setCharacterSize(30); // ToDo: Adjust font size
	m_displayText.setFillColor(sf::Color::White);
	updateText();
}

void MenuItem::setText(const std::string& text) {
	m_text = text;
	updateText();
}

const std::string& MenuItem::getText() const {
	return m_text;
}

void MenuItem::setAction(std::function<void()> action) {
	m_action = action;
}

void MenuItem::execute() const {
	if (m_action) m_action();
}

void MenuItem::setSelected(bool selected) {
	if (m_selected != selected) {
		m_selected = selected;
		updateText();
	}
}

bool MenuItem::isSelected() const {
	return m_selected;
}

void MenuItem::setPosition(const sf::Vector2f& position) {
	m_centeredPosition = position; // Store the center position
	sf::FloatRect bounds = m_displayText.getLocalBounds();
	m_displayText.setPosition(position - sf::Vector2f{ bounds.size.x / 2, bounds.size.y / 2 });
}

sf::FloatRect MenuItem::getBounds() const {
	return m_displayText.getGlobalBounds();
}

void MenuItem::draw(sf::RenderWindow& window) const {
	window.draw(m_displayText);
}

void MenuItem::updateText() {
	if (m_selected) {
		m_displayText.setString("> " + m_text + " <");
		m_displayText.setFillColor(sf::Color::Red);
	} else {
		m_displayText.setString(m_text);
		m_displayText.setFillColor(sf::Color::White);
	}

	// Recenter using the stored center position
	sf::FloatRect bounds = m_displayText.getLocalBounds();
	m_displayText.setPosition(m_centeredPosition - sf::Vector2f{ bounds.size.x / 2, bounds.size.y / 2 });
}