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
	velocity.y = 20;
	setPosition(200, 200);
}

void Hook::catchFish()
{
	if (caught)
		return;

	sf::FloatRect bounds = getGlobalBounds();
	for (auto it = fishes.begin(); it != fishes.end(); it++) {
		if ((*it)->state == Fish::State::Normal && bounds.intersects((*it)->getGlobalBounds())) {
			printf("caught a fish!\n");
			printf("  1 x = %f  y = %f\n", getPosition().x, getPosition().y);
			printf("  2 x = %f  y = %f\n", (*it)->getPosition().x, (*it)->getPosition().y);
			caught = *it;
			caught->state = Fish::State::Caught;
			velocity.y = -50;
			break;
		}
	}
}

void Hook::update(const sf::RenderWindow &, const sf::Time &delta)
{
	move(velocity * delta.asSeconds());
	if (getPosition().y < 200) {
		velocity.y = 20;
		if (caught) {
			caught->propel();
			caught = NULL;
		}
	}
	if (getPosition().y > 550)
		velocity.y = -20;

	catchFish();
}
