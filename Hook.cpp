#include <SFML/System/Vector2.hpp>
#include <math.h>

#include "Hook.hpp"
#include "Fish.hpp"
#include "TextParticle.hpp"

extern std::vector<Fish*> fishes;
extern sf::Texture dude_texture[3];
extern sf::Sprite dude;
extern std::vector<TextParticle*> particles;

extern int score;
extern int multiplier;

Hook::Hook(const sf::Texture &texture) : GameSprite(texture)
{
	sf::Vector2u size = texture.getSize();
	setOrigin(size.x/2, size.y/2);
	velocity.x = 0;
	velocity.y = 30;
	setPosition(600, 20);
	state = Launch;
	dude.setTexture(dude_texture[2]);
	dude.setPosition(570, 43);
	setRotation(0);
	velocity.x = -(rand() % 150 + 100);
	velocity.y = -50;
	caught = NULL;
}

void Hook::catchFish()
{
	if (caught)
		return;

	sf::FloatRect bounds = getGlobalBounds();
	for (auto it = fishes.begin(); it != fishes.end(); it++) {
		if (((*it)->state == Fish::State::Normal || (*it)->state == Fish::State::Selected)
		    && bounds.intersects((*it)->getGlobalBounds())) {
			caught = *it;
			caught->state = Fish::State::Caught;
			caught->setRotation(90);
			velocity.y = -100;
			dude.setTexture(dude_texture[1]);
			dude.setPosition(570, 43);
			break;
		}
	}
}

void Hook::update(const sf::RenderWindow &, const sf::Time &delta)
{
	if (state == Launch) {
		velocity.y += 130 * delta.asSeconds();
		move(velocity * delta.asSeconds());
		if (velocity.y > 0 && getPosition().y > 220) {
			state = Normal;
			dude.setTexture(dude_texture[0]);
			dude.setPosition(570, 53);
			velocity.x = 0;
			velocity.y = 30;
		}
		return;
	}

	if (state == Propelled) {
		move(velocity * delta.asSeconds());
		if (getPosition().x > 650 && getPosition().y < 100) {
			char buf[16];

			state = Launch;
			dude.setTexture(dude_texture[2]);
			dude.setPosition(570, 43);
			setRotation(0);
			velocity.x = -(rand() % 200 + 100);
			velocity.y = -50;

			snprintf(buf, sizeof(buf), "%dx%d", caught->points, multiplier);
			particles.push_back(new TextParticle(buf));
			score += caught->points * multiplier;
			multiplier++;

			caught->propel();
			caught = NULL;
		}
		return;
	}

	move(velocity * delta.asSeconds());
	if (getPosition().y < 200) {
		velocity.y = 20;
		if (caught) {
			caught->rotate(70);
			state = Propelled;
			rotate(70);
			velocity.x = (650 - getPosition().x);
			velocity.y = (50 - getPosition().y);
			float len = sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
			if (len > 0)
				velocity /= len;
			velocity.x *= 500;
			velocity.y *= 500;
		}
	}
	if (getPosition().y > 550)
		velocity.y = -20;

	catchFish();
}
