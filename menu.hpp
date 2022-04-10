#include <SFML/Graphics.hpp>
#include <list>
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

const int menuEntriesCount = 5, gameOverEntriesCount = 3,
          saveScoreEntriesCount = 11;
sf::String menuEntries[menuEntriesCount]{ "Play",
                                          "Leaderboard",
                                          "Settings",
                                          "Info",
                                          "Exit" },
  gameOverEntries[gameOverEntriesCount]{ "Your score", "New game", "Menu" },
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
        if (activeEntry - 1 >= 0) {
          entryText[activeEntry].setFillColor(sf::Color::White);
          activeEntry--;
          entryText[activeEntry].setFillColor(sf::Color::Red);
        } else if (activeEntry - 1 < 0) {
          entryText[activeEntry].setFillColor(sf::Color::White);
          activeEntry = entriesCount - 1;
          entryText[activeEntry].setFillColor(sf::Color::Red);
        }
        break;
      case down:
        if (activeEntry + 1 < entriesCount) {
          entryText[activeEntry].setFillColor(sf::Color::White);
          activeEntry++;
          entryText[activeEntry].setFillColor(sf::Color::Red);
        } else if (activeEntry + 1 >= entriesCount) {
          entryText[activeEntry].setFillColor(sf::Color::White);
          activeEntry = 0;
          entryText[activeEntry].setFillColor(sf::Color::Red);
        }
        break;
    }
    // format and color entries
    for (int i = 0; i < entriesCount; i++) {
      entryText[i].setString(entries[i]);
      entryText[i].setOrigin(entryText[i].getGlobalBounds().width / 2,
                             entryText[i].getGlobalBounds().height / 2);
    }
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
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        move(up);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        move(down);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        click();
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
      case 3:
        openInBrowser();
        break;
      case 4:
        window.close();
        break;
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