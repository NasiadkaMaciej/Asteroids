#include "Collision.h"
#include "GameSettings.hpp"
#include "textures.hpp"
#include "entities.hpp"
#include "sounds.hpp"
#include "menu.hpp"
#include <list>
#include <thread>

int main()
{
	// load all assets TODO: Check if textues and ScoreBoard is ok
	if (!loadBase() || !loadTextures() || !loadSounds())
		return 0;
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
	background.setTextureRect(sf::IntRect(0, 0, gameSettings.resX, gameSettings.resY));

	placeholder.pg.setFillColor(sf::Color::White);
	placeholder.update();

	// resetting game to base values
	auto reset = [&]()
	{
		asteroids.clear();
		bullets.clear();
		powerUps.clear();
		delete gameVal;
		delete delta;
		gameVal = new GameValues;
		delta = new GameTime;
		delete p;
		delete u;
		p = new Player;
		u = new UFO;
	};

	auto updateList = [&](std::list<Entity *> &list)
	{
		for (auto i = list.begin(); i != list.end();)
		{
			Entity *e = *i;
			e->update();
			if (!e->life)
			{
				if (e->type() == _ASTEROID)
				{
					// Generate smaller asteroids after being hit and give points
					if (*e == &tAsteroid[BIG])
					{
						p->givePoints(20);
						list.emplace_back(Asteroid::generate(e->x, e->y, MEDIUM));
						list.emplace_back(Asteroid::generate(e->x, e->y, MEDIUM));
					}
					else if (*e == &tAsteroid[MEDIUM])
					{
						p->givePoints(50);
						list.emplace_back(Asteroid::generate(e->x, e->y, SMALL));
						list.emplace_back(Asteroid::generate(e->x, e->y, SMALL));
					}
					else if (*e == &tAsteroid[SMALL])
						p->givePoints(100);
				}
				i = list.erase(i);
				delete e;
			}
			else
				i++;
		}
	};
	reset();

	while (window.isOpen())
	{
		delta->Time = delta->Clock.restart();

		if (!isPlaying)
		{
			delta->Menu += delta->Time.asMilliseconds();
			if (isMenu)
			{
				menu.show();
				if (p->lifes <= 0)
					reset();
			}
			else if (isGameOver)
			{
				gameOver.setScore(p->points);
				gameOver.show();
				if (isPlaying) // returned to playing from some menu
					reset();
			}
			else if (isSettings)
				settings.show();
			else if (isSaveScreen)
			{
				saveScore.setScore(p->points);
				saveScore.show();
			}
			else if (isLeaderBoard)
			{
				leaderBoard.setScore();
				leaderBoard.show();
			}
		}
		else
		{
			std::thread worker(checkCollision);
			delta->gameUpdate();
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					setState(menuState);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
					reset();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
					settings.toggleFS();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					p->isRotatingRight = true;
				else
					p->isRotatingRight = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					p->isRotatingLeft = true;
				else
					p->isRotatingLeft = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					p->thrust = true;
				else
					p->thrust = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					p->isShooting = true;
				else
					p->isShooting = false;
			}

			if (p->canShoot())
			{
				playSound(&laserSound);
				if (p->isDoubleShooting) // shoot 2 bullets simultaneously
				{
					bullets.emplace_back(new Bullet(p->x, p->y, p->angle + 2.5, &tBullet, p->bulletScale()));
					bullets.emplace_back(new Bullet(p->x, p->y, p->angle - 2.5, &tBullet, p->bulletScale()));
				}
				else if (p->isDoublePenetrating)
				{
					Bullet *b = new Bullet(p->x, p->y, p->angle, &tBullet, p->bulletScale());
					b->lifes = 2;
					bullets.emplace_back(b);
				}
				else
					bullets.emplace_back(new Bullet(p->x, p->y, p->angle, &tBullet, p->bulletScale()));
				delta->Shoot = 0;
			}

			// Spawn random power up evey 10 seconds and clear old
			if (delta->PowerUp > gameVal->powerUpRestore)
			{
				int rand = std::rand() % 4;
				powerUps.clear();
				switch (rand)
				{
				case 0: // Generate bullet resize powerup
					powerUps.emplace_back(new PowerUp(&tBulletUp));
					break;
				case 1: // Generate life bonus powerup
					powerUps.emplace_back(new PowerUp(&tLifeUp));
					break;
				case 2: // Generate double shoot powerup
					powerUps.emplace_back(new PowerUp(&tDoubleBullet));
					break;
				case 3: // Generate double penetrate powerup
					powerUps.emplace_back(new PowerUp(&tPenetratingBullet));
					break;
				}
				delta->PowerUp = 0;
				// After 10 seconds of last powerUp collection, restore basic gameplay
				// (for now, only bullet)
				p->isPowerBullet = false;
				p->isDoubleShooting = false;
				p->isDoublePenetrating = false;
			}
			if (delta->UFO >= gameVal->UFORestore)
			{
				if (!u->isActive)
					u->activate();
				else
					u->isActive = false;
				delta->UFO = 0;
			}

			worker.join();
			// Update all entities and remove dead ones
			updateList(asteroids);
			updateList(bullets);
			updateList(powerUps);
			updateList(u->ufoBullets);

			p->update();
			u->update(p->x, p->y);

			if (!u->life)
			{
				delta->UFO = 0;
				delete u;
				u = new UFO;
				p->givePoints(1000);
			}

			// Start new level after clearing all asteroids
			if (!asteroids.size())
			{
				gameVal->bigAsteroids += 2;
				gameVal->roundNum++;
				for (int i = 0; i < gameVal->bigAsteroids; i++)
					asteroids.emplace_back(Asteroid::generateBig());
				progressBar.reset();
			}

			progressBar.update();
			std::string sPoints = std::to_string(p->points);
			std::string sTime = std::to_string(p->aliveTime.asMilliseconds() / 10);
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
			text.setString(sTime + " sec" + "\n" + sPoints + " points" + "\n" +
						   "Round: " + sLevel + "\n" + sLifes);
			window.clear();
			if (gameSettings.background)
				window.draw(background);
			for (auto &i : asteroids)
				i->draw(window);
			for (auto &i : bullets)
				i->draw(window);
			for (auto &i : powerUps)
				i->draw(window);
			for (auto &i : u->ufoBullets)
				i->draw(window);
			p->draw(window);
			if (u->isActive)
				u->draw(window);
			window.draw(text);
			window.draw(placeholder.pg);
			window.draw(progressBar.pg);
			window.display();
			delta->Move = 0;
		}
	}
	delete p;
	delete u;
	clearEntities(asteroids);
	clearEntities(bullets);
	clearEntities(powerUps);
	return 0;
}

void checkCollision()
{
	for (auto a : asteroids)
	{
		for (auto b : bullets)
			// Check bullets and asteroids collisons
			if (Collision::PixelPerfectTest(a->sprite, b->sprite) && b->life)
			{
				playSound(&destroySound);
				a->life = false;
				b->lifes--;
				if (!b->lifes)
					b->life = false;
				progressBar.retractPoint();
			}
		// Check asteroids and player collisions
		if (Collision::PixelPerfectTest(a->sprite, p->sprite) && !p->isIdle)
		{
			playSound(&deathSound);
			a->life = false;
			p->life = false;
			progressBar.retractPoint();
		}
	}
	// Check power ups and player collisions
	for (auto a : powerUps)
	{
		if (Collision::PixelPerfectTest(a->sprite, p->sprite) && !p->isIdle)
		{
			a->life = false;
			delta->PowerUp = 0;
			if (*a == &tBulletUp)
				p->isPowerBullet = true;
			else if (*a == &tLifeUp)
				p->lifes++;
			else if (*a == &tDoubleBullet)
				p->isDoubleShooting = true;
			else if (*a == &tPenetratingBullet)
				p->isDoublePenetrating = true;
		}
		else if ((delta->PowerUp > gameVal->powerUpRestore))
		{
			a->life = false;
			delta->PowerUp = 0;
		}
	}
	for (auto a : u->ufoBullets)
	{
		// Check ufoBullets and player collisons
		if (Collision::PixelPerfectTest(a->sprite, p->sprite) && !p->isIdle)
		{
			playSound(&destroySound);
			a->life = false;
			p->life = false;
			a->lifes--;
			if (!a->lifes)
				a->life = false;
			if (!p->lifes)
				p->life = false;
		}
	}
	for (auto b : bullets)
	{
		// Check bullets and UFO collisons
		if (Collision::PixelPerfectTest(b->sprite, u->sprite) && !p->isIdle && u->isActive)
		{
			playSound(&destroySound);
			u->lifes--;
			b->lifes--;
			if (!b->lifes)
				b->life = false;
			if (!u->lifes)
				u->life = false;
		}
	}
}