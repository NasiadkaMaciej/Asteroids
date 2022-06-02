#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

// states
#define playState 0
#define menuState 1
#define gameoverState 2
#define settingsState 3
#define saveScreenState 4
#define leaderBoardState 5

// game start values;
int bigAsteroids = 4; // when generating, 2 more are created
int roundNum = 0;     // when starting, 1 is added
sf::Clock deltaClock;
sf::Time deltaTime;
float deltaShoot, deltaPowerUp, deltaMenu;

// create fullscreen window
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::RenderWindow window(sf::VideoMode(desktopMode.width,
                                      desktopMode.height,
                                      desktopMode.bitsPerPixel),
                        "Asteroids - Macieson",
                        sf::Style::Fullscreen);

bool isFS = true;
bool isMute = false;

bool isDoubleShooting = false;

float degToRad = M_PI / 180;

// to array?
bool isPlaying = false, isMenu = true, isGameOver = false, isSettings = false,
     isSaveScreen = false, isLeaderBoard = false;

sf::Font font;
sf::Text text;

bool loadBase()
{
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);

  sf::Image image;

  if (!font.loadFromFile("Hyperspace.otf") || !image.loadFromFile("textures/icon.png"))
  {
    std::cout << "Error loading font or icon\n";
    return false;
  }

  text.setFont(font);
  text.setCharacterSize(50);
  text.setFillColor(sf::Color::White);
  text.setPosition(60, 60);

  window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

  srand(time(NULL));
  return true;
}

void setStates(bool state)
{
  isPlaying = state;
  isMenu = state;
  isGameOver = state;
  isSettings = state;
  isSaveScreen = state;
  isLeaderBoard = state;
}

void setState(int state)
{
  switch (state)
  {
  case playState:
    window.setMouseCursorVisible(false);
    setStates(false);
    isPlaying = true;
    break;
  case menuState:
    window.setMouseCursorVisible(true);
    setStates(false);
    isMenu = true;
    break;
  case gameoverState:
    window.setMouseCursorVisible(true);
    setStates(false);
    isGameOver = true;
    break;
  case settingsState:
    window.setMouseCursorVisible(true);
    setStates(false);
    isSettings = true;
    break;
  case saveScreenState:
    window.setMouseCursorVisible(true);
    setStates(false);
    isSaveScreen = true;
    break;
  case leaderBoardState:
    window.setMouseCursorVisible(true);
    setStates(false);
    isLeaderBoard = true;
    break;
  default:
    break;
  }
  deltaClock.restart();
}