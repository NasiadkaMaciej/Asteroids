// src/ui/Menu.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum MenuDirection { UP = 1, DOWN = 2 };

class Menu {
  public:
	int activeEntry = 0;
	int entriesCount;
	std::vector<sf::Text> entryText;
	std::vector<std::string> entries;

	Menu(int entriesCount, std::string entries[]);
	virtual ~Menu() = default;

	void draw(sf::RenderWindow& window);
	void reset();
	void move(int direction);

	virtual void show();
	virtual void click();
	virtual void mouseClick();
};