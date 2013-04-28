#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>

class TextParticle {
public:
	TextParticle(const char *str);

	sf::Text text;
	sf::Vector2f velocity;
	float alpha;
	float decay;
	float decay_acceleration;

	void update(const sf::RenderWindow &window, const sf::Time &delta);
	bool isDead() const;
private:
	TextParticle(const TextParticle&);
	TextParticle& operator=(const TextParticle&);

};
