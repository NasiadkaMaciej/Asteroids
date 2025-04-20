/*
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility), Maciej Nasiadka (altered and adjusted for SFML 3.0)
 */
#include "Collision.h"
#include <SFML/Graphics.hpp>
#include <map>

namespace Collision {
class BitmaskManager {
  public:
	~BitmaskManager() {
		std::map<const sf::Texture*, uint8_t*>::const_iterator end = Bitmasks.end();
		for (std::map<const sf::Texture*, uint8_t*>::const_iterator iter = Bitmasks.begin(); iter != end; ++iter)
			delete[] iter->second;
	}

	uint8_t GetPixel(const uint8_t* mask, const sf::Texture* tex, unsigned int x, unsigned int y) {
		if (x > tex->getSize().x || y > tex->getSize().y) return 0;

		return mask[x + y * tex->getSize().x];
	}

	uint8_t* GetMask(const sf::Texture* tex) {
		uint8_t* mask;
		std::map<const sf::Texture*, uint8_t*>::iterator pair = Bitmasks.find(tex);
		if (pair == Bitmasks.end()) {
			sf::Image img = tex->copyToImage();
			mask = CreateMask(tex, img);
		} else
			mask = pair->second;

		return mask;
	}

	uint8_t* CreateMask(const sf::Texture* tex, const sf::Image& img) {
		uint8_t* mask = new uint8_t[tex->getSize().y * tex->getSize().x];

		for (unsigned int y = 0; y < tex->getSize().y; y++)
			for (unsigned int x = 0; x < tex->getSize().x; x++)
				mask[x + y * tex->getSize().x] = img.getPixel({ x, y }).a;

		Bitmasks.insert(std::pair<const sf::Texture*, uint8_t*>(tex, mask));

		return mask;
	}

  private:
	std::map<const sf::Texture*, uint8_t*> Bitmasks;
};

BitmaskManager Bitmasks;

bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, uint8_t AlphaLimit) {
	auto intersectionOpt = Object1.getGlobalBounds().findIntersection(Object2.getGlobalBounds());
	if (intersectionOpt) {
		sf::FloatRect Intersection = *intersectionOpt;
		sf::IntRect O1SubRect = Object1.getTextureRect();
		sf::IntRect O2SubRect = Object2.getTextureRect();

		uint8_t* mask1 = Bitmasks.GetMask(&Object1.getTexture());
		uint8_t* mask2 = Bitmasks.GetMask(&Object2.getTexture());

		// Loop through our pixels
		for (int i = Intersection.position.x; i < Intersection.position.x + Intersection.size.x; i++) {
			for (int j = Intersection.position.y; j < Intersection.position.y + Intersection.size.y; j++) {

				sf::Vector2f o1v = Object1.getInverseTransform().transformPoint({ float(i), float(j) });
				sf::Vector2f o2v = Object2.getInverseTransform().transformPoint({ float(i), float(j) });

				// Make sure pixels fall within the sprite's subrect
				if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 && o1v.x < O1SubRect.size.x &&
					o1v.y < O1SubRect.size.y && o2v.x < O2SubRect.size.x && o2v.y < O2SubRect.size.y) {

					if (Bitmasks.GetPixel(mask1,
										  &Object1.getTexture(),
										  (int)(o1v.x) + O1SubRect.position.x,
										  (int)(o1v.y) + O1SubRect.position.y) > AlphaLimit &&
						Bitmasks.GetPixel(mask2,
										  &Object2.getTexture(),
										  (int)(o2v.x) + O2SubRect.position.x,
										  (int)(o2v.y) + O2SubRect.position.y) > AlphaLimit)
						return true;
				}
			}
		}
	}
	return false;
}
}