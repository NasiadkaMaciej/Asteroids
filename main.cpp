#include "Collision.h"
#include "entities.hpp"
#include "textures.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <math.h>

// game start values;
int bigAsteroids = 6;
int roundNum = 1;

int
main()
{
  // create fullscreen window
  sf::ContextSettings settings;
  // settings.antialiasingLevel = 8.0;
  sf::RenderWindow window(sf::VideoMode(desktopMode.width,
                                        desktopMode.height,
                                        desktopMode.bitsPerPixel),
                          "Asteroids - Macieson",
                          sf::Style::Fullscreen,
                          settings);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  loadTextures();

  sf::Font font;
  sf::Text text;
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

  std::list<Asteroid*> asteroids;
  std::list<Bullet*> bullets;

  for (int i = 0; i < bigAsteroids; i++) {
    asteroids.push_back(generateBigAsteroids(&tAsteroid));
  }

  srand(time(NULL));
  sf::Clock deltaClock;
  sf::Time deltaTime, time;
  float deltaShoot;

  while (window.isOpen()) {
    time += deltaTime;
    deltaTime = deltaClock.restart();
    deltaShoot += deltaTime.asMilliseconds();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
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
      if (deltaShoot >= 250) {
        Bullet* b = new Bullet(p.x, p.y, p.angle, &tBullet);
        bullets.push_back(b);
        deltaShoot = 0;
      }

    // Check bullets and asteroids collisons
    for (auto a : asteroids) {
      for (auto b : bullets) {
        if (Collision::PixelPerfectTest(a->sprite, b->sprite)) {
          a->life = false;
          b->life = false;
          if (a->sprite.getTexture() == &tAsteroid) {
            p.points += 20;
            for (int i = 0; i < 2; i++) {
              Asteroid* e = new Asteroid(a->x,
                                         a->y,
                                         random(6, 3),
                                         random(6, 3),
                                         rand() % 360,
                                         &tAsteroidMedium);
              asteroids.push_back(e);
            }
          } else if (a->sprite.getTexture() == &tAsteroidMedium) {
            p.points += 50;
            for (int i = 0; i < 2; i++) {
              Asteroid* e = new Asteroid(a->x,
                                         a->y,
                                         random(6, 3),
                                         random(6, 3),
                                         rand() % 360,
                                         &tAsteroidSmall);
              asteroids.push_back(e);
            }
          } else if (a->sprite.getTexture() == &tAsteroidSmall)
            p.points += 100;
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
      // Entity* e = *i;
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
        asteroids.push_back(generateBigAsteroids(&tAsteroid));
      }
    }

    int i;
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
    for (const auto& i : asteroids) {
      i->draw(window);
    }
    for (const auto& i : bullets) {
      i->draw(window);
    }
    p.draw(window);
    window.display();
  }
  return 0;
}