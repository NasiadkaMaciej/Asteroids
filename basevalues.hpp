#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>

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
float deltaShoot, deltaPowerUp, deltaMenu, deltaMove;

sf::VideoMode desktopMode;
sf::RenderWindow window;

bool isDoubleShooting = false;

float degToRad = M_PI / 180;

// to array?
bool isPlaying = false, isMenu = true, isGameOver = false, isSettings = false,
     isSaveScreen = false, isLeaderBoard = false;

sf::Font font;
sf::Text text;

struct GameSettings
{
public:
  GameSettings()
  {
    loadSetting();
  }
  int frames;
  bool vsync, fs, sfx, music;
  void loadSetting()
  {
    std::ifstream file("asteroids.cfg");
    if (file.is_open())
    {
      std::string tmpString;
      std::getline(file, tmpString, ':');
      std::getline(file, tmpString);
      frames = stoi(tmpString);
      std::getline(file, tmpString, ':');
      std::getline(file, tmpString);
      vsync = stoi(tmpString);
      std::getline(file, tmpString, ':');
      std::getline(file, tmpString);
      fs = stoi(tmpString);
      std::getline(file, tmpString, ':');
      std::getline(file, tmpString);
      sfx = stoi(tmpString);
      std::getline(file, tmpString, ':');
      std::getline(file, tmpString);
      music = stoi(tmpString);
      file.close();
    }
    else
    {
      std::ofstream file("asteroids.cfg");
      if (file.is_open())
      {
        file << "Frames:60\n";
        file << "VSync:1\n";
        file << "FullScreen:1\n";
        file << "SFX:1\n";
        file << "Music:1\n";
      }
      file.close();
      loadSetting();
    }
  }
  void saveSettings()
  {
    std::ofstream file("asteroids.cfg");
    if (file.is_open())
    {
      file << "Frames:" << frames << "\n";
      file << "VSync:" << vsync << "\n";
      file << "FullScreen:" << fs << "\n";
      file << "SFX:" << sfx << "\n";
      file << "Music:" << music << "\n";
    }
    file.close();
  }
};
int translateFS(int fs)
{
  if (fs == 0)
    return sf::Style::Default;
  else if (fs == 1)
    return sf::Style::Fullscreen;
  return 0;
}
GameSettings gameSettings;

bool loadBase()
{
  // create fullscreen window
  desktopMode = sf::VideoMode::getDesktopMode();
  window.create(sf::VideoMode(desktopMode.width,
                              desktopMode.height,
                              desktopMode.bitsPerPixel),
                "Asteroids - Macieson",
                translateFS(gameSettings.fs));
  window.setFramerateLimit(gameSettings.frames);
  window.setVerticalSyncEnabled(gameSettings.vsync);

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
  }
  deltaClock.restart();
}