#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class TextParticle {
public:
	TextParticle(const char *str);

	sf::RenderTexture renderTexture;
	sf::Vector2f velocity;
	sf::Vector2f position;
	float alpha;
	float decay;
	float decay_acceleration;

	void update(const sf::RenderWindow &window, const sf::Time &delta);
	bool isDead() const;
	void createSprite(sf::Sprite &sprite);
private:
	TextParticle(const TextParticle&);
	TextParticle& operator=(const TextParticle&);

};
