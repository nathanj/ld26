#include <SFML/System/Vector2.hpp>
#include <math.h>
#include <string.h>

#include "TextParticle.hpp"

extern sf::Texture font_texture[12];

TextParticle::TextParticle(const char *str)
{
	decay = 150;
	decay_acceleration = 100;
	alpha = 255;
	velocity.x = 20;
	velocity.y = -50;

	position.x = 600;
	position.y = 30;

	renderTexture.create(200, 50);
	sf::Sprite s;
	renderTexture.clear(sf::Color::Transparent);
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			s.setTexture(font_texture[str[i]-'0']);
			s.setPosition(i*32, 0);
			renderTexture.draw(s);
		} else if (str[i] == 'x') {
			s.setTexture(font_texture[11]);
			s.setPosition(i*32, 0);
			renderTexture.draw(s);
		}
	}
	renderTexture.display();
}

void TextParticle::update(const sf::RenderWindow &, const sf::Time &delta)
{
	position.x += velocity.x * delta.asSeconds();
	position.y += velocity.y * delta.asSeconds();
	alpha -= decay * delta.asSeconds();
	decay += decay_acceleration * delta.asSeconds();
	if (alpha < 200) {
		velocity.x = 0;
		velocity.y = 0;
	}
}

bool TextParticle::isDead() const
{
	return alpha <= 0;
}


void TextParticle::createSprite(sf::Sprite &sprite)
{
	sprite.setTexture(renderTexture.getTexture());
	sprite.setPosition(position);
	sprite.setColor(sf::Color(255, 255, 255, alpha));
}
