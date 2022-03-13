#include "Collision.h"
#include "entities.hpp"
#include "menu.hpp"
#include <list>

// game start values;
int bigAsteroids = 4; // when generating, 2 more are created
int roundNum = 0;     // when starging, 1 is added
sf::Clock deltaClock;
float deltaShoot;
int
main()
{
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
  loadTextures();

  font.loadFromFile("Hyperspace.otf");
  text.setFont(font);
  text.setCharacterSize(50);
  text.setFillColor(sf::Color::White);
  text.setPosition(60, 60);

  Player p(window.getView().getCenter().x,
           window.getView().getCenter().y,
           0,
           0,
           0,
           &tPlayer);

  Menu menu(desktopMode.width, desktopMode.height);
  std::list<Asteroid*> asteroids;
  std::list<Bullet*> bullets;

  srand(time(NULL));
  sf::Time deltaTime, time;

  while (window.isOpen()) {
    if (isMenu) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          isMenu = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          menu.move(up);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          menu.move(down);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
          menu.click();
        }
        window.clear();
        if (isMenu) {
          menu.draw(window);
          window.display();
        }
      }
    } else {
      time += deltaTime;
      deltaTime = deltaClock.restart();
      deltaShoot += deltaTime.asMilliseconds();
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          isMenu = true;
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
      if (p.isShooting) {
        if (deltaShoot >= p.bulletFreq) {
          Bullet* b = new Bullet(p.x, p.y, p.angle, &tBullet);
          bullets.push_back(b);
          deltaShoot = 0;
        }
      }
      // Check bullets and asteroids collisons
      for (auto a : asteroids) {
        for (auto b : bullets) {
          if (Collision::PixelPerfectTest(a->sprite, b->sprite)) {
            a->life = false;
            b->life = false;
            sf::Texture* tmpTxt;
            if (a->sprite.getTexture() == &tAsteroid[big])
              p.points += 20;
            else if (a->sprite.getTexture() == &tAsteroid[medium])
              p.points += 50;
            else if (a->sprite.getTexture() == &tAsteroid[small])
              p.points += 100;

            for (int i = 0; i < 2; i++)
			 if(generateAsteroids(*a)!=NULL) asteroids.push_back(generateAsteroids(*a));
          }
        }
        if (Collision::PixelPerfectTest(a->sprite, p.sprite)) {
          p.sprite.setPosition(window.getView().getCenter());
          p.x = p.sprite.getPosition().x, p.y = p.sprite.getPosition().y;
          p.x_speed = 0;
          p.y_speed = 0;
          time = sf::seconds(0);
        }
      }
      p.update();

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

      std::string sPoints = std::to_string(p.points);
      std::string sTime = std::to_string(time.asMilliseconds() / 10);
      std::string sLevel = std::to_string(roundNum);
      if (sTime.length() > 2)
        sTime.insert(sTime.length() - 2, ".");
      else
        sTime = "0." + sTime;
      text.setString(sTime + "\n" + sPoints + " points" + "\n" +
                     "Round: " + sLevel);

      if (asteroids.size() == 0) {
        bigAsteroids += 2;
        roundNum++;
        for (int i = 0; i < bigAsteroids; i++) {
          asteroids.push_back(generateBigAsteroids(&tAsteroid[big]));
        }
      }

      for (auto i = asteroids.begin(); i != asteroids.end();) {
        Asteroid* e = *i;
        e->update();
        i++;
      }

      for (auto i = bullets.begin(); i != bullets.end();) {
        Bullet* e = *i;
        e->update();
        i++;
      }
      window.clear();
      window.draw(text);
      for (auto& i : asteroids) {
        i->draw(window);
      }
      for (auto& i : bullets) {
        i->draw(window);
      }
      p.draw(window);
      window.display();
    }
  }
  return 0;
}