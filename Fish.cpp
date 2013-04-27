#include <SFML/System/Vector2.hpp>
#include <math.h>

#include "Fish.hpp"
#include "Hook.hpp"

extern Hook *hook;

Fish::Fish(const sf::Texture &texture) : GameSprite(texture)
{
	sf::Vector2u size = texture.getSize();
	setOrigin(size.x/2, size.y/2);
	state = Normal;
}

static int chooseDirection(const sf::Vector2f &pos)
{
	// If really close to one of the sides, go the other direction.
	if (pos.x < 30)
		return 1;
	if (pos.x > 680 + (550 - 700) * (pos.y - 200)/400.0)
		return -1;

	// Otherwise randomly choose.
	return (rand() % 2 == 0) ? -1 : 1;
}

void Fish::update(const sf::RenderWindow &, const sf::Time &delta)
{
	/*
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	sf::Vector2f spos = getPosition();

	velocity.x = pos.x - spos.x;
	velocity.y = pos.y - spos.y;

	float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if (length > 0)
		velocity /= length;
		*/

	if (state == Done)
		return;

	if (state == Caught) {
		setScale(1, 1);
		setPosition(hook->getPosition().x, hook->getPosition().y+getTexture()->getSize().y);
		return;
	}

	if (state == Propelled) {
		velocity.y += 130 * delta.asSeconds();
		move(velocity * delta.asSeconds());
		rotate(360 * delta.asSeconds());
		if (velocity.y > 0 && getPosition().y > 120)
			state = Done;
		return;
	}

	if (velocity.x == 0 && velocity.y == 0) {
		// If currently stationary, decide if should move.
		if (rand() % 30 == 0) {
			int dir = chooseDirection(getPosition());
			velocity.x = dir * 30;
			setScale(dir * -1, 1);
		}
	} else {
		// If currently moving, decide if should stop.
		if (rand() % 200 == 0) {
			velocity.x = 0;
		}
	}

	move(velocity * delta.asSeconds());
	if (getPosition().x < 5)
		velocity.x = 0;

	// Stop if running into the grass.
	if (getPosition().x > 700 + (550 - 700) * (getPosition().y - 200)/400.0)
		velocity.x = 0;
}

void Fish::propel()
{
	state = Propelled;
	velocity.x = (650 - getPosition().x) + rand()%100;
	velocity.y = -50-rand()%30;
	rotate(rand() % 360);
}
