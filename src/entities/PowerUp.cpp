#include "PowerUp.hpp"
#include "../utils/GameSettings.hpp"
#include "../utils/textures.hpp"
#include "utils.hpp"

void PowerUp::generate(std::list<std::unique_ptr<Entity>>* e, const std::unique_ptr<Player>& p) {
	if (delta->PowerUp > gameVal->powerUpRestore) {
		int rand = std::rand() % 4;
		e->clear();
		switch (rand) {
		case 0: // Generate bullet resize powerup
			e->emplace_back(std::make_unique<PowerUp>(&tBulletUp));
			break;
		case 1: // Generate life bonus powerup
			e->emplace_back(std::make_unique<PowerUp>(&tLifeUp));
			break;
		case 2: // Generate double shoot powerup
			e->emplace_back(std::make_unique<PowerUp>(&tDoubleBullet));
			break;
		case 3: // Generate double penetrate powerup
			e->emplace_back(std::make_unique<PowerUp>(&tPenetratingBullet));
			break;
		}
		delta->PowerUp = 0;

		// After 10 seconds of last powerUp collection, restore basic gameplay
		p->isPowerBullet = false;
		p->isDoubleShooting = false;
		p->isDoublePenetrating = false;
	}
}

PowerUp::PowerUp(sf::Texture* TEXTURE)
  : Entity(static_cast<float>(std::rand() % gameSettings.resX),
		   static_cast<float>(std::rand() % gameSettings.resY),
		   0,
		   0,
		   sf::degrees(-90.f),
		   TEXTURE) {}