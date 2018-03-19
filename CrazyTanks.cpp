
// CrazyTanks(FinalSolution).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <conio.h>
#include <vector>
#include <ctime>

using namespace std;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

const int width = 20;
const int heigth = 20;
bool gameOver;
int wall_X1, wall_Y1, R;
char map[heigth][width + 1];
unsigned int start_time = clock();
unsigned int end_time;


class Player {
public:
	int x;
	int y;
	int healthPower;
	int ammunition;
	eDirection direction;
	eDirection memDirection;
	bool lifeIsSpent;

	Player() {
		direction = STOP;
		memDirection = UP;
		healthPower = 3;
		x = width / 2 - 1;
		y = heigth - 2;
	}

};

Player player;

class Bullet {
public:
	bool belongsToTheEnemy;
	int x;
	int y;
	eDirection dir;
	bool Fly() {
		if (map[y][x] == 'o') map[y][x] = ' ';
		if (x == player.x && y == player.y) {
			player.healthPower--;
		}
		switch (dir)
		{
		case LEFT:
			if (map[y][x - 1] == ' ') {
				map[y][--x] = 'o';
			}
			break;
		case RIGHT:
			if (map[y][x + 1] == ' ') {
				map[y][++x] = 'o';
			}
			break;
		case UP:
			if (map[y - 1][x] == ' ') {
				map[--y][x] = 'o';
			}
			break;
		case DOWN:
			if (map[y + 1][x] == ' ') {
				map[++y][x] = 'o';
			}
			break;
		default:
			return false;
		}
		return true;
	}
};
vector<Bullet> bullets;

void Shot(int y_pos, int x_pos, eDirection dir, bool belongsToTheEnemy) {
	int x = x_pos;
	int y = y_pos;
	Bullet bullet = Bullet();
	bullet.x = x;
	bullet.y = y;
	bullet.dir = dir;
	bullet.belongsToTheEnemy = belongsToTheEnemy;
	bullets.push_back(bullet);
}

class Enemy {
public:
	int x;
	int y;
	char direction;
	eDirection memDirection;
	int speed;

	Enemy() {
		x = 3;
		y = 3;
	}
	
	
	void Move() {
		srand(time(NULL));
		int randDir = 1 + rand() % 6;
		
		
		if (map[y][x] == 'O') map[y][x] = ' ';
		switch (randDir)
		{
		case LEFT:
			if (map[y][x - 1] == ' ') {
				map[y][--x] = 'O';
			}
			break;
		case RIGHT:
			if (map[y][x + 1] == ' ') {
				map[y][++x] = 'O';
			}
			break;
		case UP:
			if (map[y - 1][x] == ' ') {
				map[--y][x] = 'O';
			}
			break;
		case DOWN:
			if (map[y + 1][x] == ' ') {
				map[++y][x] = 'O';
			}
			break;
		default:
			if (x == player.x) {
				int dif = y - player.y;
				Shot(y, x, dif > 0 ? UP : DOWN, true);
			}
			else if (y == player.y) {
				int dif = x - player.x;
				Shot(y, x, dif > 0 ? LEFT : RIGHT, true);
			}
		}
		
	}
	
};

Enemy enemy1;


void Setup() {
	gameOver = false;

}

//Генерация границ карты
void ConstructBoundaries() {
	for (int i = 0; i < heigth; i++) {
		map[i][width] = '\n';
	}
	for (int i = 0; i < width; i++)
		map[0][i] = '#';
	for (int i = 1; i < heigth - 1; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width - 1)
				map[i][j] = '#';

			else
				map[i][j] = ' ';
		}
	}
	for (int i = 0; i < width; i++)
		map[heigth - 1][i] = '#';
}

//Генерация стен. Функция составлена таким образом, чтобы последующий блок стены не пересекался
//со стеной, сгенерированной функцией ConstructBoundaries(). Проверка на четность суммы координат
//определяет вдоль какой оси будет направлена стена.
void GenerateWalls() {
	srand(time(NULL));
	for (int j = 0; j < 5; j++) {

		R = 1 + rand() % 5;
		wall_X1 = 1 + rand() % (width - 1);
		wall_Y1 = 1 + rand() % (heigth - 2);

		double distributor = (wall_X1 + wall_Y1) % 2;
		if (distributor == 0) {
			if (wall_Y1 >(heigth / 2)) {
				for (int i = 0; i < R; i++) {
					map[wall_Y1 - i][wall_X1] = '#';
				}
			}
			else {
				for (int i = 0; i < R; i++) {
					map[wall_Y1 + i][wall_X1] = '#';
				}
			}
		}
		else {
			if (wall_X1 >(width / 2)) {
				for (int i = 0; i < R; i++) {
					map[wall_Y1][wall_X1 - i] = '#';
				}
			}
			else {
				for (int i = 0; i < R; i++) {
					map[wall_Y1][wall_X1 + i] = '#';
				}
			}
		}
	}
}


//Отрисовка

void Draw() {
	system("cls");

	for (int i = 0; i < heigth; i++) {
		for (int j = 0; j < width + 1; j++) {
			if (i == player.y && j == player.x) {
				cout << '0';
			}
			else
				cout << map[i][j];
		}
	}

	for (vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
		if (!(*it).Fly()) {
			
			bullets.erase(it);
		}
	}

	

}

//Получение информации о состоянии танка игрока и времени игрового сеанса
void getInfo() {
	cout << "HP: " << player.healthPower << endl;
	end_time = clock();
	cout << "you play " << (end_time - start_time) / 1000 << " sec" << endl;
	Sleep(50);
}

//Слушатель ввода
void Input() {
	if (_kbhit()) {
		switch (_getch())
		{
		case ' ':
			Shot(player.y, player.x, player.memDirection, false);
			break;
		case 'w':
			player.direction = UP;
			player.memDirection = UP;
			break;
		case 'a':
			player.direction = LEFT;
			player.memDirection = LEFT;
			break;
		case 's':
			player.direction = DOWN;
			player.memDirection = DOWN;
			break;
		case 'd':
			player.direction = RIGHT;
			player.memDirection = RIGHT;
			break;
		}
	}
	else
		player.direction = STOP;
}

//Логические компоненты игры
void Logic() {
	switch (player.direction) {
	case UP:
		if (map[player.y - 1][player.x] == ' ') {
			player.y--;
		}
		break;
	case LEFT:
		if (map[player.y][player.x - 1] == ' ') {
			player.x--;
		}
		break;
	case DOWN:
		if (map[player.y + 1][player.x] == ' ') {
			player.y++;
		}
		break;
	case RIGHT:
		if (map[player.y][player.x + 1] == ' ') {
			player.x++;
		}
		break;
	}
}

// main,
int main()
{
	Setup();
	ConstructBoundaries();
	GenerateWalls();

	while (player.healthPower != 0) {
		Draw();
		enemy1.Move();
		getInfo();
		Input();
		Logic();
	}
	return 0;
}

