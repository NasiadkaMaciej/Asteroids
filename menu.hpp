#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <string>

struct ScoreBoard
{
	unsigned int points;
	std::string name;
	std::string toString()
	{
		return std::to_string(points) + " - " + name;
	}
};

ScoreBoard scoreBoard[10];

// Compare elements in ScoreBoard to sort them
bool compare(ScoreBoard a, ScoreBoard b)
{
	if (a.points < b.points)
		return false;
	else
		return true;
}

// Load score board from file if exists,
// todo: else create file with blank entries
void loadScoreBoard()
{
	std::ifstream file("scoreBoard.dat");
	if (file.is_open())
	{
		std::string tmpString;
		int i = 0;
		while (file.good())
		{
			std::getline(file, tmpString, ':');
			scoreBoard[i].points = stoi(tmpString);
			std::getline(file, tmpString);
			scoreBoard[i].name = tmpString;
			i++;
		}
		file.close();
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			scoreBoard[i].points = 0;
			scoreBoard[i].name = "";
		}
	}
	std::sort(scoreBoard, scoreBoard + 10, compare);
};

// Write actual score board to file
void writeScoreBoard()
{
	std::ofstream file("scoreBoard.dat");
	if (file.is_open())
	{
		for (int i = 0; i < 10; i++)
		{
			file << scoreBoard[i].points << ":";
			file << scoreBoard[i].name;
			if (i < 9)
				file << "\n";
		}
	}
};

// Open link in browser for every operating system
void openInBrowser()
{
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

	std::string str;
	std::string p = "https://github.com/NasiadkaMaciej/Asteroids";
	if (platform)
		switch (platform)
		{
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
		  settingEntriesCount = 4, saveScoreEntriesCount = 12,
		  leaderBoardEntriesCount = 11;
std::string menuEntries[menuEntriesCount]{"Play",
										  "Leaderboard",
										  "Settings",
										  "Info",
										  "Exit"},
	gameOverEntries[gameOverEntriesCount]{"Your score", "New game", "Menu"},
	settingEntries[settingEntriesCount]{"Frame rate limit: 60",
										"VSync: On",
										"Fullscreen: On",
										"Menu"},
	saveScoreEntries[saveScoreEntriesCount],
	leaderBoardEntries[leaderBoardEntriesCount];

// General class for all menus
class Menu
{
public:
	int activeEntry = 0;
	// eventually
	int entriesCount;
	sf::Text *entryText;
	std::string *entries;
	Menu(int ENTRIESCOUNT, std::string ENTRIES[])
	{
		entries = ENTRIES;
		entriesCount = ENTRIESCOUNT;
		entryText = new sf::Text[entriesCount];
		for (int i = 0; i < entriesCount; i++)
		{
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
		entryText[activeEntry].setFillColor(sf::Color::Red);
		entryText[activeEntry].setString("> " + entryText[activeEntry].getString() +
										 " <");
		entryText[activeEntry].setOrigin(
			entryText[activeEntry].getGlobalBounds().width / 2,
			entryText[activeEntry].getGlobalBounds().height / 2);
	}
	void draw(sf::RenderWindow &window)
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
		switch (direction)
		{
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
		for (int i = 0; i < entriesCount; i++)
		{
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
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				setState(playState);
			if (deltaMenu >=
				100)
			{ // dissalow too quick movement and prevent double clicks
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					move(up);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					move(down);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
					click();
				deltaMenu = 0;
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
		switch (activeEntry)
		{
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

		for (int i = 0; i < entriesCount; i++)
		{
			if (entryText[i].getGlobalBounds().contains(translated_pos))
			{
				if (activeEntry == i)
					click();
				else
					do
					{
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
		switch (activeEntry)
		{
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
		switch (activeEntry)
		{
		case 0:
			switchRefreshRate();
			break;
		case 1:
			toggleVsync();
			break;
		case 2:
			toggleFS();
			break;
		case 3:
			setState(menuState);
			break;
		}
	}
	void switchRefreshRate()
	{
		switch (activeLimit)
		{
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
	void toggleFS()
	{
		if (isFS)
		{
			window.create(sf::VideoMode(desktopMode.width,
										desktopMode.height,
										desktopMode.bitsPerPixel),
						  "Asteroids - Macieson"); //,
												   // sf::Style::Fullscreen);
			entries[2] = "Fullscreen: Off";
			isFS = false;
		}
		else
		{
			window.create(sf::VideoMode(desktopMode.width,
										desktopMode.height,
										desktopMode.bitsPerPixel),
						  "Asteroids - Macieson",
						  sf::Style::Fullscreen);
			entries[2] = "Fullscreen: On";

			isFS = true;
			;
		}
		move(0);
	}
};

class SaveScore : public Menu
{
public:
	std::string name;
	unsigned int points;
	bool isSaving = true, wasSaved = false;
	//  int iNameChar, activeChar = 0;
	SaveScore(int EntriesCount, std::string entries[])
		: Menu(EntriesCount, entries){};
	void click()
	{
		switch (activeEntry)
		{
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
		if (!wasSaved)
		{
			entries[0] = "Your name: ";
			name = "";
			move(0);
			sf::Event event;

			while (isSaving)
			{
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						isSaving = false;
					if (event.type == sf::Event::TextEntered)
					{
						if (event.text.unicode < 128 && event.text.unicode != 8 &&
							event.text.unicode != 13)
						{
							//&& std::isprint(event.text.unicode)) {
							name += event.text.unicode;
							entries[0] += event.text.unicode;
						}
						else if (event.text.unicode == 8)
						{
							if (name.length() > 0)
								name.pop_back();
							if (entries[0] != "Your name: " + name)
								entries[0].pop_back();
						}
						move(0);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && name != "" &&
						!wasSaved)
					{
						if (scoreBoard[9].points < points)
						{
							scoreBoard[9] = {points, name};
							isSaving = false;
							wasSaved = true;
						}
						std::sort(scoreBoard, scoreBoard + 10, compare);

						for (int i = 1; i < 11; i++)
							entries[i] = scoreBoard[i - 1].toString();
						entries[0] = "Your score " + std::to_string(points);
						move(0);

						// Save score to online leaderboard, secret is only in built releases.
						std::string secret = "";
						if (secret != "")
						{
							std::string request = "name=" + name + "&points=" + std::to_string(points) + "&secret=" + secret;
							const char *cRequest = request.c_str();
							std::cout << request;
							CURL *curl;
							CURLcode res;

#ifdef _WIN32
							curl_global_init(CURL_GLOBAL_ALL);
#endif
							/* get a curl handle */
							curl = curl_easy_init();
							if (curl)
							{
								/* First set the URL that is about to receive our POST. This URL can
								   just as well be a https:// URL if that is what should receive the
								   data. */
								curl_easy_setopt(curl, CURLOPT_URL, "https://maciej.ml/Asteroids/");
								/* Now specify the POST data */
								curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cRequest);

								/* Perform the request, res will get the return code */
								res = curl_easy_perform(curl);
								/* Check for errors */
								if (res != CURLE_OK)
									fprintf(stderr, "curl_easy_perform() failed: %s\n",
											curl_easy_strerror(res));

								/* always cleanup */
								curl_easy_cleanup(curl);
							}
							curl_global_cleanup();
						}
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
		switch (activeEntry)
		{
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