#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameSprite.hpp"

class Fish;

class Hook : public GameSprite {
public:
	Hook(const sf::Texture&);

	virtual void update(const sf::RenderWindow &window, const sf::Time &delta);
private:
	Hook(const Hook&);
	Hook& operator=(const Hook&);

	void catchFish();

	Fish *caught; // The currently caught fish.
};
