//#include "entities.hpp"
#include "Collision.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <math.h>
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
float degToRad = M_PI / 180;
bool thrust = false, isShooting = false;
bool rotateRight = false, rotateLeft = false;
int shootCount;

// returns random value excluding 0
int
random(int range, int modifier)
{
  int x;
  do {
    x = rand() % range - modifier;
  } while (x == 0);
  return x;
}

// general class for all existing entities
class Entity
{
public:
  float x, y, x_speed, y_speed, angle;
  bool life = true;
  sf::Sprite sprite;

  Entity(float X,
         float Y,
         float X_SPEED,
         float Y_SPEED,
         float ANGLE,
         sf::Texture* TEXTURE)
  {
    x = X;
    y = Y;
    x_speed = X_SPEED;
    y_speed = Y_SPEED;
    angle = ANGLE;
    sprite.setTexture(*TEXTURE, true);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2,
                     sprite.getGlobalBounds().height / 2);
  }
  // Entity() { life = true; }

  virtual void update(){};

  void draw(sf::RenderWindow& window)
  {
    sprite.setPosition(x, y);
    sprite.setRotation(angle + 90);
    window.draw(sprite);
  }
};
class Player : public Entity
{
public:
  Player(float X,
         float Y,
         float X_SPEED,
         float Y_SPEED,
         float ANGLE,
         sf::Texture* TEXTURE)
    : Entity(X, Y, X_SPEED, Y_SPEED, ANGLE, TEXTURE){};
  void update()
  {
    if (rotateRight)
      sprite.setRotation(sprite.getRotation() + 2);
    if (rotateLeft)
      sprite.setRotation(sprite.getRotation() - 2);
    angle = sprite.getRotation() - 90;

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
  }
};
class Asteroid : public Entity
{
public:
  Asteroid(float X,
           float Y,
           float X_SPEED,
           float Y_SPEED,
           float ANGLE,
           sf::Texture* TEXTURE)
    : Entity(X, Y, X_SPEED, Y_SPEED, ANGLE, TEXTURE){};
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
  /* Bullet(float X, float Y, float ANGLE, sf::Texture* TEXTURE){
          Entity:Entity(X, Y, cos(angle * degToRad) * 10, sin(angle * degToRad)
  * 10, angle, TEXTURE);
  } */
  Bullet(float X, float Y, float ANGLE, sf::Texture* TEXTURE)
    : Entity(X,
             Y,
             cos(angle * degToRad) * 10,
             cos(angle * degToRad) * 10,
             ANGLE,
             TEXTURE){};
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

  Player p(window.getView().getCenter().x,
           window.getView().getCenter().y,
           0,
           0,
           0,
           &tPlayer);

  std::list<Asteroid*> asteroids;
  std::list<Bullet*> bullets;

  for (int i = 0; i < 15; i++) {
    Asteroid* a = new Asteroid(rand() % desktopMode.width,
                               rand() % desktopMode.height,
                               random(8, 4),
                               random(8, 4),
                               rand() % 360,
                               &tAsteroid);
    asteroids.push_back(a);
  }

  sf::Clock deltaClock;
  sf::Time deltaTime;
  float deltaShoot;
  window.setKeyRepeatEnabled(true);
  while (window.isOpen()) {
    deltaTime = deltaClock.restart();
    deltaShoot += deltaTime.asMilliseconds();
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
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        isShooting = true;
      else
        isShooting = false;
    }
    if (isShooting)
      if (deltaShoot >= 300) {
        Bullet* b = new Bullet(p.x, p.y, p.angle, &tBullet);
        bullets.push_back(b);
        deltaShoot = 0;
      }
    // Check bullets and asteroids collisons
    for (auto a : asteroids)
      for (auto b : bullets)
        if (Collision::PixelPerfectTest(a->sprite, b->sprite)) {
          a->life = false;
          b->life = false;
          if (a->sprite.getTexture() == &tAsteroid)
            for (int i = 0; i < 3; i++) {
              Asteroid* e = new Asteroid(a->x,
                                         a->y,
                                         random(6, 3),
                                         random(6, 3),
                                         rand() % 360,
                                         &tAsteroidSmall);
              e->sprite.setOrigin(e->sprite.getGlobalBounds().width / 2,
                                  e->sprite.getGlobalBounds().height / 2);
              asteroids.push_back(e);
            }
          if (Collision::PixelPerfectTest(a->sprite, p.sprite)) {
            p.sprite.setPosition(window.getView().getCenter());
            p.x = p.sprite.getPosition().x, p.y = p.sprite.getPosition().y;
            p.x_speed = 0;
            p.y_speed = 0;
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
    // display debug info
    /*     sf::Font font;
        font.loadFromFile("UbuntuMono-B.ttf");
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(p.x) + ":" + std::to_string(p.y) + "\n" +
                       std::to_string(desktopMode.width) + ":" +
                       std::to_string(desktopMode.height) + "\n" +
                       std::to_string(p.x_speed) + ":" +
       std::to_string(p.y_speed)); text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);
        text.setPosition(100, 100); */
    if (asteroids.size() <= 15) {
      Asteroid* a = new Asteroid(rand() % desktopMode.width,
                                 rand() % desktopMode.height,
                                 random(8, 4),
                                 random(4, 4),
                                 rand() % 360,
                                 &tAsteroid);
      asteroids.push_back(a);
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
    // window.draw(text);
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