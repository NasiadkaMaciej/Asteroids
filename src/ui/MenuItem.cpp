#include "MenuItem.hpp"
#include <SFML/System/Vector2.hpp>

extern sf::Font font;

MenuItem::MenuItem(const std::string& text, std::function<void()> action, const sf::Font& font)
  : text(text)
  , action(action)
  , selected(false)
  , displayText(font, "") {
	displayText.setCharacterSize(30); // ToDo: Adjust font size
	displayText.setFillColor(sf::Color::White);
	updateText();
}

void MenuItem::setText(const std::string& text) {
	this->text = text;
	updateText();
}

const std::string& MenuItem::getText() const {
	return text;
}

void MenuItem::setAction(std::function<void()> action) {
	this->action = action;
}

void MenuItem::execute() const {
	if (action) action();
}

void MenuItem::setSelected(bool selected) {
	if (this->selected != selected) {
		this->selected = selected;
		updateText();
	}
}

bool MenuItem::isSelected() const {
	return selected;
}

void MenuItem::setPosition(const sf::Vector2f& position) {
	centeredPosition = position; // Store the center position
	sf::FloatRect bounds = displayText.getLocalBounds();
	displayText.setPosition(position - sf::Vector2f{ bounds.size.x / 2, bounds.size.y / 2 });
}

sf::FloatRect MenuItem::getBounds() const {
	return displayText.getGlobalBounds();
}

void MenuItem::draw(sf::RenderWindow& window) const {
	window.draw(displayText);
}

void MenuItem::updateText() {
	if (selected) {
		displayText.setString("> " + text + " <");
		displayText.setFillColor(sf::Color::Red);
	} else {
		displayText.setString(text);
		displayText.setFillColor(sf::Color::White);
	}

	// Recenter using the stored center position
	sf::FloatRect bounds = displayText.getLocalBounds();
	displayText.setPosition(centeredPosition - sf::Vector2f{ bounds.size.x / 2, bounds.size.y / 2 });
}