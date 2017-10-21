#include "stdafx.h"
#include "gGraphics.h"
#include "PrivateFuncs.h"
#include <Windows.h>
#include <iostream>
#include <cmath>
#include <SFML\Graphics.hpp>

using namespace std; using namespace sf;

RenderWindow window(sf::VideoMode(800, 600), "My window");
gGraphics graphics(window);

RectangleShape Player(Vector2f(40, 40));

Vector2<int> WASD(0, 0);
vector<CircleShape*> Fruits(0, new CircleShape(25, 25));

Font font;
Text Score;

int AmountOfFruitsOnScreen = 30;
int SpawnSpeed = 300;
int TempSpawn;
bool focus;
int score;

void Async();
int DistanceFrom(int x, int y) {
	return max(x, y) - min(x, y);
}
Vector2f Vector2DistanceFrom(Vector2f x, Vector2f y) {
	int v1x = x.x;
	int v1y = x.y;
	int v2x = y.x;
	int v2y = y.y;
	
	int xdist = DistanceFrom(v1x, v2x);
	int ydist = DistanceFrom(v1y, v2y);
	return Vector2f((float)xdist, (float)ydist);
}

int main()
{
	srand(time(0));

	Thread t = &Async;
	t.launch();
	focus = true;
	if (!font.loadFromFile("arial.ttf")) {
		cout << "Error loading font" << endl;
		sleep(seconds(5));
	}
	Score.setCharacterSize(24);
	Score.setFont(font);
	Score.setPosition(window.getSize().x / 2, window.getSize().y - 50);
	Score.setString("Score: " + to_string(0));
	Score.setFillColor(Color::White);
	while (window.isOpen()) {
		graphics.Render();
	}
    return 0;
}

void gGraphics::WindowRender() {
	window.draw(Player);
	for (int i = 0; i < Fruits.size(); i++) {
		if (Fruits[i] != nullptr) {
			window.draw(*Fruits[i]);
		}
	}
	window.draw(Score);
}

void gGraphics::Poll(Event e) {
	switch (e.type) {
		case Event::Closed:
			window.close();
			break;
		case Event::LostFocus:
			cout << "Lost focus" << endl;
			focus = false;
			break;
		case Event::GainedFocus:
			cout << "Gained focus" << endl;
			focus = true;
			break;
	}
}

void Logic() {
	for (int i = 0; i < 200; i++) {
		Player.move((float)WASD.x / 100, (float)WASD.y / 100);
		sleep(Time(microseconds(10)));
	}
	TempSpawn++;
	int Items = 0;
	if (TempSpawn > SpawnSpeed) {
		TempSpawn = 0;
		for (int i = 0; i < Fruits.size(); i++) {
			if (Fruits[i] != nullptr) {
				Items++;
			}
		}
		if (Items < AmountOfFruitsOnScreen) {
			Fruits.push_back(new CircleShape(25, 25));
			int x = rand() % window.getSize().x;
			int y = rand() % window.getSize().y;
			Fruits.back()->setPosition(x, y);
			cout << "Spawned a fruit with vector id " << Fruits.size() -1 << " at " << x << ", " << y << endl;
		}
		cout << "Amount of fruits on field is: " << Items +1 <<endl;
	
	}
	vector<Vector2f> dists(0);
	for (int i = 0; i < Fruits.size(); i++) {
		dists.push_back(Vector2DistanceFrom(Fruits[i]->getPosition(), Player.getPosition()));
		if (dists[i].x <= 40 && dists[i].y <= 40 && i != 2) {
			cout << "Collision!!" << endl;
			cout << i << endl;
			Fruits.erase(Fruits.begin() + i);
			score++;
		}
	}
	Score.setString("Score: " + to_string(score));
}

void Async() {
	while (window.isOpen()) {
		if (focus) {
			WASD.x = 0;
			WASD.y = 0;
			if (GetKeyState('W') < 0) {
				WASD.y = -1;
			}
			if (GetKeyState('A') < 0) {
				WASD.x = -1;

			}
			if (GetKeyState('S') < 0) {
				WASD.y = 1;
			}
			if (GetKeyState('D') < 0) {
				WASD.x = 1;
			}
			PrivateFuncs::ExecuteAfter(5, &Logic);
		}
	}
}


/*
Code may be needed later

for (int i = 0; i < Fruits.size(); i++) {
if (Fruits[i] == nullptr) {
if (Items < AmountOfFruitsOnScreen) {
Fruits.resize(Fruits.size() + 1);
Fruits[i] = (new CircleShape(25, 25));
int x = rand() % window.getSize().x;
int y = rand() % window.getSize().y;
Fruits[i]->setPosition(x, y);
cout << "Spawned a fruit with vector id " << i << " at " << x << ", " << y << endl;
sleep(seconds(1));
}
}
}

*/