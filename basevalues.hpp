#include <SFML/Graphics.hpp>
#include <math.h>

// states
#define playState 0
#define menuState 1
#define gameoverstate 2

// game start values;
int bigAsteroids = 4; // when generating, 2 more are created
int roundNum = 0;     // when starging, 1 is added
sf::Clock deltaClock;
sf::Time deltaTime, deltaPausedTime;
float deltaShoot, deltaPowerUp, deltaMenu;

// create fullscreen window
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::RenderWindow window(sf::VideoMode(desktopMode.width,
                                      desktopMode.height,
                                      desktopMode.bitsPerPixel),
                        "Asteroids - Macieson",
                        sf::Style::Fullscreen);

float degToRad = M_PI / 180;

bool isPlaying = false, isMenu = true, isGameOver = false, isSaveScreen = false;

sf::Font font;
sf::Text text;

void
loadBase()
{
  font.loadFromFile("Hyperspace.otf");
  text.setFont(font);
  text.setCharacterSize(50);
  text.setFillColor(sf::Color::White);
  text.setPosition(60, 60);

  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
}

void
setState(int state)
{
  switch (state) {
    case playState:
      window.setMouseCursorVisible(false);
      isPlaying = true;
      isMenu = false;
      isGameOver = false;
      isSaveScreen = false;
      deltaClock.restart();
      deltaTime = deltaPausedTime;
      break;
    case menuState:
      window.setMouseCursorVisible(true);
      isPlaying = false;
      isMenu = true;
      isGameOver = false;
      isSaveScreen = false;
      deltaClock.restart();
      deltaPausedTime = deltaTime;
      break;
    case gameoverstate:
      window.setMouseCursorVisible(true);
      isPlaying = false;
      isMenu = false;
      isGameOver = true;
      isSaveScreen = false;
      deltaClock.restart();
      deltaPausedTime = deltaTime;
      break;
    default:
      break;
  }
}