#include <SFML/Graphics.hpp>
#include <list>
#include <string>

#include <stdio.h>

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

struct ScoreBoard
{
  unsigned int points;
  std::string name;
  std::string toString() { return std::to_string(points) + " - " + name; }
};

bool
compare(ScoreBoard a, ScoreBoard b)
{
  if (a.points < b.points)
    return false;
  else
    return true;
}
ScoreBoard scoreBoard[10];

void
loadScoreBoard()
{
  FILE* fin = fopen("scoreBoard.dat", "r");
  for (int i = 0; i < 10; i++)
    fread(&scoreBoard[i], sizeof(scoreBoard), 1, fin);
  fclose(fin);

  /*    for (int i = 0; i < 10; i++)
      scoreBoard[i] = { (unsigned int)std::rand() % 50, "Macieson" };  */

  std::sort(scoreBoard, scoreBoard + 10, compare);
};
void
writeScoreBoard()
{
  /*   for (int i = 0; i < 10; i++) {
      scoreBoard[i] = { 0, "" };
    } */
  FILE* fout = fopen("scoreBoard.dat", "w");
  for (int i = 0; i < 10; i++)
    fwrite(&scoreBoard[i], sizeof(ScoreBoard), 1, fout);
  fclose(fout);
};
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
          settingEntriesCount = 3, saveScoreEntriesCount = 12,
          leaderBoardEntriesCount = 11;
std::string menuEntries[menuEntriesCount]{ "Play",
                                           "Leaderboard",
                                           "Settings",
                                           "Info",
                                           "Exit" },
  gameOverEntries[gameOverEntriesCount]{ "Your score", "New game", "Menu" },
  settingEntries[settingEntriesCount]{ "Frame rate limit: 60",
                                       "VSync: On",
                                       "Menu" },
  saveScoreEntries[saveScoreEntriesCount],
  leaderBoardEntries[leaderBoardEntriesCount];

class Menu
{
public:
  int activeEntry = 0;
  // eventually
  int entriesCount;
  sf::Text* entryText;
  std::string* entries;
  Menu(int ENTRIESCOUNT, std::string ENTRIES[])
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
  void reset()
  {
    do
      move(up);
    while (activeEntry != 0);
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
      case 1:
        setState(leaderBoardState);
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
  GameOver(int EntriesCount, std::string entries[])
    : Menu(EntriesCount, entries){};
  void click()
  {
    switch (activeEntry) {
      case 0:
        setState(saveScreenState);
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
  Settings(int EntriesCount, std::string entries[])
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
    move(0);
  }
  void toggleVsync()
  {
    vsync = !vsync;
    window.setVerticalSyncEnabled(vsync);
    if (vsync)
      entries[1] = "VSync: On";
    else
      entries[1] = "VSync: Off";
    move(0);
  }
};

class SaveScore : public GameOver
{
public:
  std::string name;
  unsigned int points;
  bool isSaving = true, wasSaved = false;
  //  int iNameChar, activeChar = 0;
  SaveScore(int EntriesCount, std::string entries[])
    : GameOver(EntriesCount, entries){};
  void click()
  {
    switch (activeEntry) {
      case 0:
        saveScore();
        break;
      case 11:
        setState(menuState);
        break;
      default:
        // delete active entry
        break;
    }
  }
  void setScore(int POINTS)
  {
    for (int i = 1; i < 11; i++)
      entries[i] = scoreBoard[i - 1].toString();

    points = POINTS;
    entries[0] = "Your score " + std::to_string(points);
    entries[11] = "Menu";

    move(0);
    name = "";
  }
  void saveScore()
  {
    move(0);
    if (!wasSaved) {
      entries[0] = "Your name: ";
      name = "";
      move(0);
      sf::Event event;

      while (isSaving) {
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed)
            window.close();
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            isSaving = false;
          if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128 && event.text.unicode != 8 &&
                event.text.unicode != 13) {
              //&& std::isprint(event.text.unicode)) {
              name += event.text.unicode;
              entries[0] += event.text.unicode;
            } else if (event.text.unicode == 8) {
              if (name.length() > 0)
                name.pop_back();
              if (entries[0] != "Your name: " + name)
                entries[0].pop_back();
            }
            move(0);
          }
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && name != "" &&
              !wasSaved) {
            if (scoreBoard[9].points < points) {
              scoreBoard[9] = { points, name };
              isSaving = false;
              wasSaved = true;
            }
            std::sort(scoreBoard, scoreBoard + 10, compare);

            for (int i = 1; i < 11; i++)
              entries[i] = scoreBoard[i - 1].toString();
            entries[0] = "Your score " + std::to_string(points);
            move(0);
            isSaving = false;
            wasSaved = true;
          }
        }
        window.clear();
        draw(window);
        window.display();
      }
    }
    writeScoreBoard();
  }
};

class LeaderBoard : public GameOver
{
public:
  LeaderBoard(int EntriesCount, std::string entries[])
    : GameOver(EntriesCount, entries){};
  void click()
  {
    switch (activeEntry) {
      case 10:
        setState(menuState);
        break;
    }
  }
  void setScore()
  {
    for (int i = 0; i < 10; i++)
      entries[i] = scoreBoard[i].toString();
    entries[10] = "Menu";
    move(0);
  }
};