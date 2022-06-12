#include "GameSettings.hpp"
#include <iostream>
#include <fstream>

float degToRad = M_PI / 180;

sf::VideoMode desktopMode;
sf::RenderWindow window;
sf::Font font;
sf::Text text;

float scale = (float)sf::VideoMode::getDesktopMode().height / 2000;

// to array?
bool isPlaying = false, isMenu = true, isGameOver = false, isSettings = false,
     isSaveScreen = false, isLeaderBoard = false;

void GameSettings::loadSettings()
{
  std::ifstream file("asteroids.cfg");
  if (file.is_open())
  {
    const int values = 6;
    int value[values];
    std::string tmpString;
    for (int i = 0; i < values; i++)
    {
      std::getline(file, tmpString, ':');
      std::getline(file, tmpString);
      value[i] = stoi(tmpString);
    }
    frames = value[0];
    vsync = value[1];
    fs = value[2];
    sfx = value[3];
    music = value[4];
    background = value[5];

    file.close();
  }
}
void GameSettings::saveSettings()
{
  {
    std::ofstream file("asteroids.cfg");
    if (file.is_open())
    {
      file << "Frames:" << frames << "\n";
      file << "VSync:" << vsync << "\n";
      file << "FullScreen:" << fs << "\n";
      file << "SFX:" << sfx << "\n";
      file << "Music:" << music << "\n";
      file << "Background:" << background << "\n";
    }
    file.close();
  }
}
int translateFS(int fs)
{
  if (fs == 0)
    return sf::Style::Default;
  else if (fs == 1)
    return sf::Style::Fullscreen;
  return 0;
}

GameValues *gameVal;
GameSettings gameSettings;
GameTime *delta;

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

  if (!font.loadFromFile("textures/Hyperspace.otf") || !image.loadFromFile("textures/icon.png"))
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
  isPlaying = isMenu = isGameOver = isSettings = isSaveScreen = isLeaderBoard = state;
}

void setState(int state)
{
  // All cases are hiding cursor except for playState
  window.setMouseCursorVisible(true);
  setStates(false);

  switch (state)
  {
  case playState:
    isPlaying = true;
    break;
  case menuState:
    isMenu = true;
    break;
  case gameoverState:
    isGameOver = true;
    break;
  case settingsState:
    isSettings = true;
    break;
  case saveScreenState:
    isSaveScreen = true;
    break;
  case leaderBoardState:
    isLeaderBoard = true;
    break;
  }
  delta->Clock.restart();
}
