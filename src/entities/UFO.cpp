#include "UFO.hpp"
#include "../utils/sounds.hpp"
#include "../utils/textures.hpp"
#include "Bullet.hpp"
#include "utils.hpp"

UFO::UFO()
  : Entity(static_cast<float>(std::rand() % gameSettings.resX),
		   static_cast<float>(std::rand() % gameSettings.resY),
		   0,
		   0,
		   sf::degrees(-90.f),
		   &tUFO) {
	lifes = 3;
}

bool UFO::canShoot() {
	return delta->ufoShoot > bulletFreq;
}

void UFO::update(float pX, float pY) {
	if (isActive) {
		x_speed = (pX - x) / 1000;
		y_speed = (pY - y) / 1000;

		x += x_speed * delta->Move / 10 * speedScale;
		y += y_speed * delta->Move / 10 * speedScale;

		if (canShoot()) {
			playSound(&ufoLaserSound);
			sf::Angle angle = sf::radians(atan2(pY - y, pX - x));
			Scale s(2, 4);
			ufoBullets.emplace_back(std::make_unique<Bullet>(x, y, angle, &tUFOBullet, s));
			delta->ufoShoot = 0;
		}
	} else {
		x = 10000;
		y = 10000;
	}
}

void UFO::activate() {
	if (delta->UFO >= gameVal->UFORestore) {
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
}

UFO::~UFO() {
	ufoBullets.clear();
}