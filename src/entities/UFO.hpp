#pragma once

#include "Entity.hpp"
#include <list>

class UFO : public Entity {
	int bulletFreq = 3000;

  public:
	bool isActive = false;
	std::list<std::unique_ptr<Entity>> ufoBullets;

	UFO();
	bool canShoot();
	void update(float pX, float pY);
	void activate();
	~UFO();
};