#pragma once

#include "Entity.hpp"
#include "utils.hpp"

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