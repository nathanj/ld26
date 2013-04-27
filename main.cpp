#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>

using namespace std;
using namespace sf;

void move(Sprite &sprite, float dx, float dy)
{
	Vector2f v = sprite.getPosition();
	v.x += dx;
	v.y += dy;
	sprite.setPosition(v);
}

int main(int, char **)
{
	RenderWindow window(VideoMode(800, 600), "SFML Window");

	Texture bg_texture;
	if (!bg_texture.loadFromFile("bg.png"))
		return 1;
	Sprite bg(bg_texture);

	Texture fish_texture;
	if (!fish_texture.loadFromFile("fish.png"))
		return 1;
	Sprite fish(fish_texture);
	fish.setPosition(300, 300);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::JoystickButtonPressed) {
				printf("joystick: %d button: %d\n",
				       event.joystickButton.joystickId,
				       event.joystickButton.button);
			}
			if (event.type == Event::JoystickButtonReleased) {
				printf("joyreles: %d button: %d\n",
				       event.joystickButton.joystickId,
				       event.joystickButton.button);
			}
			if (event.type == Event::JoystickMoved) {
				printf("joystick: %d axis: %d position: %f\n",
				       event.joystickMove.joystickId,
				       event.joystickMove.axis,
				       event.joystickMove.position);
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Q)
					window.close();
				if (event.key.code == Keyboard::Up)
					move(fish, 0, -10);
				if (event.key.code == Keyboard::Down)
					move(fish, 0, 10);
				if (event.key.code == Keyboard::Left)
					move(fish, -10, 0);
				if (event.key.code == Keyboard::Right)
					move(fish, 10, 0);
			}
		}

		//window.clear();
		window.draw(bg);
		window.draw(fish);
		window.display();
	}

	return 0;
}
