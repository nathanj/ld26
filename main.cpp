#include <list>
#include <string>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Fish.hpp"
#include "GameSprite.hpp"
#include "Hook.hpp"
#include "TextParticle.hpp"

sf::Texture bg_texture[2];
sf::Sprite bg[2];
sf::Texture hook_texture;
Hook *hook;

sf::Texture sun_texture;;
sf::Sprite sun;

sf::Texture font_texture[12];

sf::Texture dude_texture[3];
sf::Sprite dude;
sf::Texture fish_texture[3];
std::vector<Fish*> fishes;
std::vector<TextParticle*> particles;

sf::Font font;
Fish *selected;

int score;
int multiplier;

void load_data()
{
	assert(bg_texture[0].loadFromFile("bg.png"));
	bg[0].setTexture(bg_texture[0]);
	assert(bg_texture[1].loadFromFile("bg2.png"));
	bg[1].setTexture(bg_texture[1]);
	assert(sun_texture.loadFromFile("sun.png"));
	sun.setTexture(sun_texture);
	sun.setPosition(-80, 100);
	assert(hook_texture.loadFromFile("hook.png"));
	assert(fish_texture[0].loadFromFile("fish.png"));
	assert(fish_texture[1].loadFromFile("fish2.png"));
	assert(fish_texture[2].loadFromFile("fish3.png"));
	assert(dude_texture[0].loadFromFile("dude.png"));
	assert(dude_texture[1].loadFromFile("dude2.png"));
	assert(dude_texture[2].loadFromFile("dude3.png"));
	dude.setTexture(dude_texture[0]);
	dude.setPosition(570, 53);

	for (int i = 0; i < 12; i++)
		assert(font_texture[i].loadFromFile("font.png",
						    sf::IntRect(i*32, 0, 32, 32)));

	assert(font.loadFromFile("Arial.ttf"));
}

static Fish *find_fish(int x, int y)
{
	for (auto it = fishes.begin(); it != fishes.end(); it++) {
		sf::FloatRect bounds = (*it)->getGlobalBounds();
		bounds.left -= 3;
		bounds.top -= 3;
		bounds.width += 3;
		bounds.height += 3;
		if (bounds.contains(x, y))
			return *it;
	}
	return NULL;
}


int main(int, char **)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
	sf::Clock clock;
	sf::Time prev = clock.getElapsedTime();
	int frames = 0;
	sf::Vector2i mouse_positions[5];
	//int mouse_index = 0;

	srand(time(NULL));

	load_data();

	score = 0;
	multiplier = 1;

	for (int i = 0; i < 5; i++)
		mouse_positions[0] = sf::Mouse::getPosition(window);

	for (int i = 0; i < 30; i++) {
		Fish *f = new Fish(fish_texture[0], 10);
		f->setPosition(rand() % 600 + 20, 250 + rand() % 200);
		f->setColor(sf::Color(255-rand()%50, 255-rand()%50, 255-rand()%50, 255));
		fishes.push_back(f);
	}

	for (int i = 0; i < 20; i++) {
		Fish *f = new Fish(fish_texture[1], 50);
		f->setPosition(rand() % 600 + 20, 300 + rand() % 200);
		f->setColor(sf::Color(255-rand()%50, 255-rand()%50, 255-rand()%50, 255));
		fishes.push_back(f);
	}

	for (int i = 0; i < 5; i++) {
		Fish *f = new Fish(fish_texture[2], 200);
		f->setPosition(rand() % 500 + 20, 400 + rand() % 200);
		f->setColor(sf::Color(255-rand()%50, 255-rand()%50, 255-rand()%50, 255));
		fishes.push_back(f);
	}

	hook = new Hook(hook_texture);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	while (window.isOpen()) {
		sf::Time time = clock.getElapsedTime();
		sun.setPosition(time.asSeconds()*8 - 80, 130 - 100*sinf(M_PI * time.asSeconds()/90.0));
		sf::Time delta = time - prev;
		prev = time;
		frames++;

		if (selected && selected->state == Fish::Selected) {
			sf::Vector2i pos = sf::Mouse::getPosition(window);

			if (pos.x < 5)
				pos.x = 0;

			// Stop if running into the grass.
			if (pos.x > (700 + (550 - 700) * (selected->getPosition().y - 200)/400.0) - 30)
				pos.x = (700 + (550 - 700) * (selected->getPosition().y - 200)/400.0) - 30;;

			selected->setPosition(pos.x, selected->getPosition().y);
		}

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q)
					window.close();
			} else if (event.type == sf::Event::MouseButtonPressed) {
				selected = find_fish(event.mouseButton.x, event.mouseButton.y);
				if (selected)
					selected->state = Fish::Selected;
			} else if (event.type == sf::Event::MouseButtonReleased) {
				if (selected && selected->state == Fish::Selected) {
					selected->state = Fish::Normal;
					selected->velocity.x = 0;
					selected->velocity.y = 0;
				}
				selected = NULL;
			}
		}

		for (auto it = fishes.begin(); it != fishes.end(); it++)
			(*it)->update(window, delta);
		hook->update(window, delta);

		//window.clear();
		window.draw(bg[0]);
		window.draw(sun);
		window.draw(bg[1]);
		window.draw(dude);
		for (auto it = fishes.begin(); it != fishes.end(); it++)
			window.draw(**it);
		window.draw(*hook);
		for (auto it = particles.begin(); it != particles.end(); it++) {
			TextParticle *p = *it;
			p->update(window, delta);
			if (p->isDead()) {
				particles.erase(it);
				delete *it;
				it--;
				continue;
			}
			window.draw(p->text);
		}

		char buf[256];
		snprintf(buf, sizeof(buf), "Time Left: %d", 100 - (int) time.asSeconds());
		sf::Text text(buf, font, 24);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color(0, 0, 30, 255));
		window.draw(text);

		snprintf(buf, sizeof(buf), "Score: %d", score);
		text.setString(buf);
		text.setPosition(200, 0);
		window.draw(text);

		window.display();
	}

	sf::Time time = clock.getElapsedTime();
	printf("%d frames in %.2f seconds = %.2f FPS\n", frames, time.asSeconds(),
	       frames / time.asSeconds());

	for (auto it = fishes.begin(); it != fishes.end(); it++)
		delete *it;
	delete hook;
	for (auto it = particles.begin(); it != particles.end(); it++)
		delete *it;

	return 0;
}
