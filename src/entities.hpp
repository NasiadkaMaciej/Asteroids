#include <list>
#include "sounds.hpp"

enum eTypes : char
{
	_PLAYER,
	_ASTEROID,
	_BULLET,
	_UFO,
	_POWERUP
};

float speedScale = gameSettings.resY / 200;
float asteroidMaxSpeed[3] = {speedScale, speedScale * 1.5f, speedScale * 2};
float asteroidDiffSpeed[3] = {speedScale / 2, speedScale / 2 * 1.5f, speedScale};

struct Scale
{
	float x, y;
	Scale(float X, float Y)
	{
		x = X;
		y = Y;
	}
};

// returns random value excluding 0
int random(int range, int modifier)
{
	int randomValue;
	do
		randomValue = std::rand() % range - modifier;
	while (randomValue == 0);
	return randomValue;
}

class Entity // general class for all existing entities
{
public:
	virtual char type()
	{
		return -1;
	}
	float x, y, x_speed, y_speed, angle;
	bool life = true;
	int lifes = 0;
	sf::Sprite sprite;

	Entity(float X, float Y, float X_SPEED, float Y_SPEED, float ANGLE, sf::Texture *TEXTURE)
	{
		x = X;
		y = Y;
		x_speed = X_SPEED;
		y_speed = Y_SPEED;
		angle = ANGLE;
		sprite.setTexture(*TEXTURE, true);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
		sprite.scale(scale, scale);
	}

	void draw(sf::RenderWindow &window)
	{
		sprite.setPosition(x, y);
		sprite.setRotation(angle + 90);
		window.draw(sprite);
	}
	virtual void update(){};
};
class Player : public Entity
{
	int bulletFreq = 250, maxSpeed = speedScale * 2;
public:
	bool thrust = false, isShooting = false, isIdle = true,
		 isRotatingRight = false, isRotatingLeft = false,
		 isDoubleShooting = false, isPowerBullet = false, isDoublePenetrating = false;
	int points = 0, earnedLifes = 1;

	sf::Time aliveTime = sf::Time::Zero;
	char type()
	{
		return _PLAYER;
	}
	Player()
		: Entity(window.getView().getCenter().x, window.getView().getCenter().y, 0, 0, 0, &tPlayer){
			lifes = 3;
		};
	void update()
	{
		aliveTime += delta->Time;
		if (life)
		{
			float rotateSpeed = 2.5 * delta->Move / 15;
			if (isRotatingRight)
				sprite.setRotation(sprite.getRotation() + rotateSpeed);
			else if (isRotatingLeft)
				sprite.setRotation(sprite.getRotation() - rotateSpeed);
			angle = sprite.getRotation() - 90;

			if (thrust)
			{
				x_speed += cos(angle * degToRad) * 0.2 * delta->Move / 15;
				y_speed += sin(angle * degToRad) * 0.2 * delta->Move / 15;
				isIdle = false;
			}
			else
			{
				x_speed *= (1 - delta->Move / 1200);
				y_speed *= (1 - delta->Move / 1200);
			}

			float speed = sqrt(x_speed * x_speed + y_speed * y_speed);
			if (speed > maxSpeed)
			{
				x_speed *= maxSpeed / speed;
				y_speed *= maxSpeed / speed;
			}

			x += x_speed * delta->Move / 15;
			y += y_speed * delta->Move / 15;

			if (x > gameSettings.resX)
				x = 0;
			else if (x < 0)
				x = gameSettings.resX;
			if (y > gameSettings.resY)
				y = 0;
			else if (y < 0)
				y = gameSettings.resY;
		}
		else
		{ // reset player after death
			sprite.setPosition(gameSettings.resX / 2, gameSettings.resY / 2);
			x = sprite.getPosition().x, y = sprite.getPosition().y;
			x_speed = 0;
			y_speed = 0;
			lifes--;
			aliveTime = sf::Time::Zero;

			if (lifes <= 0)
				setState(gameoverState);
			else
				life = true;
			isIdle = true;
		}
	}
	void givePoints(int x)
	{
		points += x;
		if (points / 10000 >= earnedLifes)
		{
			earnedLifes++;
			lifes++;
		}
	}
	Scale bulletScale()
	{
		if (isPowerBullet)
			return Scale(2, 2);
		else if (isDoublePenetrating)
			return Scale(1, 5);
		else
			return Scale(1, 1);
	}
	bool canShoot()
	{
		if (isShooting && !isIdle && delta->Shoot > bulletFreq)
			return true;
		else
			return false;
	}
};
class Asteroid : public Entity
{
public:
	char type()
	{
		return _ASTEROID;
	}
	Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture *TEXTURE)
		: Entity(X, Y, X_SPEED, Y_SPEED, rand() % 360, TEXTURE){};
	void update()
	{
		x += x_speed * delta->Move / 15;
		y += y_speed * delta->Move / 15;
		if (x > gameSettings.resX)
			x = 0;
		else if (x < 0)
			x = gameSettings.resX;
		if (y > gameSettings.resY)
			y = 0;
		else if (y < 0)
			y = gameSettings.resY;
	}
	// Generates asteroid at random edge of the screen
	static Asteroid *generateBig()
	{
		int side = std::rand() % 4;
		float x, y;
		switch (side)
		{
		case 0:
			x = 0;
			y = rand() % gameSettings.resY;
			break;
		case 1:
			x = gameSettings.resX;
			y = rand() % gameSettings.resY;
			break;
		case 2:
			x = rand() % gameSettings.resX;
			y = 0;
			break;
		case 3:
			x = rand() % gameSettings.resX;
			y = gameSettings.resY;
			break;
		}
		Asteroid *a =
			new Asteroid(x, y,
						 random(asteroidMaxSpeed[BIG], asteroidDiffSpeed[BIG]),
						 random(asteroidMaxSpeed[BIG], asteroidDiffSpeed[BIG]),
						 &tAsteroid[BIG]);
		return a;
	}
	static Asteroid *generate(Entity asteroid)
	{
		eSizes asteroidNum;
		if (asteroid.sprite.getTexture() == &tAsteroid[BIG])
			asteroidNum = MEDIUM;
		else if (asteroid.sprite.getTexture() == &tAsteroid[MEDIUM])
			asteroidNum = SMALL;
		else
			return NULL;
		Asteroid *a = new Asteroid(
			asteroid.x,
			asteroid.y,
			random(asteroidMaxSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
			random(asteroidMaxSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
			&tAsteroid[asteroidNum]);
		return a;
	}
};
class Bullet : public Entity
{
public:
	char type()
	{
		return _BULLET;
	}
	Bullet(float X, float Y, float ANGLE, sf::Texture *TEXTURE, Scale s)
		: Entity(X, Y, 0, 0, ANGLE, TEXTURE)
	{
		sprite.scale(s.x, s.y);
		lifes = 1;
		x_speed = (cos(angle * degToRad) * delta->Move) * speedScale / 5;
		y_speed = (sin(angle * degToRad) * delta->Move) * speedScale / 5;
	};
	void update()
	{
		x += x_speed;
		y += y_speed;
		if (x > gameSettings.resX || x < 0 || y > gameSettings.resY || y < 0)
		{
			life = false;
			lifes--;
		}
	}
};

class UFO : public Entity
{
	int bulletFreq = 3000;

public:
	char type()
	{
		return _UFO;
	}
	bool isActive = false;
	std::list<Entity *> ufoBullets;

	UFO()
		: Entity(rand() % gameSettings.resX, rand() % gameSettings.resY, 0, 0, -90, &tUFO)
	{
		lifes = 5;
		life = true;
		isActive = false;
	};

	bool canShoot()
	{
		if (delta->ufoShoot >= bulletFreq)
			return true;
		else
			return false;
	}
	void update(float pX, float pY)
	{
		if (isActive)
		{

			x_speed = (pX - x);
			y_speed = (pY - y);

			x += x_speed * delta->Move / (1000 * speedScale);
			y += y_speed * delta->Move / (1000 * speedScale);
	

			if (canShoot())
			{
				playSound(&ufoLaserSound);
				float angle = atan2(pY - y, pX - x) * (180 / M_PI);
				Scale s(1, 1);
				ufoBullets.push_back(new Bullet(x, y, angle, &tUFOBullet, s));
				delta->ufoShoot = 0;
			}
		}
		else
		{
			x = 10000;
			y = 10000;
		}
	}
	void activate()
	{
		x = rand() % gameSettings.resX;
		y = rand() % gameSettings.resY;
		isActive = true;
		delta->ufoShoot = 0;
	}
};

class PowerUp : public Entity
{
public:
	char type()
	{
		return _POWERUP;
	}
	PowerUp(sf::Texture *TEXTURE)
		: Entity(rand() % gameSettings.resX, rand() % gameSettings.resY, 0, 0, -90, TEXTURE){};
};