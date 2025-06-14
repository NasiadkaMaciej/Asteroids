#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <string>

class MenuItem {
  public:
	MenuItem(const std::string& text, std::function<void()> action, const sf::Font& font);

	void setText(const std::string& text);
	const std::string& getText() const;

	void setAction(std::function<void()> action);
	void execute() const;

	void setSelected(bool selected);
	bool isSelected() const;

	void setPosition(const sf::Vector2f& position);
	sf::FloatRect getBounds() const;

	void draw(sf::RenderWindow& window) const;

  private:
	std::string text;
	std::function<void()> action;
	bool selected;
	sf::Text displayText;
	sf::Vector2f centeredPosition;

	void updateText();
};