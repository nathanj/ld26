#define _USE_MATH_DEFINES

#include <list>
#include <string>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Fish.hpp"
#include "GameSprite.hpp"
#include "Hook.hpp"
#include "TextParticle.hpp"

#if _MSC_VER
#define snprintf _snprintf
#endif

sf::Music music;

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
	assert(music.openFromFile("bg.ogg"));
}

static Fish *find_fish(int x, int y)
{
	for (auto it = fishes.begin(); it != fishes.end(); it++) {
		if ((*it)->state != Fish::Normal)
			continue;
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

enum State {
	Title,
	Playing,
	GameOver
} state;

sf::Clock game_clock;
sf::Time prev;

void initialize()
{
	game_clock.restart();
	prev = game_clock.getElapsedTime();
	score = 0;
	multiplier = 1;

	for (auto it = fishes.begin(); it != fishes.end(); it++)
		delete *it;
	fishes.clear();
	for (auto it = particles.begin(); it != particles.end(); it++)
		delete *it;
	particles.clear();
	if (hook) {
		delete hook;
		hook = NULL;
	}

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
}

int main(int, char **)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
	int frames = 0;

	srand(time(NULL));

	load_data();

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	state = Title;
restart:
	initialize();
	while (window.isOpen()) {
		sf::Time time = game_clock.getElapsedTime();
		sf::Time delta = time - prev;
		prev = time;
		frames++;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q
				    || event.key.code == sf::Keyboard::Escape)
					window.close();
			}

			if (state == Playing) {
				if (event.type == sf::Event::MouseButtonPressed) {
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

			if (state == Title || state == GameOver) {
				if (event.type == sf::Event::MouseButtonPressed) {
					sf::IntRect rect(80, 480, 670-80, 580-480);
					if (rect.contains(event.mouseButton.x, event.mouseButton.y)) {
						state = Playing;
						music.stop();
						music.setVolume(100);
						music.play();
						goto restart;
					}
				}
			}
		}

		if (state == GameOver) {
			sf::sleep(sf::microseconds(16));
			music.setVolume(music.getVolume() * 0.99999);
		}

		if (state == Title || state == Playing) {
			for (auto it = fishes.begin(); it != fishes.end(); it++)
				(*it)->update(window, delta);

			if (state == Playing)
				hook->update(window, delta);

			sun.setPosition(time.asSeconds()*7 - 80, 130 - 100*sinf(M_PI * time.asSeconds()/120.0));
			if (selected && selected->state == Fish::Selected) {
				sf::Vector2i pos = sf::Mouse::getPosition(window);

				if (pos.x < 5)
					pos.x = 0;

				// Stop if running into the grass.
				if (pos.x > (700 + (550 - 700) * (selected->getPosition().y - 200)/400.0) - 30)
					pos.x = (700 + (550 - 700) * (selected->getPosition().y - 200)/400.0) - 30;;

				selected->setPosition(pos.x, selected->getPosition().y);
			}

			//window.clear();
			window.draw(bg[0]);
			window.draw(sun);
			window.draw(bg[1]);
			window.draw(dude);
			for (auto it = fishes.begin(); it != fishes.end(); it++)
				window.draw(**it);
			if (state == Playing)
				window.draw(*hook);
			for (auto it = particles.begin(); it != particles.end(); it++) {
				TextParticle *p = *it;
				if (!p)
					continue;
				p->update(window, delta);
				if (p->isDead()) {
					delete p;
					*it = NULL;
				} else {
					window.draw(p->text);
				}
				/*
				if (p->isDead()) {
					//delete *it;
					particles.erase(it);
					it--;
					continue;
				}
				window.draw(p->text);*/
			}

			if (state == Playing) {
				char buf[256];
				snprintf(buf, sizeof(buf), "Time Left: %d", 120 - (int) time.asSeconds());
				sf::Text text(buf, font, 24);
				text.setStyle(sf::Text::Bold);
				text.setColor(sf::Color(0, 0, 30, 255));
				window.draw(text);

				snprintf(buf, sizeof(buf), "Score: %d", score);
				text.setString(buf);
				text.setPosition(200, 0);
				window.draw(text);
			}

			if (state == Playing && time.asSeconds() >= 120) {
				sf::RectangleShape rect(sf::Vector2f(800, 600));
				rect.setFillColor(sf::Color(0, 0, 0, 150));
				window.draw(rect);
				state = GameOver;

				sf::Text text;
				text.setFont(font);
				char buf[256];
				text.setCharacterSize(50);
				text.setColor(sf::Color(255, 255, 255, 255));

				snprintf(buf, sizeof(buf), "Time's Up!");
				text.setString(buf);
				text.setPosition(230, 200);
				window.draw(text);
				snprintf(buf, sizeof(buf), "Your score was:");
				text.setString(buf);
				text.setPosition(180, 250);
				window.draw(text);
				snprintf(buf, sizeof(buf), "%d", score);
				text.setColor(sf::Color(255, 255, 200, 255));
				text.setString(buf);
				text.setPosition(280, 300);
				window.draw(text);

				snprintf(buf, sizeof(buf), "Click here to play again");
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 500);
				window.draw(text);
			}

			if (state == Title) {
				sf::Text text;
				char buf[256];
				sf::RectangleShape rect(sf::Vector2f(800, 600));
				rect.setFillColor(sf::Color(0, 0, 0, 150));
				window.draw(rect);

				text.setFont(font);
				text.setCharacterSize(80);
				text.setColor(sf::Color(255, 255, 255, 255));

				snprintf(buf, sizeof(buf), "Fisherdude");
				text.setString(buf);
				text.setPosition(70, 30);
				window.draw(text);

				snprintf(buf, sizeof(buf), "Instructions:");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 250);
				window.draw(text);
				snprintf(buf, sizeof(buf), "Click and drag fishes left and right.");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 280);
				window.draw(text);
				snprintf(buf, sizeof(buf), "Catch smaller fishes to increase multiplier");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 310);
				window.draw(text);
				snprintf(buf, sizeof(buf), "then go for the big catch!");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(130, 340);
				window.draw(text);
				snprintf(buf, sizeof(buf), "Get a high score before the sun goes down!");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 370);
				window.draw(text);

				snprintf(buf, sizeof(buf), "Points: ");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 430);
				window.draw(text);

				snprintf(buf, sizeof(buf), "10");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(210, 450);
				window.draw(text);

				snprintf(buf, sizeof(buf), "50");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(320, 450);
				window.draw(text);

				snprintf(buf, sizeof(buf), "200");
				text.setCharacterSize(20);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(430, 450);
				window.draw(text);

				sf::Sprite s(fish_texture[0]);
				s.setPosition(210, 420);
				s.setScale(2,2);
				window.draw(s);

				sf::Sprite s2(fish_texture[1]);
				s2.setPosition(300, 410);
				s2.setScale(2,2);
				window.draw(s2);

				sf::Sprite s3(fish_texture[2]);
				s3.setPosition(400, 420);
				s3.setScale(2,2);
				window.draw(s3);

				snprintf(buf, sizeof(buf), "Click here to play");
				text.setCharacterSize(50);
				text.setColor(sf::Color(255, 255, 255, 255));
				text.setString(buf);
				text.setPosition(100, 500);
				window.draw(text);
			}

			window.display();
		}
	}

	for (auto it = fishes.begin(); it != fishes.end(); it++)
		delete *it;
	delete hook;
	for (auto it = particles.begin(); it != particles.end(); it++)
		delete *it;

	return 0;
}
