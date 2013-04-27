#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class GameSprite : public sf::Sprite {
public:
	GameSprite(const sf::Texture&);
	sf::Vector2f velocity;

	virtual void update(const sf::RenderWindow &window, const sf::Time &delta);
private:
	GameSprite(const GameSprite&);
	GameSprite& operator=(const GameSprite&);
};
