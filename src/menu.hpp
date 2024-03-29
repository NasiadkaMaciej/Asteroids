#include <SFML/Graphics.hpp>
#include "control.hpp"
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
	file.close();
};

// Load score board from file if exists,
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
		writeScoreBoard();
	}

	std::sort(scoreBoard, scoreBoard + 10, [&](ScoreBoard a, ScoreBoard b)
			  { return a.points > b.points; });
};

// Open link in browser for every operating system
void openInBrowser(const std::string &p)
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
	std::system(str.append(" " + p).c_str());
}

#define up 1
#define down 2

const int menuEntriesCount = 5, gameOverEntriesCount = 3,
		  settingEntriesCount = 9, saveScoreEntriesCount = 13,
		  leaderBoardEntriesCount = 12;
std::string returnBool(int value)
{
	if (value == 0)
		return "Off";
	else if (value == 1)
		return "On";
	return "Error";
}
// Struct? Some array?
std::string menuEntries[menuEntriesCount]{"Play",
										  "Leaderboard",
										  "Settings",
										  "Info",
										  "Exit"},
	gameOverEntries[gameOverEntriesCount]{"Your score", "New game", "Menu"},
	settingEntries[settingEntriesCount]{"Frame rate limit: " + std::to_string(gameSettings.frames),
										"VSync: " + returnBool(gameSettings.vsync),
										"Fullscreen: " + returnBool(gameSettings.fs),
										"Sound: " + returnBool(gameSettings.sfx),
										"Music: " + returnBool(gameSettings.music),
										"Background: " + returnBool(gameSettings.background),
										"Resolution: " + std::to_string(gameSettings.resX) + "x" + std::to_string(gameSettings.resY),
										"Antialiasing: " + std::to_string(gameSettings.antialias),
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
				sf::Vector2f(gameSettings.resX / 2,
							 gameSettings.resY / (entriesCount + 1) * (i + 1)));
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
			else
				activeEntry = entriesCount - 1;
			break;
		case down:
			if (activeEntry + 1 < entriesCount)
				activeEntry++;
			else
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
			bool clicked = false;
			if (event.type == sf::Event::Closed)
				window.close();
			if (delta->Menu > 100)
			{ // dissalow too quick movement and prevent double clicks
				if (CONTROL::isESC())
				{
					if (activeState == menuState)
						setState(playState);
					else if (activeState == saveScreenState)
						setState(gameoverState);
					else
						setState(menuState);
					clicked = true;
				}
				if (CONTROL::isUP())
				{
					move(up);
					clicked = true;
				}
				else if (CONTROL::isDOWN())
				{
					move(down);
					clicked = true;
				};
				if (CONTROL::isEnter() || CONTROL::isSpace())
				{
					click();
					clicked = true;
				}
				delta->Menu = 0;
			}
			if (CONTROL::isClick())
			{
				mouseClick();
				clicked = true;
			}
			if (clicked)
				playSound(&menuSound);

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta > 0)
					move(up);
				else if (event.mouseWheelScroll.delta < 0)
					move(down);
			}
		}

		window.clear();
		draw(window);
		window.draw(newVersion);
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
			openInBrowser("https://github.com/NasiadkaMaciej/Asteroids");
			break;
		case 4:
			window.close();
			break;
		}
	}
	virtual void mouseClick()
	{ // Select entry or click selected
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
						move(up);
					while (activeEntry != i);
			}
		}
	}
	virtual ~Menu()
	{
		delete[] entryText;
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
		move(0);
	}
};

class Settings : public Menu
{
public:
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
			toggleSFX();
			break;
		case 4:
			toggleMusic();
			break;
		case 5:
			toggleBackground();
			break;
		case 6:
			switchResolution();
			break;
		case 7:
			switchAA();
			break;
		case 8:
			setState(menuState);
			break;
		}
		move(0);
		gameSettings.saveSettings();
	}
	void switchRefreshRate()
	{
		switch (gameSettings.frames)
		{
		case 60:
			gameSettings.frames = 75;
			break;
		case 75:
			gameSettings.frames = 120;
			break;
		case 120:
			gameSettings.frames = 144;
			break;
		case 144:
			gameSettings.frames = 165;
			break;
		case 165:
			gameSettings.frames = 500;
			break;
		case 500:
			gameSettings.frames = 1000;
			break;
		case 1000:
			gameSettings.frames = 60;
			break;
		}
		window.setFramerateLimit(gameSettings.frames);
		entries[0] = "Frame rate limit: " + std::to_string(gameSettings.frames);
	}
	void toggleVsync()
	{
		gameSettings.vsync = !gameSettings.vsync;
		window.setVerticalSyncEnabled(gameSettings.vsync);
		entries[1] = "VSync: " + returnBool(gameSettings.fs);
	}
	void toggleFS()
	{
		gameSettings.fs = !gameSettings.fs;
		gameSettings.reloadWindow();
		entries[2] = "Fullscreen: " + returnBool(gameSettings.fs);
	}
	void toggleSFX()
	{
		gameSettings.sfx = !gameSettings.sfx;
		entries[3] = "Sound: " + returnBool(gameSettings.sfx);
	}
	void toggleMusic()
	{
		gameSettings.music = !gameSettings.music;
		entries[4] = "Music: " + returnBool(gameSettings.music);
		playMusic();
	}
	void toggleBackground()
	{
		gameSettings.background = !gameSettings.background;
		entries[5] = "Background: " + returnBool(gameSettings.background);
	}
	void switchResolution()
	{
		if (gameSettings.availRes.end()->width == gameSettings.resX && gameSettings.availRes.end()->height == gameSettings.resY)
		{
			gameSettings.resX = gameSettings.availRes.begin()->width;
			gameSettings.resY = gameSettings.availRes.begin()->height;
		}
		else
			for (auto it = gameSettings.availRes.begin(); it != gameSettings.availRes.end(); it++)
				if (it->width == gameSettings.resX && it->height == gameSettings.resY)
				{
					it++;
					gameSettings.resX = it->width;
					gameSettings.resY = it->height;
					break;
				}
		entries[6] = "Resolution: " + std::to_string(gameSettings.resX) + "x" + std::to_string(gameSettings.resY);
	}
	void switchAA()
	{
		if (gameSettings.antialias == 0)
			gameSettings.antialias = 2;
		else if (gameSettings.antialias < 16)
			gameSettings.antialias = gameSettings.antialias * 2;
		else if (gameSettings.antialias == 16)
			gameSettings.antialias = 0;

		entries[7] = "Antialiasing: " + std::to_string(gameSettings.antialias);
		gameSettings.reloadWindow();
	}
};

class SaveScore : public Menu
{
public:
	std::string name;
	unsigned int points;
	bool isSaving = true, wasSaved = false;
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
			openInBrowser("https://nasiadka.pl/asteroids/");
			break;
		case 12:
			setState(menuState);
			break;
		}
	}
	void setScore(int POINTS)
	{
		for (int i = 1; i < 11; i++)
			entries[i] = scoreBoard[i - 1].toString();

		points = POINTS;
		entries[0] = "Your score " + std::to_string(points);
		entries[11] = "Global leaderboard";
		entries[12] = "Menu";
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
					if (event.type == sf::Event::TextEntered)
					{
						//
						if ((event.text.unicode != 8 && name.length() <= 18 && std::isalnum(event.text.unicode)) ||
							(event.text.unicode == 32 && name.length() <= 18))
						{
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
					if (CONTROL::isEnter() && name != "" && !wasSaved)
					{
						// If last entry in leaderboard is lower, replace it with current score
						if (scoreBoard[9].points < points)
						{
							scoreBoard[9] = {points, name};
							isSaving = false;
							wasSaved = true;
							std::sort(scoreBoard, scoreBoard + 10, [&](ScoreBoard a, ScoreBoard b)
									  { return a.points > b.points; });
						}
						// Show 10 scores and "Your score" at the top
						for (int i = 1; i < 11; i++)
							entries[i] = scoreBoard[i - 1].toString();
						entries[0] = "Your score " + std::to_string(points);
						move(0);

						// Save score to online leaderboard, secret is only in built releases.
						std::string const secret = "PapiezPolak";
						if (secret != "")
						{
							transform(name.begin(), name.end(), name.begin(), ::tolower);
							std::string request = "name=" + name + "&points=" + std::to_string(points) + "&secret=" + secret;
							const char *cRequest = request.c_str();
							CURL *curl;
							curl_global_init(CURL_GLOBAL_ALL);
							curl = curl_easy_init();
							CURLcode res;
							if (curl)
							{
								curl_easy_setopt(curl, CURLOPT_URL, "https://nasiadka.pl/asteroids/send");
								curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cRequest);
								res = curl_easy_perform(curl);
								if (res != CURLE_OK)
									fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
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
	void reset()
	{
		Menu::reset();
		wasSaved = false;
		isSaving = true;
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
			openInBrowser("https://nasiadka.pl/asteroids/");
			break;
		case 11:
			setState(menuState);
			break;
		}
	}
	void setScore()
	{
		for (int i = 0; i < 10; i++)
			entries[i] = scoreBoard[i].toString();
		entries[10] = "Global leaderboard";
		entries[11] = "Menu";
		move(0);
	}
};

struct ProgressBar
{
public:
	float fullSize;
	sf::RectangleShape pg;
	float objectSize;
	float numOfObjects;
	float removedObjects = 0;
	float height;
	ProgressBar(float HEIGHT)
		: height(HEIGHT), fullSize(gameSettings.resX / 2)
	{
		pg.setFillColor(sf::Color::Black);
		pg.setOutlineThickness(1);
		pg.setOutlineColor(sf::Color::White);
	}

	void retractPoint()
	{
		numOfObjects--;
		removedObjects += objectSize;
	}
	void update()
	{
		pg.setSize(sf::Vector2f(fullSize - removedObjects, height));
		pg.setOrigin(pg.getGlobalBounds().width / 2, pg.getGlobalBounds().height / 2);
		pg.setPosition(window.getView().getCenter().x, window.getView().getCenter().y * 1.95);
	}
	void reset()
	{
		removedObjects = 0;
		numOfObjects = gameVal->bigAsteroids + gameVal->bigAsteroids * 2 + gameVal->bigAsteroids * 4;
		objectSize = fullSize / numOfObjects;
	}
};
void mute(Settings *s)
{
	if (CONTROL::mute() && delta->Menu > 300)
	{
		s->toggleMusic();
		delta->Menu = 0;
		gameSettings.saveSettings();
	}
}

ProgressBar progressBar(15), placeholder(0);