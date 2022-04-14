#include <SFML/Graphics.hpp>
#include <list>
#include <string>

#include <iostream>

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
        str = "Unknown operating system, for info enter: " + p;
    }
  str.append(" " + p);
  std::system(str.c_str());
}

#define up 1
#define down 2

const int menuEntriesCount = 5, gameOverEntriesCount = 3,
          settingEntriesCount = 3, saveScoreEntriesCount = 11;
sf::String menuEntries[menuEntriesCount]{ "Play",
                                          "Leaderboard",
                                          "Settings",
                                          "Info",
                                          "Exit" },
  gameOverEntries[gameOverEntriesCount]{ "Your score", "New game", "Menu" },
  settingEntries[settingEntriesCount]{ "Frame rate limit: 60",
                                       "VSync: On",
                                       "Menu" },
  saveScoreEntries[saveScoreEntriesCount];

class Menu
{
public:
  int activeEntry = 0;
  // eventually
  int entriesCount;
  sf::Text* entryText;
  sf::String* entries;
  Menu(int ENTRIESCOUNT, sf::String ENTRIES[])
  {
    entries = ENTRIES;
    entriesCount = ENTRIESCOUNT;
    entryText = new sf::Text[entriesCount];
    for (int i = 0; i < entriesCount; i++) {
      entryText[i].setString(entries[i]);
      entryText[i].setFont(font);
      entryText[i].setCharacterSize(50);
      entryText[i].setFillColor(sf::Color::White);
      entryText[i].setOrigin(entryText[i].getGlobalBounds().width / 2,
                             entryText[i].getGlobalBounds().height / 2);
      entryText[i].setPosition(
        sf::Vector2f(desktopMode.width / 2,
                     desktopMode.height / (entriesCount + 1) * (i + 1)));
    }
    // Make active entry look like active one
    entryText[0].setFillColor(sf::Color::Red);
    entryText[0].setString("> " + entryText[0].getString() + " <");
    entryText[0].setOrigin(entryText[0].getGlobalBounds().width / 2,
                           entryText[0].getGlobalBounds().height / 2);
  }
  void draw(sf::RenderWindow& window)
  {
    for (int i = 0; i < entriesCount; i++)
      window.draw(entryText[i]);
  }
  void move(int direction)
  {
    // actually move
    switch (direction) {
      case up:
        if (activeEntry - 1 >= 0)
          activeEntry--;
        else if (activeEntry - 1 < 0)
          activeEntry = entriesCount - 1;
        break;
      case down:
        if (activeEntry + 1 < entriesCount)
          activeEntry++;
        else if (activeEntry + 1 >= entriesCount)
          activeEntry = 0;
        break;
      default:
        break;
    }
    // format and color
    for (int i = 0; i < entriesCount; i++) {
      entryText[i].setFillColor(sf::Color::White);
      entryText[i].setString(entries[i]);
      entryText[i].setOrigin(entryText[i].getGlobalBounds().width / 2,
                             entryText[i].getGlobalBounds().height / 2);
    }
    entryText[activeEntry].setFillColor(sf::Color::Red);
    entryText[activeEntry].setString("> " + entryText[activeEntry].getString() +
                                     " <");
    entryText[activeEntry].setOrigin(
      entryText[activeEntry].getGlobalBounds().width / 2,
      entryText[activeEntry].getGlobalBounds().height / 2);
  }
  virtual void show()
  {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        setState(playState);
      if (deltaMenu >= 100) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          move(up);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          move(down);
        deltaMenu = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
          click();
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        mouseClick();
      if (event.type == sf::Event::MouseWheelScrolled)
        if (event.mouseWheelScroll.delta > 0)
          move(up);
        else if (event.mouseWheelScroll.delta < 0)
          move(down);
    }

    window.clear();
    draw(window);
    window.display();
  }
  // only valid for actual Menu
  virtual void click()
  {
    switch (activeEntry) {
      case 0:
        setState(playState);
        break;
      case 2:
        setState(settingsState);
        break;
      case 3:
        openInBrowser();
        break;
      case 4:
        window.close();
        break;
    }
  }
  virtual void mouseClick()
  {
    auto mouse_pos = sf::Mouse::getPosition(window);
    auto translated_pos = window.mapPixelToCoords(mouse_pos);

    for (int i = 0; i < entriesCount; i++) {
      if (entryText[i].getGlobalBounds().contains(translated_pos)) {
        if (activeEntry == i)
          click();
        else
          do {
            move(up);
          } while (activeEntry != i);
      }
    }
  }
};

class GameOver : public Menu
{
public:
  GameOver(int EntriesCount, sf::String entries[])
    : Menu(EntriesCount, entries){};
  void click()
  {
    switch (activeEntry) {
      case 0:
        isSaveScreen = true;
        break;
      case 1:
        setState(playState);
        break;
      case 2:
        setState(menuState);
        break;
    }
  }
  void setScore(int POINTS)
  {
    entries[0] = "Your score " + std::to_string(POINTS);
    // is updating text on menu screen
    move(0);
  }
};

class Settings : public Menu
{
public:
  int activeLimit = 60;
  bool vsync = true;
  Settings(int EntriesCount, sf::String entries[])
    : Menu(EntriesCount, entries){};
  void click()
  {
    switch (activeEntry) {
      case 0:
        switchRefreshRate();
        break;
      case 1:
        toggleVsync();
        break;
      case 2:
        setState(menuState);
        break;
    }
    move(0);
  }
  void switchRefreshRate()
  {
    switch (activeLimit) {
      case 60:
        activeLimit = 75;
        break;
      case 75:
        activeLimit = 120;
        break;
      case 120:
        activeLimit = 144;
        break;
      case 144:
        activeLimit = 165;
        break;
      case 165:
        activeLimit = 60;
        break;
    }
    window.setFramerateLimit(activeLimit);
    entries[0] = "Frame rate limit: " + std::to_string(activeLimit);
  }
  void toggleVsync()
  {
    vsync = !vsync;
    window.setVerticalSyncEnabled(vsync);
    if (vsync)
      entries[1] = "VSync: On";
    else
      entries[1] = "VSync: Off";
  }
};

class SaveScore : public GameOver
{
public:
  SaveScore(int EntriesCount, sf::String entries[])
    : GameOver(EntriesCount, entries){};
  void click()
  {
    if (activeEntry == 0)
      int x; // save score
             // else click(active) to delete entry
  }
};