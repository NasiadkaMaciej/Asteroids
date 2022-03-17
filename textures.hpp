sf::Texture tPlayer, tAsteroid[3], tBullet;

#define big 0
#define medium 1
#define small 2

void
loadTextures()
{
  std::string dir = "textures/";

  tPlayer.loadFromFile(dir + "player.png");
  tAsteroid[big].loadFromFile(dir + "asteroidBig.png");
  tAsteroid[medium].loadFromFile(dir + "asteroidMedium.png");
  tAsteroid[small].loadFromFile(dir + "asteroidSmall.png");
  tBullet.loadFromFile(dir + "bullet.png");
  tPlayer.setSmooth(true);
  tAsteroid[big].setSmooth(true);
  tAsteroid[medium].setSmooth(true);
  tAsteroid[small].setSmooth(true);
  tBullet.setSmooth(true);
}