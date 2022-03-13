sf::Texture tPlayer, tAsteroid[3], tBullet;


#define big 0
#define medium 1
#define small 2

void loadTextures(){

sf::Texture xd;
tPlayer.loadFromFile("player.png");
tAsteroid[big].loadFromFile("asteroidBig.png");
tAsteroid[medium].loadFromFile("asteroidMedium.png");
tAsteroid[small].loadFromFile("asteroidSmall.png");
tBullet.loadFromFile("bullet.png");
tPlayer.setSmooth(true);
tAsteroid[big].setSmooth(true);
tAsteroid[medium].setSmooth(true);
tAsteroid[small].setSmooth(true);
tBullet.setSmooth(true);
}