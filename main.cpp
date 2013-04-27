#include <list>
#include <string>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Mouse.hpp>

#include "GameSprite.hpp"
#include "Fish.hpp"
#include "Hook.hpp"

sf::Texture bg_texture;
sf::Sprite bg;
sf::Texture hook_texture;
Hook *hook;

sf::Texture fish_texture[3];
std::vector<Fish*> fishes;

void load_data()
{
	assert(bg_texture.loadFromFile("bg.png"));
	bg.setTexture(bg_texture);
	assert(hook_texture.loadFromFile("hook.png"));
	assert(fish_texture[0].loadFromFile("fish.png"));
	assert(fish_texture[1].loadFromFile("fish2.png"));
	assert(fish_texture[2].loadFromFile("fish3.png"));
}

int main(int, char **)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
	sf::Clock clock;
	sf::Time prev = clock.getElapsedTime();
	int frames = 0;

	srand(time(NULL));

	load_data();

	for (int i = 0; i < 100; i++) {
		Fish *f = new Fish(fish_texture[0]);
		f->setPosition(rand() % 600 + 20, 250 + rand() % 200);
		f->setColor(sf::Color(255-rand()%50, 255-rand()%50, 255-rand()%50, 255));
		fishes.push_back(f);
	}

	for (int i = 0; i < 30; i++) {
		Fish *f = new Fish(fish_texture[1]);
		f->setPosition(rand() % 600 + 20, 300 + rand() % 200);
		f->setColor(sf::Color(255-rand()%50, 255-rand()%50, 255-rand()%50, 255));
		fishes.push_back(f);
	}

	for (int i = 0; i < 5; i++) {
		Fish *f = new Fish(fish_texture[2]);
		f->setPosition(rand() % 500 + 20, 400 + rand() % 200);
		f->setColor(sf::Color(255-rand()%50, 255-rand()%50, 255-rand()%50, 255));
		fishes.push_back(f);
	}

	hook = new Hook(hook_texture);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	while (window.isOpen()) {
		sf::Time time = clock.getElapsedTime();
		sf::Time delta = time - prev;
		prev = time;
		frames++;

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
			(*it)->update(window, delta);
		hook->update(window, delta);

		//window.clear();
		window.draw(bg);
		for (auto it = fishes.begin(); it != fishes.end(); it++)
			window.draw(**it);
		window.draw(*hook);
		window.display();
	}

	sf::Time time = clock.getElapsedTime();
	printf("%d frames in %.2f seconds = %.2f FPS\n", frames, time.asSeconds(),
	       frames / time.asSeconds());

	for (auto it = fishes.begin(); it != fishes.end(); it++)
		delete *it;
	delete hook;

	return 0;
}
