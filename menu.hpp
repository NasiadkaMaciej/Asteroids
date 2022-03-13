#include <SFML/Graphics.hpp>
#include <iostream>

const int MENU_ITEMS = 5;
#define up 1
#define down 2

sf::Font font;
sf::Text text;

bool isMenu = true;

// create fullscreen window
sf::ContextSettings settings;
// settings.antialiasingLevel = 8.0;
sf::RenderWindow window(sf::VideoMode(desktopMode.width,
                                      desktopMode.height,
                                      desktopMode.bitsPerPixel),
                        "Asteroids - Macieson",
                        sf::Style::Fullscreen,
                        settings);
						
/*   window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60); */

class Menu
{
public:
  int selectedItemIndex = 0;
  sf::Text menuPos[MENU_ITEMS];
  sf::String sMenu[MENU_ITEMS]{ "Play",
                                "Leaderboard",
                                "Settings",
                                "Info",
                                "Exit" };

  Menu(float width, float height)
  {
    for (int i = 0; i < MENU_ITEMS; i++) {
      menuPos[i].setString(sMenu[i]);
      menuPos[i].setFont(font);
      menuPos[i].setCharacterSize(50);
      menuPos[i].setFillColor(sf::Color::White);
      menuPos[i].setOrigin(menuPos[i].getGlobalBounds().width / 2,
                           menuPos[i].getGlobalBounds().height / 2);
      menuPos[i].setPosition(
        sf::Vector2f(width / 2, height / (MENU_ITEMS + 1) * (i + 1)));
    }
    menuPos[0].setFillColor(sf::Color::Red);
    menuPos[0].setString("> " + menuPos[0].getString() + " <");
    menuPos[0].setOrigin(menuPos[0].getGlobalBounds().width / 2,
                         menuPos[0].getGlobalBounds().height / 2);
  }
  void draw(sf::RenderWindow& window)
  {
    for (int i = 0; i < MENU_ITEMS; i++)
      window.draw(menuPos[i]);
  }
  void move(int direction)
  {
    switch (direction) {
      case up:
        if (selectedItemIndex - 1 >= 0) {
          menuPos[selectedItemIndex].setFillColor(sf::Color::White);
          selectedItemIndex--;
          menuPos[selectedItemIndex].setFillColor(sf::Color::Red);
        }
        break;
      case down:
        if (selectedItemIndex + 1 < MENU_ITEMS) {
          menuPos[selectedItemIndex].setFillColor(sf::Color::White);
          selectedItemIndex++;
          menuPos[selectedItemIndex].setFillColor(sf::Color::Red);
        }
        break;
    }
    for (int i = 0; i < MENU_ITEMS; i++) {
      menuPos[i].setString(sMenu[i]);
      menuPos[i].setOrigin(menuPos[i].getGlobalBounds().width / 2,
                           menuPos[i].getGlobalBounds().height / 2);
    }
    menuPos[selectedItemIndex].setString(
      "> " + menuPos[selectedItemIndex].getString() + " <");
    menuPos[selectedItemIndex].setOrigin(
      menuPos[selectedItemIndex].getGlobalBounds().width / 2,
      menuPos[selectedItemIndex].getGlobalBounds().height / 2);
  };
  void click()
  {
    switch (selectedItemIndex) {
      case 0:
        isMenu = false;
        break;
      case 4:
        window.close();
        break;
    }
  }
};