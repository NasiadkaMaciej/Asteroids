#pragma once

#include "Entity.hpp"
#include <list>

class UFO : public Entity {
	int bulletFreq = 3000;

  public:
	char type() override;
	bool isActive = false;
	std::list<Entity*> ufoBullets;

	UFO();
	bool canShoot();
	void update(float pX, float pY);
	void activate();
	~UFO();
};