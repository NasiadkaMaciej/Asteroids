#include "control.hpp"
#include "sounds.hpp"
#include "textures.hpp"
#include <SFML/Audio.hpp>
#include <SFML/System/Angle.hpp>
#include <list>
#include <memory>

enum eTypes : char { _PLAYER, _ASTEROID, _BULLET, _UFO, _POWERUP };

// Multiplier that ensures equal speed of entities at all resolutions
float speedScale = gameSettings.resY / 200;
float asteroidSpeed[3] = { speedScale, speedScale * 1.5f, speedScale * 2 };
float asteroidDiffSpeed[3] = { asteroidSpeed[0] / 2, asteroidSpeed[1] / 2, asteroidSpeed[2] / 2 };

struct Scale {
	float x, y;
	Scale(float X, float Y) {
		x = X * screenScale;
		y = Y * screenScale;
	}
};

// min + rand() % ( max - min + 1 )
// returns random value excluding 0
int random(int range, int modifier) {
	int randomValue;
	do
		randomValue = std::rand() % range - modifier;
	while (randomValue == 0);
	return randomValue;
}

class Entity // general class for all existing entities
{
  public:
	virtual char type() { return -1; }
	float x, y, x_speed, y_speed;
	sf::Angle angle;
	bool life = true;
	int lifes = 0;
	sf::Sprite sprite;

	Entity(float X, float Y, float X_SPEED, float Y_SPEED, sf::Angle ANGLE, sf::Texture* TEXTURE)
	  : x(X)
	  , y(Y)
	  , x_speed(X_SPEED)
	  , y_speed(Y_SPEED)
	  , angle(ANGLE)
	  , sprite(*TEXTURE) {

		auto bounds = sprite.getGlobalBounds();
		sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
		sprite.scale(sf::Vector2f{ screenScale, screenScale });
	}

	const void draw(sf::RenderWindow& window) {
		sprite.setPosition(sf::Vector2f{ x, y });
		sprite.setRotation(angle + sf::degrees(90.f));
		window.draw(sprite);
	}
	virtual void update() {};

	bool operator==(const sf::Texture* txt) const { return &sprite.getTexture() == txt; }
};

void clearEntities(std::list<Entity*> list) {
	for (auto i = list.begin(); i != list.end();) {
		Entity* e = *i;
		i = list.erase(i);
		delete e;
	}
}

class Bullet : public Entity {
  public:
	char type() { return _BULLET; }
	Bullet(float X, float Y, sf::Angle ANGLE, sf::Texture* TEXTURE, Scale s)
	  : Entity(X, Y, 0, 0, ANGLE, TEXTURE) {
		sprite.scale({ s.x, s.y });
		lifes = 1;
	};
	void update() {
		sf::Angle angle_rad = angle; // convert angle in degrees to sf::Angle

		// Calculate x_speed and y_speed using sf::Angle
		x_speed = (cos(angle_rad.asRadians()) * delta->Move / 5) * speedScale;
		y_speed = (sin(angle_rad.asRadians()) * delta->Move / 5) * speedScale;
		x += x_speed;
		y += y_speed;
		if (x > gameSettings.resX || x < 0 || y > gameSettings.resY || y < 0) {
			life = false;
			lifes--;
		}
	}
};

class Player : public Entity {
	int bulletFreq = 250, maxSpeed = speedScale * 1.75, pts = 0;

  public:
	bool thrust = false, isShooting = false, isIdle = true, isRotatingRight = false, isRotatingLeft = false,
		 isDoubleShooting = false, isPowerBullet = false, isDoublePenetrating = false;
	const int& points = pts; // readonly variable, to give points, use givePoints(int)

	unsigned long long aliveTime = 0;
	char type() { return _PLAYER; }
	Player()

	  : Entity(window.getView().getCenter().x,
			   window.getView().getCenter().y,
			   0.f, // x‐speed
			   0.f, // y‐speed
			   sf::degrees(0.f),
			   &tPlayer) {
		lifes = 3;
	}
	void update() {
		aliveTime += delta->Move;
		if (life) {
			float rotateSpeed = 18 * delta->Move / 100;
			if (CONTROL::getAxisPos(sf::Joystick::Axis::X) != 0)
				rotateSpeed *= std::abs(CONTROL::getAxisPos(sf::Joystick::Axis::X));
			if (isRotatingRight)
				sprite.setRotation(sprite.getRotation() + sf::degrees(rotateSpeed));
			else if (isRotatingLeft)
				sprite.setRotation(sprite.getRotation() - sf::degrees(rotateSpeed));

			angle = sprite.getRotation() - sf::degrees(90.f);
			if (thrust) {
				float axPos = (axPos == !0.f) ? std::abs(CONTROL::getAxisPos(sf::Joystick::Axis::R)) : 1;
				x_speed += std::cos(angle.asRadians()) * delta->Move / 100.f * axPos;
				y_speed += std::sin(angle.asRadians()) * delta->Move / 100.f * axPos;
				isIdle = false;
			} else {
				x_speed *= (1 - delta->Move / 1000);
				y_speed *= (1 - delta->Move / 1000);
			}

			float speed = sqrt(x_speed * x_speed + y_speed * y_speed);
			if (speed > maxSpeed) {
				x_speed *= maxSpeed / speed;
				y_speed *= maxSpeed / speed;
			}

			x += x_speed * delta->Move / 10;
			y += y_speed * delta->Move / 10;

			if (x > gameSettings.resX)
				x = 0;
			else if (x < 0)
				x = gameSettings.resX;
			if (y > gameSettings.resY)
				y = 0;
			else if (y < 0)
				y = gameSettings.resY;
		} else { // reset player after death
			sprite.setPosition(sf::Vector2f(gameSettings.resX / 2, gameSettings.resY / 2));
			x = sprite.getPosition().x, y = sprite.getPosition().y;
			x_speed = 0;
			y_speed = 0;
			lifes--;
			aliveTime = 0;

			if (lifes <= 0)
				setState(gameoverState);
			else
				life = true;
			isIdle = true;
		}
	}
	void givePoints(int x) {
		static int tmpPoints = 0;
		pts += x;
		tmpPoints += x;
		if (tmpPoints >= 5000) {
			lifes++;
			tmpPoints = 0;
		}
	}
	Scale bulletScale() {
		if (isPowerBullet)
			return Scale(2, 2);
		else if (isDoublePenetrating)
			return Scale(1, 5);
		else
			return Scale(1, 1);
	}
	bool canShoot() { return isShooting && !isIdle && delta->Shoot > bulletFreq; }
	void getControl() {
		isRotatingRight = CONTROL::isRight();
		isRotatingLeft = CONTROL::isLeft();
		thrust = CONTROL::isThrust();
		isShooting = CONTROL::isSpace();
	}
	void shoot(std::__cxx11::list<Entity*>* list) {
		if (canShoot()) {
			playSound(&laserSound);
			if (isDoubleShooting) // shoot 2 bullets simultaneously
			{
				list->emplace_back(new Bullet(x, y, angle + sf::degrees(2.5f), &tBullet, bulletScale()));
				list->emplace_back(new Bullet(x, y, angle - sf::degrees(2.5f), &tBullet, bulletScale()));
			} else if (isDoublePenetrating) {
				Bullet* b = new Bullet(x, y, angle, &tBullet, bulletScale());
				b->lifes = 2;
				list->emplace_back(b);
			} else
				list->emplace_back(new Bullet(x, y, angle, &tBullet, bulletScale()));
			delta->Shoot = 0;
		}
	}
};
class Asteroid : public Entity {
	float rotation;

  public:
	char type() { return _ASTEROID; }
	Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture* TEXTURE)
	  : Entity(X, Y, X_SPEED, Y_SPEED, sf::degrees(static_cast<float>(std::rand() % 360)), TEXTURE) {};

	void update() {
		// Slowly rotate asteroids
		sprite.setRotation(sprite.getRotation() + sf::degrees(rotation * delta->Move / 10000.f));
		angle = sprite.getRotation() - sf::degrees(90.f);

		x += x_speed * delta->Move / 10;
		y += y_speed * delta->Move / 10;
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
	static Asteroid* generateBig() {
		int side = std::rand() % 4;
		float x, y;
		switch (side) {
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
		return new Asteroid(x,
							y,
							random(asteroidSpeed[BIG], asteroidDiffSpeed[BIG]),
							random(asteroidSpeed[BIG], asteroidDiffSpeed[BIG]),
							&tAsteroid[BIG]);
	}
	static Asteroid* generate(float X, float Y, eSizes asteroidNum) {
		return new Asteroid(X,
							Y,
							random(asteroidSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
							random(asteroidSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
							&tAsteroid[asteroidNum]);
	}
};

class UFO : public Entity {
	int bulletFreq = 3000;

  public:
	char type() { return _UFO; }
	bool isActive = false;
	std::list<Entity*> ufoBullets;

	UFO()
	  : Entity(static_cast<float>(std::rand() % gameSettings.resX),
			   static_cast<float>(std::rand() % gameSettings.resY),
			   0,
			   0,
			   sf::degrees(-90.f),
			   &tUFO) {
		lifes = 5;
		life = true;
		isActive = false;
	}

	bool canShoot() { return delta->ufoShoot >= bulletFreq; }
	void update(float pX, float pY) {
		if (isActive) {

			x_speed = (pX - x) / 100;
			y_speed = (pY - y) / 100;

			x += x_speed * delta->Move / 100 * speedScale;
			y += y_speed * delta->Move / 100 * speedScale;

			if (canShoot()) {
				playSound(&ufoLaserSound);
				sf::Angle angle = sf::radians(atan2(pY - y, pX - x));
				Scale s(2, 4);
				ufoBullets.emplace_back(new Bullet(x, y, angle, &tUFOBullet, s));
				delta->ufoShoot = 0;
			}
		} else {
			x = 10000;
			y = 10000;
		}
	}
	void activate() {
		if (delta->UFO >= gameVal->UFORestore)
			if (isActive)
				isActive = false;
			else {
				x = rand() % gameSettings.resX;
				y = rand() % gameSettings.resY;
				isActive = true;
				delta->ufoShoot = 0;
			}
		delta->UFO = 0;
	}

	~UFO() { clearEntities(ufoBullets); }
};

class PowerUp : public Entity {
  public:
	char type() { return _POWERUP; }

	static void generate(std::__cxx11::list<Entity*>* e, Player* p) {
		if (delta->PowerUp > gameVal->powerUpRestore) {
			int rand = std::rand() % 4;
			e->clear();
			switch (rand) {
			case 0: // Generate bullet resize powerup
				e->emplace_back(new PowerUp(&tBulletUp));
				break;
			case 1: // Generate life bonus powerup
				e->emplace_back(new PowerUp(&tLifeUp));
				break;
			case 2: // Generate double shoot powerup
				e->emplace_back(new PowerUp(&tDoubleBullet));
				break;
			case 3: // Generate double penetrate powerup
				e->emplace_back(new PowerUp(&tPenetratingBullet));
				break;
			}
			delta->PowerUp = 0;
			// After 10 seconds of last powerUp collection, restore basic gameplay
			// (for now, only bullet)
			p->isPowerBullet = false;
			p->isDoubleShooting = false;
			p->isDoublePenetrating = false;
		}
	}
	PowerUp(sf::Texture* TEXTURE)
	  : Entity(static_cast<float>(std::rand() % gameSettings.resX),
			   static_cast<float>(std::rand() % gameSettings.resY),
			   0,
			   0,
			   sf::degrees(-90.f),
			   TEXTURE) {};
};

Player* p;
UFO* u;

// std::unique_ptr<Player> p = std::make_unique<Player>();
// std::unique_ptr<UFO> u = std::make_unique<UFO>();

std::list<Entity*> asteroids;
std::list<Entity*> bullets;
std::list<Entity*> powerUps;