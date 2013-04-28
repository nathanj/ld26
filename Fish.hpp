#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameSprite.hpp"

class Fish : public GameSprite {
public:
	Fish(const sf::Texture&, int points);

	virtual void update(const sf::RenderWindow &window, const sf::Time &delta);
	void propel();

	enum State {
		Normal,
		Caught,
		Propelled,
		Selected,
		Done
	} state;

	int points;
	sf::Vector2f decay;

private:
	Fish(const Fish&);
	Fish& operator=(const Fish&);
};
