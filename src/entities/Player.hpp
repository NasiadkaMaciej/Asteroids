#pragma once

#include "../utils/control.hpp"
#include "Entity.hpp"
#include "utils.hpp"
#include <list>

class Player : public Entity {
  public:
	Player();

	void givePoints(int x);
	Scale bulletScale();
	bool canShoot();
	void getControl();
	void shoot(std::list<std::unique_ptr<Entity>>* list);
	void update() override;

	int points = 0;
	int aliveTime = 0;
	bool isRotatingRight = false;
	bool isRotatingLeft = false;
	bool thrust = false;
	bool isShooting = false;
	bool isIdle = true;
	bool isPowerBullet = false;
	bool isDoubleShooting = false;
	bool isDoublePenetrating = false;

  private:
	int bulletFreq = 250;
	float maxSpeed = speedScale * 1.75f;
};