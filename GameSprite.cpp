#include <SFML/System/Vector2.hpp>
#include <math.h>

#include "GameSprite.hpp"

GameSprite::GameSprite(const sf::Texture &texture) : sf::Sprite(texture)
{
}

void GameSprite::update(const sf::RenderWindow &window)
{
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	sf::Vector2f spos = getPosition();

	velocity.x = pos.x - spos.x;
	velocity.y = pos.y - spos.y;

	float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if (length > 0)
		velocity /= length;

	move(velocity);
}
