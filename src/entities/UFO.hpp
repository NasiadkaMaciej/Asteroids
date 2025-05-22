#pragma once

#include "Entity.hpp"
#include "utils.hpp"
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

	~UFO() {
		for (auto i = ufoBullets.begin(); i != ufoBullets.end();) {
			Entity* e = *i;
			i = ufoBullets.erase(i);
			delete e;
		}
	}
};
