float asteroidMaxSpeed[3] = {8, 12, 16}, asteroidDiffSpeed[3] = {4, 6, 8};

struct Scale
{
	float x;
	float y;
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
	float x, y, x_speed, y_speed, angle;
	bool life = true;
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

	void update(){};

	void draw(sf::RenderWindow &window)
	{
		sprite.setPosition(x, y);
		sprite.setRotation(angle + 90);
		window.draw(sprite);
	}
};
class Player : public Entity
{
	int bulletFreq = 250, maxSpeed = 15;

public:
	bool thrust = false, isShooting = false, isIdle = true,
		 isRotatingRight = false, isRotatingLeft = false,
		 isDoubleShooting = false, isPowerBullet = false, isDoublePenetrating = false;
	int points = 0, lifes = 3, earnedLifes = 1;
	float deltaShoot;

	sf::Time aliveTime = sf::Time::Zero;

	Player()
		: Entity(window.getView().getCenter().x, window.getView().getCenter().y, 0, 0, 0, &tPlayer){};
	void update()
	{
		aliveTime += delta->Time;
		deltaShoot += delta->Time.asMilliseconds();
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
				x_speed *= (float)(1 - delta->Move / 1200);
				y_speed *= (float)(1 - delta->Move / 1200);
			}

			float speed = sqrt(x_speed * x_speed + y_speed * y_speed);
			if (speed > maxSpeed)
			{
				x_speed *= maxSpeed / speed;
				y_speed *= maxSpeed / speed;
			}

			x += x_speed * delta->Move / 15;
			y += y_speed * delta->Move / 15;

			if (x >= desktopMode.width)
				x = 0;
			else if (x <= 0)
				x = desktopMode.width;
			if (y >= desktopMode.height)
				y = 0;
			else if (y <= 0)
				y = desktopMode.height;
		}
		else
		{ // reset player after death
			sprite.setPosition(desktopMode.width / 2, desktopMode.height / 2);
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
		if (isShooting && !isIdle && deltaShoot >= bulletFreq)
			return true;
		else
			return false;
	}
};
class Asteroid : public Entity
{
public:
	Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture *TEXTURE)
		: Entity(X, Y, X_SPEED, Y_SPEED, rand() % 360, TEXTURE){};
	void update()
	{
		x += x_speed * delta->Move / 15;
		y += y_speed * delta->Move / 15;
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
	int lifes = 1;
	Bullet(float X, float Y, float ANGLE, sf::Texture *TEXTURE, Scale s)
		: Entity(X, Y, cos(ANGLE * degToRad) * 20, cos(ANGLE * degToRad) * 20, ANGLE, TEXTURE)
	{
		sprite.scale(s.x, s.y);
	};
	void update()
	{
		x_speed = cos(angle * degToRad) * 20 * delta->Move / 15;
		y_speed = sin(angle * degToRad) * 20 * delta->Move / 15;
		x += x_speed;
		y += y_speed;
		if (x > desktopMode.width || x < 0 || y > desktopMode.height || y < 0)
		{
			life = false;
			lifes--;
		}
	}
};

class PowerUp : public Entity
{
public:
	PowerUp(sf::Texture *TEXTURE)
		: Entity(rand() % desktopMode.width, rand() % desktopMode.height,
				 0, 0, -90, TEXTURE){};
};

// Generates asteroid at random edge of the screen
Asteroid *
generateBigAsteroid(sf::Texture *texture)
{
	int side = std::rand() % 4;
	float x, y;
	switch (side)
	{
	case 0:
		x = 0;
		y = rand() % desktopMode.height;
		break;
	case 1:
		x = desktopMode.width;
		y = rand() % desktopMode.height;
		break;
	case 2:
		x = rand() % desktopMode.width;
		y = 0;
		break;
	case 3:
		x = rand() % desktopMode.width;
		y = desktopMode.height;
		break;
	}
	Asteroid *a =
		new Asteroid(x, y,
					 random(asteroidMaxSpeed[BIG], asteroidDiffSpeed[BIG]),
					 random(asteroidMaxSpeed[BIG], asteroidDiffSpeed[BIG]),
					 texture);
	return a;
}
Asteroid *
generateAsteroid(Asteroid asteroid)
{
	int asteroidNum = 0;
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