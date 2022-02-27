//#include "entities.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <math.h>

sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
float degToRad = M_PI / 180;

// general class for all existing entities
class Entity
{
public:
  float x, y, x_speed, y_speed, angle;
  bool life;
  sf::Sprite sprite;

  Entity() { life = true; }

  virtual void update(){};

  void draw(sf::RenderWindow& window)
  {
    sprite.setPosition(x, y);
    sprite.setRotation(angle + 90);
    window.draw(sprite);
  }
};
class Asteroid : public Entity
{
public:
  Asteroid(float xx, float yy, float xx_speed, float yy_speed, float aangle)
  {
    x = xx;
    y = yy;
    x_speed = xx_speed;
    y_speed = yy_speed;
    angle = aangle;
  }
  void update()
  {
    x += x_speed;
    y += y_speed;
    if (x >= desktopMode.width)
      x = 0;
    else if (x <= 0)
      x = desktopMode.width;
    if (y >= desktopMode.height)
      y = 0;
    else if (y <= 0)
      y = desktopMode.height;
  }
};
class Bullet : public Entity
{
public:
  void update()
  {
    x_speed = cos(angle * degToRad) * 10;
    y_speed = sin(angle * degToRad) * 10;
    x += x_speed;
    y += y_speed;
    if (x > desktopMode.width || x < 0 || y > desktopMode.height || y < 0)
      life = false;
  }
};

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

  srand(time(NULL));

  // load textures
  sf::Texture tPlayer, tAsteroid, tAsteroidSmall, tBullet;
  tPlayer.loadFromFile("player.png");
  tAsteroid.loadFromFile("asteroidBig.png");
  tAsteroidSmall.loadFromFile("asteroidSmall.png");
  tBullet.loadFromFile("bullet.png");
  tPlayer.setSmooth(true);
  tAsteroid.setSmooth(true);
  tAsteroidSmall.setSmooth(true);
  tBullet.setSmooth(true);

  // set player sprite
  sf::Sprite player;
  player.setTexture(tPlayer);
  player.setOrigin(player.getGlobalBounds().width / 2,
                   player.getGlobalBounds().height / 2);
  player.setPosition(window.getView().getCenter());

  std::list<Asteroid*> asteroids;
  std::list<Bullet*> bullets;

  for (int i = 0; i < 15; i++) {
    Asteroid* a = new Asteroid(rand() % desktopMode.width,
                               rand() % desktopMode.height,
                               rand() % 6 - 3,
                               rand() % 6 - 3,
                               rand() % 360);
    a->sprite.setTexture(tAsteroid);
    a->sprite.setOrigin(a->sprite.getGlobalBounds().width / 2,
                        a->sprite.getGlobalBounds().height / 2);
    asteroids.push_back(a);
  }
  // float x,y;
  float x = player.getPosition().x, y = player.getPosition().y;
  float x_speed = 0.0, y_speed = 0.0, angle = 0;
  bool thrust = false;
  bool rotateRight = false, rotateLeft = false;
  float rotateSpeed;

  // sf::Clock deltaClock;
  window.setKeyRepeatEnabled(true);
  while (window.isOpen()) {
    // sf::Time deltaTime = deltaClock.restart();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        rotateRight = true;
      else
        rotateRight = false;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        rotateLeft = true;
      else
        rotateLeft = false;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        thrust = true;
      else
        thrust = false;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        Bullet* b = new Bullet();
        b->sprite.setTexture(tBullet);
        b->x = x;
        b->y = y;
        b->angle = angle;
        bullets.push_back(b);
      }
    }

    // Check bullets and asteroids collisons
    for (auto a : asteroids) {
      for (auto b : bullets) {
        if (a->sprite.getGlobalBounds().intersects(
              b->sprite.getGlobalBounds())) {
          a->life = false;
          b->life = false;
          if (a->sprite.getTexture() == &tAsteroid) {
            Asteroid* e = new Asteroid(
              a->x, a->y, rand() % 6 - 3, rand() % 6 - 3, rand() % 360);
            e->sprite.setTexture(tAsteroidSmall);
            asteroids.push_back(e);
          }
        }
      }
    }

    if (rotateRight)
      player.setRotation(player.getRotation() + 2);
    if (rotateLeft)
      player.setRotation(player.getRotation() - 2);
    angle = player.getRotation() - 90;

    if (thrust) {
      x_speed += cos(angle * degToRad) * 0.2;
      y_speed += sin(angle * degToRad) * 0.2;
    } else {
      x_speed *= 0.985;
      y_speed *= 0.985;
    }

    int maxSpeed = 15;
    float speed = sqrt(x_speed * x_speed + y_speed * y_speed);
    if (speed > maxSpeed) {
      x_speed *= maxSpeed / speed;
      y_speed *= maxSpeed / speed;
    }

    x += x_speed;
    y += y_speed;

    if (x >= desktopMode.width)
      x = 0;
    else if (x <= 0)
      x = desktopMode.width;
    if (y >= desktopMode.height)
      y = 0;
    else if (y <= 0)
      y = desktopMode.height;

    player.setPosition(x, y);

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
    // display debug info
    sf::Font font;
    font.loadFromFile("UbuntuMono-B.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(x) + ":" + std::to_string(y) + "\n" +
                   std::to_string(desktopMode.width) + ":" +
                   std::to_string(desktopMode.height) + "\n" +
                   std::to_string(x_speed) + ":" + std::to_string(y_speed));
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100);

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
    window.draw(player);
    window.draw(text);
    for (const auto& i : asteroids) {
      i->draw(window);
    }
    for (const auto& i : bullets) {
      i->draw(window);
    }
    window.display();
  }
  return 0;
}