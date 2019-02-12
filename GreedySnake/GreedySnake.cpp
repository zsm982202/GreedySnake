#include "GreedySnake.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <random>
using namespace std;

GreedySnake::GreedySnake(int hight/*= 10*/, int width/*= 10*/) {
	max_hight = hight;
	max_width = width;
	visited_matrix = new bool[max_hight*max_width];
	for (int i = 0; i < max_hight; i++) {
		for (int j = 0; j < max_width; j++)
			visited_matrix[i*max_width + j] = false;
	}
	dir = Left;
	initSnake();
}

void GreedySnake::initSnake() {
	int temp_x = randomNum(0, max_width);
	int temp_y = randomNum(0, max_hight);
	Node *temp_node = new Node;
	temp_node->next = NULL;
	temp_node->prev = NULL;
	temp_node->x = temp_x;
	temp_node->y = temp_y;
	node_front = node_rear = temp_node;
	setRandomFruit();
	updateVisitedMatrix();
}

void GreedySnake::updateVisitedMatrix() {
	for (int i = 0; i < max_hight; i++) {
		for (int j = 0; j < max_width; j++)
			visited_matrix[i*max_width + j] = false;
	}
	Node *p = node_front;
	while (p != NULL) {
		visited_matrix[p->y*max_width + p->x] = true;
		p = p->next;
	}
}

void GreedySnake::move() {
	if (!isGameOver()) {
		if (!isEat()) {
			Node *node_temp_front = new Node;
			switch (dir) {
			case GreedySnake::Left:
				node_temp_front->x = node_front->x - 1;
				node_temp_front->y = node_front->y;
				break;
			case GreedySnake::Right:
				node_temp_front->x = node_front->x + 1;
				node_temp_front->y = node_front->y;
				break;
			case GreedySnake::Up:
				node_temp_front->x = node_front->x;
				node_temp_front->y = node_front->y - 1;
				break;
			case GreedySnake::Down:
				node_temp_front->x = node_front->x;
				node_temp_front->y = node_front->y + 1;
				break;
			default:
				break;
			}
			node_temp_front->prev = NULL;
			node_temp_front->next = node_front;
			node_front->prev = node_temp_front;
			node_front = node_temp_front;
			node_rear = node_rear->prev;
			node_rear->next = NULL;
		}
		else {
			Node *node_temp_front = new Node;
			node_temp_front->x = fruit_x;
			node_temp_front->y = fruit_y;
			node_temp_front->prev = NULL;
			node_temp_front->next = node_front;
			node_front->prev = node_temp_front;
			node_front = node_temp_front;
			setRandomFruit();
		}
		updateVisitedMatrix();
	}
	else {
		cout << "GameOver" << endl;
		exit(0);
	}
}

bool GreedySnake::isEat() {
	switch (dir) {
	case GreedySnake::Left:
		if (node_front->x - 1 == fruit_x && node_front->y == fruit_y)
			return true;
	case GreedySnake::Right:
		if (node_front->x + 1 == fruit_x && node_front->y == fruit_y)
			return true;
	case GreedySnake::Up:
		if (node_front->x == fruit_x && node_front->y - 1 == fruit_y)
			return true;
	case GreedySnake::Down:
		if (node_front->x == fruit_x && node_front->y + 1 == fruit_y)
			return true;
	default:
		return false;
	}
}

bool GreedySnake::isGameOver() {
	if (dir == Left && node_front->x == 0) {
		return true;
	}
	else if (dir == Right && node_front->x == max_width - 1) {
		return true;
	}
	else if (dir == Up && node_front->y == 0) {
		return true;
	}
	else if (dir == Down && node_front->y == max_hight - 1) {
		return true;
	}
	bool *flag = new bool[max_hight*max_width];
	for (int i = 0; i < max_hight*max_width; i++) {
		flag[i] = false;
	}
	Node *p = node_front;
	while (p != NULL) {
		if (flag[p->y*max_width + p->x])
			return true;
		flag[p->y*max_width + p->x] = true;
		p = p->next;
	}
	return false;
}

int GreedySnake::randomNum(int min_num, int max_num) {
	random_device e;
	uniform_int_distribution<unsigned> u(min_num, max_num - 1);
	return u(e);
}

void GreedySnake::setRandomFruit() {
	bool isOverrided = false;
	int temp_x;
	int temp_y;
	do {
		isOverrided = false;
		temp_x = randomNum(0, max_width);
		temp_y = randomNum(0, max_hight);
		Node *p = node_front;
		while (p != NULL) {
			if (p->x == temp_x && p->y == temp_y)
				isOverrided = true;
			p = p->next;
		}
	} while (isOverrided);
	fruit_x = temp_x;
	fruit_y = temp_y;
}

void GreedySnake::run() {
	char input;
	while (true) {
		if (_kbhit()) {
			input = _getch();
			switch (input) {
			case 'w':
				if (dir != Down)
					dir = Up;
				break;
			case 'a':
				if (dir != Right)
					dir = Left;
				break;
			case 's':
				if (dir != Up)
					dir = Down;
				break;
			case 'd':
				if (dir != Left)
					dir = Right;
				break;
			default:
				break;
			}
		}
		move();
		show();
		direction d;
		findPath(d);
		//cout << findPath(d) << endl;
		//cout << d << endl;
		Sleep(500);
	}
}

bool GreedySnake::findPath(direction &next_dir) {
	if (findPathBFS(node_front->x, node_front->y)) {
		if (fruit_x >= node_front->x) {
			if (visited_matrix[node_front->y*max_width + node_front->x + 1]) {
				next_dir = Right;
				return true;
			}
			if (fruit_y >= node_front->y) {
				if (abs(fruit_x - node_front->x) >= abs(fruit_y - node_front->y)) {
					if (visited_matrix[(node_front->y + 1)*max_width + node_front->x]) {
						next_dir = Down;
						return true;
					}
					else if (visited_matrix[(node_front->y - 1)*max_width + node_front->x]) {
						next_dir = Up;
						return true;
					}

				}
			}
			else {
				if (abs(fruit_x - node_front->x) >= abs(fruit_y - node_front->y)) {
					if (visited_matrix[node_front->y*max_width + node_front->x + 1]) {
						next_dir = Right;
						return true;
					}
					else if (visited_matrix[(node_front->y + 1)*max_width + node_front->x]) {
						next_dir = Down;
						return true;
					}
					else if (visited_matrix[(node_front->y - 1)*max_width + node_front->x]) {
						next_dir = Up;
						return true;
					}
					else {
						next_dir = Left;
						return true;
					}
				}
			}
		}
		else {
			if (fruit_y >= node_front->y) {

			}
			else {

			}
		}
	}
	return false;
}

void GreedySnake::findPathDFS(int x, int y, int step, bool flag) {
	if (isGetNextDirection)
		return;
	assume_visited_matrix[y*max_width + x] = true;
	if (!flag) {
		if (x == fruit_x && y == fruit_y) {
			canFindPath = true;
			if (min_step > step) {
				cout << step << endl;
				min_step = step;
			}
		}
	}
	else {
		if (x == fruit_x && y == fruit_y) {
			if (min_step == step) {
				isGetNextDirection = true;
				return;
			}
		}
	}
	if (x > 0 && !assume_visited_matrix[y*max_width + x - 1]) {
		if (step == 0) {
			next_direction = Left;
		}
		findPathDFS(x - 1, y, step + 1, flag);
		if (isGetNextDirection)
			return;
	}
	if (x < max_width - 1 && !assume_visited_matrix[y*max_width + x + 1]) {
		if (step == 0) {
			next_direction = Right;
		}
		findPathDFS(x + 1, y, step + 1, flag);
		if (isGetNextDirection)
			return;
	}
	if (y > 0 && !assume_visited_matrix[(y - 1)*max_width + x]) {
		if (step == 0) {
			next_direction = Up;
		}
		findPathDFS(x, y - 1, step + 1, flag);
		if (isGetNextDirection)
			return;
	}
	if (y < max_hight && !assume_visited_matrix[(y + 1)*max_width + x]) {
		if (step == 0) {
			next_direction = Down;
		}
		findPathDFS(x, y + 1, step + 1, flag);
		if (isGetNextDirection)
			return;
	}
	assume_visited_matrix[y*max_width + x] = false;
}

bool GreedySnake::findPathBFS(int x, int y) {
	assume_visited_matrix = new bool[max_hight*max_width];
	for (int i = 0; i < max_hight*max_width; i++) {
		assume_visited_matrix[i] = visited_matrix[i];
	}

	/*queueBFS.push({ x,y });
	assume_visited_matrix[y*max_width + x] = true;
	while (!queueBFS.empty()) {
		int *temp;
		queueBFS.pop(temp);
		int temp_x = temp[0];
		int temp_y = temp[1];

		if (temp_x == fruit_x && temp_y == fruit_y) {
			return true;
		}

		//Right
		if (!assume_visited_matrix[temp_y*max_width + temp_x + 1]) {
			queueBFS.push({ temp_x + 1,temp_y });
			assume_visited_matrix[temp_y*max_width + temp_x + 1] = true;
		}
		//Left
		if (!assume_visited_matrix[temp_y*max_width + temp_x - 1]) {
			queueBFS.push({ temp_x - 1,temp_y });
			assume_visited_matrix[temp_y*max_width + temp_x - 1] = true;
		}
		//Up
		if (!assume_visited_matrix[(temp_y - 1)*max_width + temp_x]) {
			queueBFS.push({ temp_x,temp_y - 1 });
			assume_visited_matrix[(temp_y - 1)*max_width + temp_x] = true;
		}
		//Down
		if (!assume_visited_matrix[(temp_y + 1)*max_width + temp_x]) {
			queueBFS.push({ temp_x,temp_y + 1 });
			assume_visited_matrix[(temp_y + 1)*max_width + temp_x] = true;
		}
	}*/
	return false;
}

void GreedySnake::show() {
	system("cls");
	for (int i = 0; i < max_width + 2; i++) {
		cout << "@ ";
	}
	cout << endl;
	for (int i = 0; i < max_hight; i++) {
		cout << "@ ";
		for (int j = 0; j < max_width; j++) {
			int flag1 = false;
			int flag2 = false;
			Node *p = node_front;
			while (p != NULL) {
				if (p->x == j && p->y == i)
					flag1 = true;
				p = p->next;
			}
			if (fruit_x == j && fruit_y == i)
				flag2 = true;
			if (flag1)
				cout << "# ";
			else if (flag2)
				cout << "* ";
			else
				cout << "  ";
		}
		cout << "@" << endl;
	}
	for (int i = 0; i < max_width + 2; i++) {
		cout << "@ ";
	}
	cout << endl;
}