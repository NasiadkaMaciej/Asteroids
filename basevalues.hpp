#include <SFML/Graphics.hpp>
#include <math.h>

// game start values;
int bigAsteroids = 4; // when generating, 2 more are created
int roundNum = 0;     // when starging, 1 is added
sf::Clock deltaClock;
sf::Time deltaTime, deltaPausedTime;
float deltaShoot;

// create fullscreen window
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::RenderWindow window(sf::VideoMode(desktopMode.width,
                                      desktopMode.height,
                                      desktopMode.bitsPerPixel),
                        "Asteroids - Macieson",
                        sf::Style::Fullscreen);

float degToRad = M_PI / 180;

bool isMenu = true;
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

  // Thinking about mouse support in menu
  window.setMouseCursorVisible(false);
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
}
void
pause()
{
  isMenu = true;
  deltaClock.restart();
  deltaPausedTime = deltaTime;
}
void
resume()
{
  isMenu = false;
  deltaClock.restart();
  deltaTime = deltaPausedTime;
}