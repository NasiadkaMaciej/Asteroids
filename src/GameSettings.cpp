#include "GameSettings.hpp"
#include <iostream>
#include <fstream>

float degToRad = M_PI / 180;

sf::VideoMode desktopMode;
sf::RenderWindow window;
sf::Font font;
sf::Text text;

float scale;

// to array?
bool isPlaying = false, isMenu = true, isGameOver = false, isSettings = false,
     isSaveScreen = false, isLeaderBoard = false;

void GameSettings::loadSettings()
{
  // Read all available fullscreen resolutions and sort them
  for (const auto &tmp : sf::VideoMode::getFullscreenModes())
    if (tmp.bitsPerPixel == 24)
      availRes.push_back(tmp);
  std::reverse(availRes.begin(), availRes.end());

  std::ifstream file("asteroids.cfg");
  if (file.is_open())
  {
    // list?
    const int values = 8;
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
    resX = value[6];
    resY = value[7];

    scale = (float)resY / 2000;
    file.close();
  }
  else
    saveSettings();
}
void GameSettings::saveSettings()
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
    file << "resX:" << resX << "\n";
    file << "resY:" << resY << "\n";
  }

  scale = (float)resY / 2000;
  file.close();
}
int GameSettings::translateFS()
{
  return fs ? sf::Style::Fullscreen : sf::Style::Default;
}
bool GameSettings::checkRes()
{
  for (const auto &videoMode : sf::VideoMode::getFullscreenModes())
    if (videoMode.width == resX && videoMode.height == resY)
      return true;
  return false;
}
GameValues *gameVal;
GameSettings gameSettings;
GameTime *delta;

bool loadBase()
{
  if (!gameSettings.checkRes()) // Check if is fs and resolutions are in valid video modes
  {
    std::cout << "Invalid fullscreen resolution. Available modes are: \n";
    for (const auto &videoMode : sf::VideoMode::getFullscreenModes())
      if (videoMode.bitsPerPixel == 32)
        std::cout << videoMode.width << "x" << videoMode.height << "\n";
    return false;
  }
  desktopMode = sf::VideoMode::getDesktopMode();
  window.create(sf::VideoMode(gameSettings.resX,
                              gameSettings.resY,
                              desktopMode.bitsPerPixel),
                "Asteroids - Macieson", gameSettings.translateFS());
                //"Asteroids - Macieson", gameSettings.translateFS(), sf::ContextSettings(24,8,16));
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

void setState(eStates state)
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
