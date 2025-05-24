#include "entities/Asteroid.hpp"
#include "entities/Bullet.hpp"
#include "entities/Player.hpp"
#include "entities/PowerUp.hpp"
#include "entities/UFO.hpp"
#include "entities/utils.hpp"
#include "ui/GameOver.hpp"
#include "ui/GlobalMenuData.hpp"
#include "ui/LeaderBoard.hpp"
#include "ui/Menu.hpp"
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

std::unique_ptr<Player> p;
std::unique_ptr<UFO> u;

std::list<std::unique_ptr<Entity>> asteroids;
std::list<std::unique_ptr<Entity>> bullets;
std::list<std::unique_ptr<Entity>> powerUps;

int main() {

	// load all assets TODO: Check if textues and ScoreBoard is ok
	if (!loadBase() || !loadTextures() || !loadSounds()) return EXIT_FAILURE;
	std::thread versionChecker(checkVersion);
	playMusic();
	loadScoreBoard();
	// writeScoreBoard();

	// create objects and lists
	Menu menu(menuEntriesCount, menuEntries);
	GameOver gameOver(gameOverEntriesCount, gameOverEntries);
	Settings settings(settingEntriesCount, settingEntries);
	SaveScore saveScore(saveScoreEntriesCount, saveScoreEntries);
	LeaderBoard leaderBoard(leaderBoardEntriesCount, leaderBoardEntries);
	sf::Sprite background(tBackground);
	ProgressBar progressBar(15), placeholder(0);
	background.setTextureRect(
	  sf::IntRect({ 0, 0 }, { static_cast<int>(gameSettings.resX), static_cast<int>(gameSettings.resY) }));

	placeholder.pg.setFillColor(sf::Color::White);
	placeholder.update();
	initializeSpeedValues();
	versionChecker.join();
	// resetting game to base values
	auto reset = [&]() {
		asteroids.clear();
		bullets.clear();
		powerUps.clear();
		delete gameVal;
		delete delta;
		gameVal = new GameValues;
		delta = new GameTime;
		p = std::make_unique<Player>();
		u = std::make_unique<UFO>();
		saveScore.reset();
		progressBar.reset();
	};

	auto updateList = [&](std::list<std::unique_ptr<Entity>>& list) {
		for (auto i = list.begin(); i != list.end();) {
			Entity* e = i->get();
			e->update();
			if (!e->life) {
				if (e == dynamic_cast<Asteroid*>(e)) {
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
					} else if (*e == &tAsteroid[SMALL])
						p->givePoints(100);
				}
				i = list.erase(i);
			} else
				i++;
		}
	};
	reset();

	while (window.isOpen()) {
		delta->Menu += delta->timer.ms();
		mute(&settings);
		switch (activeState) {
		case menuState:
			menu.show();
			if (p->lifes <= 0) reset();
			break;
		case gameoverState:
			gameOver.setScore(p->points);
			gameOver.show();
			if (activeState == playState) // returned to playing from some menu
				reset();
			break;
		case settingsState:
			settings.show();
			break;
		case saveScreenState:
			saveScore.setScore(p->points);
			saveScore.show();
			break;
		case leaderBoardState:
			leaderBoard.setScore();
			leaderBoard.show();
			break;
		case playState:
			delta->update();
			delta->UFO += delta->timer.ms();
			std::thread worker(checkCollision);
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) window.close();
				if (CONTROL::isESC()) setState(menuState);
				if (CONTROL::isReset()) reset();
				if (CONTROL::isFS()) settings.toggleFS();
				p->getControl();
			}
			p->shoot(&bullets);

			// Spawn random power up evey 10 seconds and clear old
			PowerUp::generate(&powerUps, p);

			u->activate();

			worker.join();
			// Update all entities and remove dead ones
			updateList(asteroids);
			updateList(bullets);
			updateList(powerUps);
			updateList(u->ufoBullets);

			p->update();
			u->update(p->x, p->y);

			if (!u->life) {
				delta->UFO = 0;
				u = std::make_unique<UFO>();
				p->givePoints(1000);
			}

			// Start new level after clearing all asteroids
			if (!asteroids.size()) {
				gameVal->bigAsteroids += 2;
				gameVal->roundNum++;
				for (int i = 0; i < gameVal->bigAsteroids; i++)
					asteroids.emplace_back(Asteroid::generateBig());
				progressBar.reset();
			}

			progressBar.update();
			std::string sPoints = std::to_string(p->points);
			std::string sTime = std::to_string(p->aliveTime / 10);
			std::string sLevel = std::to_string(gameVal->roundNum);
			std::string sLifes;
			if (p->lifes == 1)
				sLifes = std::to_string(p->lifes) + " life";
			else
				sLifes = std::to_string(p->lifes) + " lifes";

			if (sTime.length() > 2)
				sTime.insert(sTime.length() - 2, ".");
			else
				sTime = "0." + sTime;
			text.setString(sTime + " sec" + "\n" + sPoints + " points" + "\n" + "Round: " + sLevel + "\n" + sLifes);
			window.clear();
			if (gameSettings.background) window.draw(background);
			for (auto& i : asteroids)
				i->draw(window);
			for (auto& i : bullets)
				i->draw(window);
			for (auto& i : powerUps)
				i->draw(window);
			for (auto& i : u->ufoBullets)
				i->draw(window);
			p->draw(window);
			if (u->isActive) u->draw(window);
			window.draw(text);
			window.draw(placeholder.pg);
			window.draw(progressBar.pg);
			window.display();
			delta->Move = 0;
			break;
		}
	}

	asteroids.clear();
	bullets.clear();
	powerUps.clear();
	u->ufoBullets.clear();
	return 0;
}

void checkCollision() {
	for (auto& a : asteroids) {
		for (auto& b : bullets)
			// Check bullets and asteroids collisons
			if (Collision::PixelPerfectTest(a.get()->sprite, b.get()->sprite) && b.get()->life) {
				playSound(&destroySound);
				a.get()->life = false;
				b.get()->lifes--;
				if (!b->lifes) b->life = false;
				progressBar.retractPoint();
			}
		// Check asteroids and player collisions
		if (Collision::PixelPerfectTest(a.get()->sprite, p.get()->sprite) && !p->isIdle) {
			playSound(&deathSound);
			a.get()->life = false;
			p.get()->life = false;
			progressBar.retractPoint();
		}
	}
	// Check power ups and player collisions
	for (auto& a : powerUps) {
		if (Collision::PixelPerfectTest(a.get()->sprite, p.get()->sprite) && !p.get()->isIdle) {
			a.get()->life = false;
			delta->PowerUp = 0;
			if (*a == &tBulletUp)
				p.get()->isPowerBullet = true;
			else if (*a == &tLifeUp)
				p.get()->lifes++;
			else if (*a == &tDoubleBullet)
				p.get()->isDoubleShooting = true;
			else if (*a == &tPenetratingBullet)
				p.get()->isDoublePenetrating = true;
		} else if ((delta->PowerUp > gameVal->powerUpRestore)) {
			a.get()->life = false;
			delta->PowerUp = 0;
		}
	}
	for (auto& a : u->ufoBullets) {
		// Check ufoBullets and player collisons
		if (Collision::PixelPerfectTest(a->sprite, p->sprite) && !p->isIdle) {
			playSound(&destroySound);
			a.get()->life = false;
			p.get()->life = false;
			a.get()->lifes--;
			if (!a->lifes) a.get()->life = false;
			if (!p->lifes) p.get()->life = false;
		}
	}
	for (auto& b : bullets) {
		// Check bullets and UFO collisons
		if (Collision::PixelPerfectTest(b->sprite, u->sprite) && !p->isIdle && u->isActive) {
			playSound(&destroySound);
			u.get()->lifes--;
			b.get()->lifes--;
			if (!b.get()->lifes) b.get()->life = false;
			if (!u.get()->lifes) u.get()->life = false;
		}
	}
}