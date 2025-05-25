#pragma once
#include "MenuItem.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

enum MenuDirection { UP = 1, DOWN = 2 };

class Menu {
  public:
	Menu();
	virtual ~Menu() = default;

	void addItem(const std::string& text, std::function<void()> action = nullptr);
	void clearItems();

	MenuItem* getItem(size_t index);
	size_t getItemCount() const;

	void selectItem(size_t index);
	size_t getSelectedIndex() const;

	void navigate(MenuDirection direction);
	void handleInput();
	void handleMouseClick(const sf::Vector2f& position);

	virtual void update();
	virtual void draw(sf::RenderWindow& window);
	virtual void show();

  protected:
	std::vector<std::unique_ptr<MenuItem>> m_items;
	size_t m_selectedIndex;

	void updateItemPositions();
};