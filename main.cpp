#include <list>
#include <string>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include "GameSprite.hpp"

sf::Texture bg_texture;
sf::Sprite bg;

sf::Texture fish_texture;
std::vector<GameSprite*> fishes;

void load_data()
{
	assert(bg_texture.loadFromFile("bg.png"));
	bg.setTexture(bg_texture);
	assert(fish_texture.loadFromFile("fish.png"));
}

int main(int, char **)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

	load_data();

	fishes.push_back(new GameSprite(fish_texture));
	fishes.push_back(new GameSprite(fish_texture));
	fishes.push_back(new GameSprite(fish_texture));
	fishes.push_back(new GameSprite(fish_texture));
	fishes.push_back(new GameSprite(fish_texture));

	for (auto it = fishes.begin(); it != fishes.end(); it++)
		(*it)->setPosition(rand() % 800, rand() % 600);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::JoystickButtonPressed) {
				printf("joystick: %d button: %d\n",
				       event.joystickButton.joystickId,
				       event.joystickButton.button);
			}
			if (event.type == sf::Event::JoystickButtonReleased) {
				printf("joyreles: %d button: %d\n",
				       event.joystickButton.joystickId,
				       event.joystickButton.button);
			}
			if (event.type == sf::Event::JoystickMoved) {
				printf("joystick: %d axis: %d position: %f\n",
				       event.joystickMove.joystickId,
				       event.joystickMove.axis,
				       event.joystickMove.position);
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q)
					window.close();
			}
			if (event.type == sf::Event::MouseMoved) {
				//printf("mouse move: %d %d\n", event.mouseMove.x, event.mouseMove.y);
			}
		}

		for (auto it = fishes.begin(); it != fishes.end(); it++)
			(*it)->update(window);

		//window.clear();
		window.draw(bg);
		for (auto it = fishes.begin(); it != fishes.end(); it++)
			window.draw(**it);
		window.display();
	}

	for (auto it = fishes.begin(); it != fishes.end(); it++)
		delete *it;

	return 0;
}
