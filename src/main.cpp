#include "entities/Asteroid.hpp"
#include "entities/Player.hpp"
#include "entities/PowerUp.hpp"
#include "entities/UFO.hpp"
#include "entities/utils.hpp"
#include "ui/GameOver.hpp"
#include "ui/LeaderBoard.hpp"
#include "ui/MainMenu.hpp"
#include "ui/MenuUtils.hpp"
#include "ui/ProgressBar.hpp"
#include "ui/SaveScore.hpp"
#include "ui/Settings.hpp"
#include "utils/Collision.h"
#include "utils/GameSettings.hpp"
#include "utils/control.hpp"
#include "utils/sounds.hpp"
#include "utils/textures.hpp"
#include <list>
#include <memory>
#include <thread>

// Game entities
std::unique_ptr<Player> p;
std::unique_ptr<UFO> u;

// Entity collections
std::list<std::unique_ptr<Entity>> asteroids;
std::list<std::unique_ptr<Entity>> bullets;
std::list<std::unique_ptr<Entity>> powerUps;

// Forward declarations
void checkCollision();
void resetGame(ProgressBar& progressBar);
void updateEntities(std::list<std::unique_ptr<Entity>>& list, ProgressBar& progressBar);
void handleGlobalControls();
void loadGameResources();
void runGameLoop();
void handlePlayState(ProgressBar& progressBar, sf::Sprite& background, sf::Text& text);

int main() {
	// Load all game resources
	if (!loadBase() || !loadTextures() || !loadSounds()) return EXIT_FAILURE;

	// Start version checking in background
	std::thread versionChecker(checkVersion);
	versionChecker.detach();

	// Initialize game resources
	loadGameResources();

	// Create game objects
	sf::Sprite background(tBackground);
	background.setTextureRect(
	  sf::IntRect({ 0, 0 }, { static_cast<int>(gameSettings.resX), static_cast<int>(gameSettings.resY) }));

	ProgressBar progressBar(15);
	ProgressBar placeholder(0);
	placeholder.pg.setFillColor(sf::Color::White);
	placeholder.update();

	// Initialize game state
	resetGame(progressBar);

	// Run the main game loop
	runGameLoop();

	return 0;
}

void loadGameResources() {
	playMusic();
	loadScoreBoard();
	initializeSpeedValues();
}

void resetGame(ProgressBar& progressBar) {
	// Clear entity collections
	asteroids.clear();
	bullets.clear();
	powerUps.clear();

	// Reset game values and timers
	delete gameVal;
	delete delta;

	// Create new objects with raw pointers
	gameVal = new GameValues();
	delta = new GameTime();

	// Create player and UFO
	p = std::make_unique<Player>();
	u = std::make_unique<UFO>();

	// Reset progress bar
	progressBar.reset();
}

void updateEntities(std::list<std::unique_ptr<Entity>>& list, ProgressBar& progressBar) {
	for (auto i = list.begin(); i != list.end();) {
		Entity* e = i->get();
		e->update();
		if (!e->life) {
			Asteroid* asteroid = dynamic_cast<Asteroid*>(e);
			if (asteroid) {
				// Generate smaller asteroids after being hit and give points
				if (*e == &tAsteroid[BIG]) {
					p->givePoints(20);
					list.emplace_back(Asteroid::generate(e->x, e->y, MEDIUM));
					list.emplace_back(Asteroid::generate(e->x, e->y, MEDIUM));
					progressBar.retractPoint();
				} else if (*e == &tAsteroid[MEDIUM]) {
					p->givePoints(50);
					list.emplace_back(Asteroid::generate(e->x, e->y, SMALL));
					list.emplace_back(Asteroid::generate(e->x, e->y, SMALL));
					progressBar.retractPoint();
				} else if (*e == &tAsteroid[SMALL]) {
					p->givePoints(100);
				}
			}
			i = list.erase(i);
		} else
			++i;
	}
}

void handleGlobalControls() {
	if (CONTROL::mute() && delta->Menu > 300) {
		gameSettings.music = !gameSettings.music;
		playMusic();
		gameSettings.saveSettings();
		delta->Menu = 0;
	}

	if (CONTROL::isFS() && delta->Menu > 300) {
		gameSettings.fs = !gameSettings.fs;
		gameSettings.reloadWindow();
		gameSettings.saveSettings();
		delta->Menu = 0;
	}
}

void handlePlayState(ProgressBar& progressBar, sf::Sprite& background, sf::Text& text) {
	// Handle collisions in a separate thread
	std::thread collisionThread(checkCollision);

	// Process window events
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) window.close();
		if (CONTROL::isESC()) setState(menuState);
		if (CONTROL::isReset()) resetGame(progressBar);
		p->getControl();
	}

	// Player actions
	p->shoot(&bullets);

	// Spawn random power ups
	PowerUp::generate(&powerUps, p);

	// UFO actions
	u->activate();

	// Wait for collision detection to finish
	collisionThread.join();

	// Update all entities
	updateEntities(asteroids, progressBar);
	updateEntities(bullets, progressBar);
	updateEntities(powerUps, progressBar);
	updateEntities(u->ufoBullets, progressBar);

	// Update player and UFO
	p->update();
	u->update(p->x, p->y);

	// Reset UFO if destroyed
	if (!u->life) {
		delta->UFO = 0;
		u = std::make_unique<UFO>();
		p->givePoints(1000);
	}

	// Start new level after clearing all asteroids
	if (asteroids.empty()) {
		gameVal->bigAsteroids += 2;
		gameVal->roundNum++;
		for (int i = 0; i < gameVal->bigAsteroids; i++)
			asteroids.emplace_back(Asteroid::generateBig());
		progressBar.reset();
	}

	// Update UI elements
	progressBar.update();

	// Format display text
	std::string sPoints = std::to_string(p->points);
	std::string sTime = std::to_string(p->aliveTime / 10);
	std::string sLevel = std::to_string(gameVal->roundNum);
	std::string sLifes = std::to_string(p->lifes) + (p->lifes == 1 ? " life" : " lifes");

	if (sTime.length() > 2)
		sTime.insert(sTime.length() - 2, ".");
	else
		sTime = "0." + sTime;

	text.setString(sTime + " sec" + "\n" + sPoints + " points" + "\n" + "Round: " + sLevel + "\n" + sLifes);

	// Render game state
	window.clear();

	if (gameSettings.background) window.draw(background);

	// Draw all entities
	for (auto& i : asteroids)
		i->draw(window);
	for (auto& i : bullets)
		i->draw(window);
	for (auto& i : powerUps)
		i->draw(window);
	for (auto& i : u->ufoBullets)
		i->draw(window);

	// Draw player and UFO
	p->draw(window);
	if (u->isActive) u->draw(window);

	// Draw UI elements
	window.draw(text);
	window.draw(placeholder.pg);
	window.draw(progressBar.pg);

	window.display();
	delta->Move = 0;
}

void runGameLoop() {
	// Create menu objects
	MainMenu mainMenu;
	Settings settingsMenu;
	GameOver gameOverMenu;
	LeaderBoard leaderBoardMenu;
	SaveScore saveScoreMenu;

	// Create game objects
	sf::Sprite background(tBackground);
	background.setTextureRect(
	  sf::IntRect({ 0, 0 }, { static_cast<int>(gameSettings.resX), static_cast<int>(gameSettings.resY) }));

	ProgressBar progressBar(15);
	ProgressBar placeholder(0);
	placeholder.pg.setFillColor(sf::Color::White);
	placeholder.update();

	// Main game loop
	while (window.isOpen()) {
		// Handle global controls
		handleGlobalControls();

		// Update timers
		delta->update();

		// State machine for game states
		switch (activeState) {
		case menuState:
			mainMenu.show();
			if (p->lifes <= 0) resetGame(progressBar);
			if (versionCheckComplete) window.draw(newVersion);
			break;

		case gameoverState:
			gameOverMenu.setScore(p->points);
			gameOverMenu.show();
			if (activeState == playState) resetGame(progressBar);
			break;

		case settingsState:
			settingsMenu.show();
			break;

		case saveScreenState:
			saveScoreMenu.setScore(p->points);
			saveScoreMenu.show();
			break;

		case leaderBoardState:
			leaderBoardMenu.show();
			break;

		case playState:
			handlePlayState(progressBar, background, text);
			break;
		}
	}
}

void checkCollision() {
	// Check asteroid collisions
	for (auto& a : asteroids) {
		// Check bullets vs asteroids
		for (auto& b : bullets) {
			if (Collision::PixelPerfectTest(a->sprite, b->sprite) && b->life) {
				playSound(&destroySound);
				a->life = false;
				b->lifes--;
				if (!b->lifes) b->life = false;
				progressBar.retractPoint();
				break;
			}
		}

		// Check player vs asteroids
		if (Collision::PixelPerfectTest(a->sprite, p->sprite) && !p->isIdle) {
			playSound(&deathSound);
			a->life = false;
			p->life = false;
			progressBar.retractPoint();
			continue;
		}
	}

	// Check power-up collisions
	for (auto& powerUp : powerUps) {
		if (Collision::PixelPerfectTest(powerUp->sprite, p->sprite) && !p->isIdle) {
			powerUp->life = false;
			delta->PowerUp = 0;

			const sf::Texture& tex = powerUp->sprite.getTexture();
			if (&tex == &tBulletUp)
				p->isPowerBullet = true;
			else if (&tex == &tLifeUp)
				p->lifes++;
			else if (&tex == &tDoubleBullet)
				p->isDoubleShooting = true;
			else if (&tex == &tPenetratingBullet)
				p->isDoublePenetrating = true;
		} else if (delta->PowerUp > gameVal->powerUpRestore) {
			powerUp->life = false;
			delta->PowerUp = 0;
		}
	}

	// Check UFO bullet collisions
	for (auto& ufoBullet : u->ufoBullets) {
		if (Collision::PixelPerfectTest(ufoBullet->sprite, p->sprite) && !p->isIdle) {
			playSound(&destroySound);
			ufoBullet->life = false;
			p->life = false;
			ufoBullet->lifes--;
			if (!ufoBullet->lifes) ufoBullet->life = false;
			if (!p->lifes) p->life = false;
		}
	}

	// Check player bullet vs UFO collisions
	for (auto& bullet : bullets) {
		if (Collision::PixelPerfectTest(bullet->sprite, u->sprite) && !p->isIdle && u->isActive) {
			playSound(&destroySound);
			u->lifes--;
			bullet->lifes--;
			if (!bullet->lifes) bullet->life = false;
			if (!u->lifes) u->life = false;
		}
	}
}