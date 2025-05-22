#pragma once

#include "Entity.hpp"
#include "Player.hpp"
#include "utils.hpp"

class PowerUp : public Entity {
  public:
	char type() { return _POWERUP; }

	static void generate(std::list<Entity*>* e, const std::unique_ptr<Player>& p) {
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
