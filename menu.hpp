#include <SFML/Graphics.hpp>
#include <iostream>

#include <string>
#ifdef _WIN32
static int platform = 1;
#elif _WIN64
static int platform = 1;
#elif __linux__
static int platform = 2;
#elif __APPLE__
static int platform = 3;
#else
static int platform = 0;
#endif

void
openInBrowser()
{
  std::string str;
  std::string p = "https://github.com/NasiadkaMaciej/Asteroids";
  if (platform)
    switch (platform) {
      case 1:
        str = "explorer";
        break;
      case 2:
        str = "xdg-open";
        break;
      case 3:
        str = "open";
        break;
      default:
        str =
          "Unknown operating system, for info enter: "
          "https://github.com/NasiadkaMaciej/Asteroids"; // Should never happen
                                                         // on the 3 defined
                                                         // platforms
    }
  str.append(" " + p);
  std::system(str.c_str());
}

#define up 1
#define down 2

class Menu
{
public:
  int selectedItemIndex = 0;
  const static int items = 5;
  sf::Text menuPos[items];
  sf::String sMenu[items]{ "Play", "Leaderboard", "Settings", "Info", "Exit" };

  Menu()
  {
    for (int i = 0; i < items; i++) {
      menuPos[i].setString(sMenu[i]);
      menuPos[i].setFont(font);
      menuPos[i].setCharacterSize(50);
      menuPos[i].setFillColor(sf::Color::White);
      menuPos[i].setOrigin(menuPos[i].getGlobalBounds().width / 2,
                           menuPos[i].getGlobalBounds().height / 2);
      menuPos[i].setPosition(sf::Vector2f(
        desktopMode.width / 2, desktopMode.height / (items + 1) * (i + 1)));
    }
    menuPos[0].setFillColor(sf::Color::Red);
    menuPos[0].setString("> " + menuPos[0].getString() + " <");
    menuPos[0].setOrigin(menuPos[0].getGlobalBounds().width / 2,
                         menuPos[0].getGlobalBounds().height / 2);
  }
  void draw(sf::RenderWindow& window)
  {
    for (int i = 0; i < items; i++)
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
        if (selectedItemIndex + 1 < items) {
          menuPos[selectedItemIndex].setFillColor(sf::Color::White);
          selectedItemIndex++;
          menuPos[selectedItemIndex].setFillColor(sf::Color::Red);
        }
        break;
    }
    for (int i = 0; i < items; i++) {
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
        resume();
        // deltaClock.restart();
        break;
      case 3:
        openInBrowser();
        break;
      case 4:
        window.close();
        break;
    }
  }
  void show()
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        resume();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        move(up);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        move(down);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        click();
      }
    }
    window.clear();
    draw(window);
    window.display();
  }
};