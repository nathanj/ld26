#include <SFML/System/Vector2.hpp>

#include "GameSprite.hpp"

GameSprite::GameSprite(const sf::Texture &texture) : sf::Sprite(texture)
{
}

void GameSprite::update(const sf::RenderWindow &, const sf::Time &)
{
}
