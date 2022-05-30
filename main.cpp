#include "Collision.h"
#include "textures.hpp"
#include "basevalues.hpp"
#include "entities.hpp"
#include "menu.hpp"
#include "sounds.hpp"
#include <list>

int main()
{
	// load all assets TODO: Check if textues are correct
	if (!loadBase() || !loadTextures() || !loadSounds())
		return 0;
	loadScoreBoard();
	// writeScoreBoard();

	// create objects and lists
	Player p(window.getView().getCenter().x, window.getView().getCenter().y, 0, 0, 0, &tPlayer);
	Menu menu(menuEntriesCount, menuEntries);
	GameOver gameOver(gameOverEntriesCount, gameOverEntries);
	Settings settings(settingEntriesCount, settingEntries);
	SaveScore saveScore(saveScoreEntriesCount, saveScoreEntries);
	LeaderBoard leaderBoard(leaderBoardEntriesCount, leaderBoardEntries);
	std::list<Asteroid *> asteroids;
	std::list<Bullet *> bullets;
	std::list<PowerUp *> powerUps;
	ProgressBar progressBar, placeholder;
	placeholder.pg.setFillColor(sf::Color::White);
	placeholder.update();

	// resetting game to base values
	auto reset = [&]()
	{
		asteroids.clear();
		bullets.clear();
		powerUps.clear();
		p.reset();
		bigAsteroids = 4;
		roundNum = 0;
		deltaShoot = 0;
		setBullet(BULLET);
		deltaPowerUp = 0;
		saveScore.wasSaved = false;
		saveScore.isSaving = true;
		menu.reset();
		gameOver.reset();
		settings.reset();
		leaderBoard.reset();
	};

	while (window.isOpen())
	{
		deltaTime = deltaClock.restart();
		if (!isPlaying)
		{
			deltaMenu += deltaTime.asMilliseconds();
			if (isMenu)
			{
				menu.show();
				if (p.lifes <= 0)
					reset();
			}
			else if (isGameOver)
			{
				gameOver.setScore(p.points);
				gameOver.show();
				if (isPlaying) // returned to playing from some menu
					reset();
			}
			else if (isSettings)
				settings.show();
			else if (isSaveScreen)
			{
				saveScore.setScore(p.points);
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
			p.aliveTime += deltaTime;
			deltaShoot += deltaTime.asMilliseconds();
			deltaPowerUp += deltaTime.asMilliseconds();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					setState(menuState);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
					reset();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					p.isRotatingRight = true;
				else
					p.isRotatingRight = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					p.isRotatingLeft = true;
				else
					p.isRotatingLeft = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					p.thrust = true;
				else
					p.thrust = false;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					p.isShooting = true;
				else
					p.isShooting = false;
			}
			if (p.isShooting && !p.isIdle)
				if (deltaShoot >= p.bulletFreq)
				{
					playSound(&laserSound);
					if (isDoubleShooting) // shoot 2 bullets simultaneously
					{
						bullets.push_back(new Bullet(p.x, p.y, p.angle + 2.5, &tBullet));
						bullets.push_back(new Bullet(p.x, p.y, p.angle - 2.5, &tBullet));
					}
					else
						bullets.push_back(new Bullet(p.x, p.y, p.angle, &tBullet));
					deltaShoot = 0;
				}
			for (auto a : asteroids)
			{
				for (auto b : bullets)
					// Check bullets and asteroids collisons
					if (Collision::PixelPerfectTest(a->sprite, b->sprite))
					{
						playSound(&destroySound);
						a->life = false;
						b->life = false;
						progressBar.retractPoint();
					}
				// Check asteroids and player collisions
				if (Collision::PixelPerfectTest(a->sprite, p.sprite) && !p.isIdle)
				{
					playSound(&deathSound);
					a->life = false;
					p.life = false;
					progressBar.retractPoint();
				}
			}
			// Spawn random power up evey 10 seconds and clear old
			if (deltaPowerUp >= 10000)
			{
				int rand = std::rand() % 3;
				powerUps.clear();
				switch (rand)
				{
				case 0: // Generate bullet resize powerup
					powerUps.push_back(new PowerUp(&tBulletUp));
					break;
				case 1: // Generate life bonus powerup
					powerUps.push_back(new PowerUp(&tLifeUp));
					break;
				case 2: // Generate double shoot powerup
					powerUps.push_back(new PowerUp(&tDoubleShoot));
					break;
				}
				deltaPowerUp = 0;
				// After 10 seconds of last powerUp collection, restore basic gameplay
				// (for now, only bullet)
				setBullet(BULLET);
				isDoubleShooting = false;
			}
			// Check power ups and player collisions
			for (auto a : powerUps)
			{
				if (Collision::PixelPerfectTest(a->sprite, p.sprite) && !p.isIdle)
				{
					a->life = false;
					deltaPowerUp = 0;
					if (a->sprite.getTexture() == &tBulletUp)
						setBullet(POWER_BULLET);
					else if (a->sprite.getTexture() == &tLifeUp)
						p.lifes++;
					else if (a->sprite.getTexture() == &tDoubleShoot)
						isDoubleShooting = true;
					// tPlayer.loadFromFile(dir + "playerShielded.png");
				}
				else if ((deltaPowerUp >= 10000) && (a->life == true))
				{
					a->life = false;
					deltaPowerUp = 0;
				}
			}

			// Update all entities and remove dead ones
			for (auto i = asteroids.begin(); i != asteroids.end();)
			{
				Asteroid *e = *i;
				e->update();
				if (e->life == false)
				{
					// Generate smaller asteroids after being hit and give points
					if (e->sprite.getTexture() == &tAsteroid[BIG])
						p.givePoints(20);
					else if (e->sprite.getTexture() == &tAsteroid[MEDIUM])
						p.givePoints(50);
					else if (e->sprite.getTexture() == &tAsteroid[SMALL])
						p.givePoints(100);
					for (int i = 0; i < 2; i++)
						if (generateSmallerAsteroid(*e) != NULL)
							asteroids.push_back(generateSmallerAsteroid(*e));

					i = asteroids.erase(i);
					delete e;
				}
				else
					i++;
			}
			for (auto i = bullets.begin(); i != bullets.end();)
			{
				Bullet *e = *i;
				e->update();
				if (e->life == false)
				{
					i = bullets.erase(i);
					delete e;
				}
				else
					i++;
			}
			for (auto i = powerUps.begin(); i != powerUps.end();)
			{
				PowerUp *e = *i;
				e->update();
				if (e->life == false)
				{
					i = powerUps.erase(i);
					delete e;
				}
				else
					i++;
			}
			p.update();
			// Start new level after clearing all asteroids
			if (asteroids.size() == 0)
			{
				bigAsteroids += 2;
				roundNum++;
				for (int i = 0; i < bigAsteroids; i++)
					asteroids.push_back(generateBigAsteroid(&tAsteroid[BIG]));
				progressBar.reset();
			}

			progressBar.update();

			std::string sPoints = std::to_string(p.points);
			std::string sTime = std::to_string(p.aliveTime.asMilliseconds() / 10);
			std::string sLevel = std::to_string(roundNum);
			std::string sLifes = std::to_string(p.lifes);
			if (sTime.length() > 2)
				sTime.insert(sTime.length() - 2, ".");
			else
				sTime = "0." + sTime;
			text.setString(sTime + " sec" + "\n" + sPoints + " points" + "\n" +
						   "Round: " + sLevel + "\n" + sLifes + " lifes");

			window.clear();
			for (auto &i : asteroids)
				i->draw(window);
			for (auto &i : bullets)
				i->draw(window);
			for (auto &i : powerUps)
				i->draw(window);
			p.draw(window);
			window.draw(text);
			window.draw(placeholder.pg);
			window.draw(progressBar.pg);
			window.display();
		}
	}
	return 0;
}
