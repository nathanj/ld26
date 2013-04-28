#include <math.h>
#include <string.h>

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "TextParticle.hpp"

extern sf::Texture font_texture[12];
extern sf::Font font;

TextParticle::TextParticle(const char *str)
{
	decay = 150;
	decay_acceleration = 100;
	alpha = 255;
	velocity.x = 20;
	velocity.y = -50;

	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(50);
	text.setStyle(sf::Text::Bold);
	text.setPosition(650, 20);
}

void TextParticle::update(const sf::RenderWindow &, const sf::Time &delta)
{
	sf::Vector2f position = text.getPosition();
	position.x += velocity.x * delta.asSeconds();
	position.y += velocity.y * delta.asSeconds();
	alpha -= decay * delta.asSeconds();
	decay += decay_acceleration * delta.asSeconds();
	if (alpha < 200) {
		velocity.x = 0;
		velocity.y = 0;
	}

	text.setPosition(position);
	text.setColor(sf::Color(0, 32, 0, alpha));
}

bool TextParticle::isDead() const
{
	return alpha <= 0;
}
