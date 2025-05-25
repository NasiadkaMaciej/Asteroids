#include "Player.hpp"
#include "../utils/sounds.hpp"
#include "../utils/textures.hpp"
#include "Bullet.hpp"
#include "utils.hpp"
#include <SFML/System/Angle.hpp>

Player::Player()
  : Entity(window.getView().getCenter().x,
		   window.getView().getCenter().y,
		   0.f, // x-speed
		   0.f, // y-speed
		   sf::degrees(0.f),
		   &tPlayer) {
	lifes = 3;
}

void Player::givePoints(int x) {
	static int tmpPoints = 0;
	points += x;
	tmpPoints += x;
	if (tmpPoints >= 5000) {
		lifes++;
		tmpPoints = 0;
	}
}

Scale Player::bulletScale() {
	if (isPowerBullet)
		return Scale(2, 2);
	else if (isDoublePenetrating)
		return Scale(1, 5);
	else
		return Scale(1, 1);
}

bool Player::canShoot() {
	return isShooting && !isIdle && delta->Shoot > bulletFreq;
}

void Player::getControl() {
	isRotatingRight = CONTROL::isRight();
	isRotatingLeft = CONTROL::isLeft();
	thrust = CONTROL::isThrust();
	isShooting = CONTROL::isSpace();
}

void Player::shoot(std::list<std::unique_ptr<Entity>>* list) {
	if (canShoot()) {
		playSound(&laserSound);
		delta->Shoot = 0;

		// Create bullet with appropriate speed
		list->emplace_back(std::make_unique<Bullet>(x, y, angle, &tBullet, bulletScale()));

		// Handle double shooting if active
		if (isDoubleShooting) {
			// Create second bullet
			list->emplace_back(std::make_unique<Bullet>(x, y, angle, &tBullet, bulletScale()));
		}
	}
}

void Player::update() {
	if (activeState == playState && !isIdle) aliveTime += delta->Move;
	if (!life) {
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

	float rotateSpeed = 18 * delta->Move / 100;
	if (CONTROL::getAxisPos(sf::Joystick::Axis::X) != 0)
		rotateSpeed *= std::abs(CONTROL::getAxisPos(sf::Joystick::Axis::X));
	if (isRotatingRight)
		sprite.setRotation(sprite.getRotation() + sf::degrees(rotateSpeed));
	else if (isRotatingLeft)
		sprite.setRotation(sprite.getRotation() - sf::degrees(rotateSpeed));

	angle = sprite.getRotation() - sf::degrees(90.f);

	sprite.setRotation(angle + sf::degrees(90.f));

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

	if (!life && lifes > 0) {
		isIdle = true;
		if (aliveTime % 100 == 0) {
			life = true;
			isIdle = false;
		}
	}
}