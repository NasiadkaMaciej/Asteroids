#include "GameSettings.hpp"
#include <iostream>
#include <fstream>
#include <curl/curl.h>

sf::VideoMode desktopMode;
sf::RenderWindow window;
sf::Font font;
sf::Text text;

std::string data;

size_t writeCallback(char *buf, size_t size, size_t nmemb, void *up)
{ // callback must have this declaration
	// buf is a pointer to the data that curl has for us
	// size*nmemb is the size of the buffer
	for (int c = 0; c < size * nmemb; c++)
		data.push_back(buf[c]);
	return size * nmemb; // tell curl how many bytes we handled
}

bool checkVersion() // Compare local version of the game with the published one
{
	CURL *curl;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "https://maciej.ml/Asteroids/ver");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return GAME_VERSION == std::stoi(data);
}

sf::Text newVersion;
float screenScale;
// Use map instead
bool isPlaying = false, isMenu = true, isGameOver = false, isSettings = false,
	 isSaveScreen = false, isLeaderBoard = false;
void GameSettings::loadSettings()
{
	// Read all available fullscreen resolutions and sort them
	for (const auto &tmp : sf::VideoMode::getFullscreenModes())
		if (tmp.bitsPerPixel == 24)
			availRes.emplace_back(tmp);
	std::reverse(availRes.begin(), availRes.end());

	if (!checkVersion())
	{
		newVersion.setString("New version is available");
		newVersion.setPosition(30, 30);
		newVersion.setFont(font);
		newVersion.setCharacterSize(25);
		newVersion.setFillColor(sf::Color::White);
	}

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

		screenScale = (float)resY / 2000;
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

	screenScale = (float)resY / 2000;
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
GameTime *delta;
GameSettings gameSettings;

bool loadBase()
{
	if (!gameSettings.checkRes()) // Check if is fs and resolutions are in valid video modes
	{
		std::cout << "Invalid fullscreen resolution. Available modes are: \n";
		for (const auto &videoMode : sf::VideoMode::getFullscreenModes())
			if (videoMode.bitsPerPixel == 24)
				std::cout << videoMode.width << "x" << videoMode.height << "\n";
		return false;
	}
	desktopMode = sf::VideoMode::getDesktopMode();
	window.create(sf::VideoMode(gameSettings.resX,
								gameSettings.resY,
								desktopMode.bitsPerPixel),
				  "Asteroids - Macieson", gameSettings.translateFS(), sf::ContextSettings(24, 8, 16));
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

char activeState = menuState;

void setState(eStates state)
{
	if (state == playState)
		window.setMouseCursorVisible(false);
	else
		window.setMouseCursorVisible(true);
	activeState = state;
	delta->Clock.restart();
}
