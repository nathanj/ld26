#include <SFML/System/Vector2.hpp>
#include <math.h>

#include "Hook.hpp"
#include "Fish.hpp"

extern std::vector<Fish*> fishes;

Hook::Hook(const sf::Texture &texture) : GameSprite(texture)
{
	sf::Vector2u size = texture.getSize();
	setOrigin(size.x/2, size.y/2);
	velocity.x = 0;
	velocity.y = 30;
	setPosition(200, 200);
	state = Launch;
}

void Hook::catchFish()
{
	if (caught)
		return;

	sf::FloatRect bounds = getGlobalBounds();
	for (auto it = fishes.begin(); it != fishes.end(); it++) {
		if ((*it)->state == Fish::State::Normal && bounds.intersects((*it)->getGlobalBounds())) {
			caught = *it;
			caught->state = Fish::State::Caught;
			caught->setRotation(90);
			velocity.y = -100;
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
			velocity.x = 0;
			velocity.y = 30;
		}
		return;
	}

	if (state == Propelled) {
		move(velocity * delta.asSeconds());
		if (getPosition().x > 650 && getPosition().y < 100) {
			state = Launch;
			setRotation(0);
			velocity.x = -(rand() % 200 + 100);
			velocity.y = -50;

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
			velocity.y = (100 - getPosition().y);
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
