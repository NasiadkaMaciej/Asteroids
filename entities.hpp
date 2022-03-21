#include "textures.hpp"
#include <math.h>

float asteroidMaxSpeed[3] = { 8, 12, 16 }, asteroidDiffSpeed[3] = { 4, 6, 8 };

// returns random value excluding 0
int
random(int range, int modifier)
{
  int randomValue;
  do
    randomValue = rand() % range - modifier;
  while (randomValue == 0);
  return randomValue;
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
    sprite.scale(0.5, 0.5);
  }

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
  bool thrust = false, isShooting = false, rotateRight = false,
       rotateLeft = false;
  int points = 0, maxSpeed = 15, bulletFreq = 250, lifes = 3, earnedLifes = 1;
  sf::Time aliveTime = sf::seconds(0);

  Player(float X,
         float Y,
         float X_SPEED,
         float Y_SPEED,
         float ANGLE,
         sf::Texture* TEXTURE)
    : Entity(X, Y, X_SPEED, Y_SPEED, ANGLE, TEXTURE){};
  void update()
  {
    if (life) {
      if (rotateRight)
        sprite.setRotation(sprite.getRotation() + 2.5);
      else if (rotateLeft)
        sprite.setRotation(sprite.getRotation() - 2.5);
      angle = sprite.getRotation() - 90;

      if (thrust) {
        x_speed += cos(angle * degToRad) * 0.2;
        y_speed += sin(angle * degToRad) * 0.2;
      } else {
        x_speed *= 0.99;
        y_speed *= 0.99;
      }

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
    } else {
      sprite.setPosition(desktopMode.width / 2, desktopMode.height / 2);
      x = sprite.getPosition().x, y = sprite.getPosition().y;
      x_speed = 0;
      y_speed = 0;
      lifes--;
      aliveTime = sf::seconds(0);
      if (lifes <= 0) {
		  setState(gameoverstate);
      } else {
        life = true;
      }
    }
  }
  void givePoints(int x)
  {
    points += x;
    if (points / 10000 >= earnedLifes) {
      earnedLifes++;
      lifes++;
    }
  }
  void reset()
  {
    x = desktopMode.width / 2, y = desktopMode.height / 2;
    x_speed = 0;
    y_speed = 0;
    angle = 90;
    life = true;
    thrust = false, isShooting = false, rotateRight = false, rotateLeft = false;
    points = 0, maxSpeed = 15, bulletFreq = 250, lifes = 3, earnedLifes = 1;
    aliveTime = aliveTime.Zero;
  }
};
class Asteroid : public Entity
{
public:
  Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture* TEXTURE)
    : Entity(X, Y, X_SPEED, Y_SPEED, rand() % 360, TEXTURE){};
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
  Bullet(float X, float Y, float ANGLE, sf::Texture* TEXTURE)
    : Entity(X,
             Y,
             cos(angle * degToRad) * 20,
             cos(angle * degToRad) * 20,
             ANGLE,
             TEXTURE){};
  void update()
  {
    x_speed = cos(angle * degToRad) * 20;
    y_speed = sin(angle * degToRad) * 20;
    x += x_speed;
    y += y_speed;
    if (x > desktopMode.width || x < 0 || y > desktopMode.height || y < 0)
      life = false;
  }
};

// Generates asteroid at random edge of the screen
Asteroid*
generateBigAsteroids(sf::Texture* texture)
{
  int side = rand() % 4;
  float x, y;
  switch (side) {
    case 0: {
      x = 0;
      y = rand() % desktopMode.height;
    } break;
    case 1: {
      x = desktopMode.width;
      y = rand() % desktopMode.height;
    } break;
    case 2: {
      x = rand() % desktopMode.width;
      y = 0;
    } break;
    case 3: {
      x = rand() % desktopMode.width;
      y = desktopMode.height;
    } break;
  }
  Asteroid* a =
    new Asteroid(x,
                 y,
                 random(asteroidMaxSpeed[big], asteroidDiffSpeed[big]),
                 random(asteroidMaxSpeed[big], asteroidDiffSpeed[big]),
                 texture);
  return a;
}
Asteroid*
generateAsteroids(Asteroid asteroid)
{
  int asteroidNum = 0;
  if (asteroid.sprite.getTexture() == &tAsteroid[big])
    asteroidNum = medium;
  else if (asteroid.sprite.getTexture() == &tAsteroid[medium])
    asteroidNum = small;
  else
    return NULL;
  Asteroid* a = new Asteroid(
    asteroid.x,
    asteroid.y,
    random(asteroidMaxSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
    random(asteroidMaxSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
    &tAsteroid[asteroidNum]);
  return a;
}