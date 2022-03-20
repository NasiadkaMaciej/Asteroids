#include "Collision.h"
#include "basevalues.hpp"
#include "entities.hpp"
#include "menu.hpp"
#include <list>

int
main()
{
  loadBase();
  loadTextures();

  Player p(window.getView().getCenter().x,
           window.getView().getCenter().y,
           0,
           0,
           0,
           &tPlayer);
  Menu menu(menuEntriesCount, menuEntries);
  GameOver gameOver(gameOverEntriesCount, gameOverEntries);
  std::list<Asteroid*> asteroids;
  std::list<Bullet*> bullets;
  srand(time(NULL));

  auto reset = [&]() {
    asteroids.clear();
    bullets.clear();
    p.reset();
    bigAsteroids = 4;
    roundNum = 0;
  };

  while (window.isOpen()) {
    if (!isPlaying) {
      if (p.lifes > 0)
        menu.show();
      else if (p.lifes <= 0) {
        // I don't like that solution, check if you can find any better
        gameOver.active = true;
        gameOver.entries[0] = "Your score " + std::to_string(p.points);
        gameOver.move(0);
        gameOver.show();

        if (gameOver.active == false) {
          reset();
          resume();
        } else if (isPlaying) {
          resume();
          reset();
        }
      }
    } else {
      p.aliveTime += deltaTime;
      deltaTime = deltaClock.restart();
      deltaShoot += deltaTime.asMilliseconds();
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          pause();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
          reset();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
          p.rotateRight = true;
        else
          p.rotateRight = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
          p.rotateLeft = true;
        else
          p.rotateLeft = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          p.thrust = true;
        else
          p.thrust = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
          p.isShooting = true;
        else
          p.isShooting = false;
      }
      if (p.isShooting)
        if (deltaShoot >= p.bulletFreq) {
          bullets.push_back(new Bullet(p.x, p.y, p.angle, &tBullet));
          deltaShoot = 0;
        }
      // Check bullets and asteroids collisons
      for (auto a : asteroids) {
        for (auto b : bullets)
          if (Collision::PixelPerfectTest(a->sprite, b->sprite)) {
            a->life = false;
            b->life = false;
            if (a->sprite.getTexture() == &tAsteroid[big])
              p.givePoints(20);
            else if (a->sprite.getTexture() == &tAsteroid[medium])
              p.givePoints(50);
            else if (a->sprite.getTexture() == &tAsteroid[small])
              p.givePoints(100);
            for (int i = 0; i < 2; i++)
              if (generateAsteroids(*a) != NULL)
                asteroids.push_back(generateAsteroids(*a));
          }
        if (Collision::PixelPerfectTest(a->sprite, p.sprite)) {
          a->life = false;
          p.life = false;
          for (int i = 0; i < 2; i++)
            if (generateAsteroids(*a) != NULL)
              asteroids.push_back(generateAsteroids(*a));
        }
      }

      for (auto i = asteroids.begin(); i != asteroids.end();) {
        Asteroid* e = *i;
        e->update();
        if (e->life == false) {
          i = asteroids.erase(i);
          delete e;
        } else
          i++;
      }
      for (auto i = bullets.begin(); i != bullets.end();) {
        Bullet* e = *i;
        e->update();
        if (e->life == false) {
          i = bullets.erase(i);
          delete e;
        } else
          i++;
      }

      p.update();

      if (asteroids.size() == 0) {
        bigAsteroids += 2;
        roundNum++;
        for (int i = 0; i < bigAsteroids; i++)
          asteroids.push_back(generateBigAsteroids(&tAsteroid[big]));
      }

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
      window.draw(text);
      for (auto& i : asteroids)
        i->draw(window);
      for (auto& i : bullets)
        i->draw(window);
      p.draw(window);
      window.display();
    }
  }
  return 0;
}