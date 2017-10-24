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

bool hasTexture = true;

Font font;
Texture bomb;
Texture apple;
Texture banana;
Texture* tex[] = { &apple, &banana };
Text Score;

int _time = 6;

class Fruit
{
public:
	CircleShape* shape;
	bool Bad;
	bool Deleted;
	Texture* text;
	int time = _time;
	int timer = rand() % time;
	Fruit(/*Texture* tex,*/ bool Bad = true) {
		//this->tex = tex;
		this->Deleted = false;
		this->shape = new CircleShape(25, 25);
		this->shape->setOrigin(12.5f, 12.5f);
		//this->shape->setTexture(tex);
		this->Bad = Bad;
	}
	void DeleteBomb() {
		timer--;
		if (this->Bad ? timer < 2 : timer < 1) {
			this->Deleted = true;
		}
	}
	~Fruit() {
		cout << "Deleted this->shape" << endl;
		delete this->shape;
	}
};

Vector2<int> WASD(0, 0);
vector<Fruit*> Fruits(0);

int AmountOfFruitsOnScreen = 3;
int SpawnSpeed = 300;
int TempSpawn;
bool focus;
int score;
int lives = 3;


void Async();
int DistanceFrom(int x, int y) {
	return max(x, y) - min(x, y);
}
Vector2f Vector2DistanceFrom(Vector2f x, Vector2f y) {
	float xdist = DistanceFrom(x.x, y.x);
	float ydist = DistanceFrom(x.y, y.y);
	return Vector2f(xdist, ydist);
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
		return -1;
	}
	if (!apple.loadFromFile("apple.png") || !banana.loadFromFile("banana.png") || !bomb.loadFromFile("bomb.png")) {
		cout << "Error loading texture" << endl;
		sleep(seconds(5));
		return -1;
	}
	Score.setCharacterSize(24);
	Score.setFont(font);
	Score.setPosition(window.getSize().x / 2, window.getSize().y - 50);
	Score.setString("Score: " + to_string(0) + " Lives: " + to_string(lives));
	Score.setFillColor(Color::White);
	while (window.isOpen() && lives > 0) {
		graphics.Render();
	}
	if (lives <= 0) {
		Text txt;
		txt.setString("You lost!");
		txt.setFillColor(Color::White);
		txt.setCharacterSize(24);
		txt.setFont(font);
		txt.setPosition(window.getSize().x / 2, window.getSize().y / 2);
		window.clear();
		window.draw(txt);
		window.display();
		sleep(seconds(3));
	}
	t.wait();
	int size = Fruits.size();
	for (int i = 0; i < size; i++) {
		delete Fruits[i];
		cout << "Deleted Fruits[" + to_string(i) << "] of size " << to_string(sizeof(Fruit)) << endl;
	}
    return 0;
}

void gGraphics::WindowRender() {
	window.draw(Player);
	for (int i = 0; i < Fruits.size(); i++) {
		if (Fruits[i]->shape != nullptr) {
			window.draw(*Fruits[i]->shape);
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
	vector<Vector2f> dists(0);
	for (int i = 0; i < Fruits.size(); i++) {
		dists.push_back(Vector2DistanceFrom(Fruits[i]->shape->getPosition(), Player.getPosition()));
		if (dists[i].x <= 40 && dists[i].y <= 40) {
			if (Fruits[i]->Bad) lives--;
			else score++;
			Fruits.erase(Fruits.begin() + i);
		}
	}
	for (int i = 0; i < 200; i++) {
		Player.move((float)WASD.x / 100, (float)WASD.y / 100);
		sleep(Time(microseconds(10)));
	}
	TempSpawn++;
	if (TempSpawn % 400 == 1) {
		for (int i = 0; i < Fruits.size(); i++) {
			if (!Fruits[i]->Deleted) {
				Fruits[i]->DeleteBomb();
				if (Fruits[i]->Deleted) {
					Fruits.erase(Fruits.begin() + i);
				}
			}
		}
	}
	int Items = 0;
	cout << to_string(SpawnSpeed) << endl;
	int tmp = ((float)score / 3);
	SpawnSpeed = 300 / (score == 0 ? 0.4 : (score >= 10 ? (((float)score / 2.5) * (0.1 * (tmp <= 0 ? 2 : (float)score / 4))) : (((float)score / 2.5) * (0.1 * (tmp <= 0? 2 : (float)score / 3))) ) );
	if (TempSpawn > SpawnSpeed) {
		cout << SpawnSpeed << endl;
		TempSpawn = 0;
		for (int i = 0; i < Fruits.size(); i++) {
			if (!Fruits[i]->Deleted) {
				Items++;
			}
			else if (Items < AmountOfFruitsOnScreen){
				int bad = rand() % 4;
				Texture* te = bad == 1 ? &bomb : tex[rand() % 1];
				Fruits[i] = new Fruit(bad == 1);
				int x = rand() % window.getSize().x;
				int y = rand() % window.getSize().y;
				Fruits[i]->shape->setPosition(x, y);
				hasTexture ? Fruits[i]->shape->setTexture(te) : Fruits[i]->shape->setFillColor(bad ? Color::Red : Color::Blue);
				cout << "Spawned a fruit with vector id " << i << " at " << x << ", " << y << endl;
			}
		}
		if (Items < AmountOfFruitsOnScreen) {
			int bad = rand() % 4;
			Texture* te = bad == 1 ? &bomb : tex[rand() % 1];
			Fruits.push_back(new Fruit(bad == 1));
			int x = rand() % window.getSize().x;
			int y = rand() % window.getSize().y;
			Fruits.back()->shape->setPosition(x, y);
			hasTexture ? Fruits.back()->shape->setTexture(te) : Fruits.back()->shape->setFillColor(bad ? Color::Red : Color::Blue);
			cout << "Spawned a fruit with vector id " << Fruits.size() -1 << " at " << x << ", " << y << endl;
		}
	
	}
	Score.setString("Score: " + to_string(score) + " Lives: " + to_string(lives));
}

void Async() {
	while (window.isOpen() && lives > 0) {
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
