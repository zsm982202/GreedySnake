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

void GreedySnake::move(direction d) {
	if (!isGameOver()) {
		if (!isEat(d)) {
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
		cout << "GameOver!Dir" << dir << endl;
		Sleep(500000);
		exit(0);
	}
}

bool GreedySnake::isEat(direction d) {
	switch (d) {
	case GreedySnake::Left:
		if (node_front->x - 1 == fruit_x && node_front->y == fruit_y) {
			return true;
		}
		else
			return false;
	case GreedySnake::Right:
		if (node_front->x + 1 == fruit_x && node_front->y == fruit_y) {
			return true;
		}
		else
			return false;
	case GreedySnake::Up:
		if (node_front->x == fruit_x && node_front->y - 1 == fruit_y) {
			return true;
		}
		else
			return false;
	case GreedySnake::Down:
		if (node_front->x == fruit_x && node_front->y + 1 == fruit_y) {
			return true;
		}
		else
			return false;
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
		direction d;
		if (findPath(d, node_front->x, node_front->y, fruit_x, fruit_y)) {
			dir = d;
			move(d);
		}
		else {
			//cout << "false" << endl;
			Sleep(300000000);
		}
		
		
		show();
		Sleep(30);
	}
}

inline bool GreedySnake::judgeDir(int x, int y, direction &next_dir, direction d) {
	if (x < 0 || x >= max_width || y < 0 || y >= max_hight)
		//cout << "ok" << endl;
		return false;
	if (!visited_matrix[y*max_width + x]||(x==node_rear->x&&y==node_rear->y)) {
		int next_node_front_x;
		int next_node_front_y;
		switch (d) {
		case Left:
			if (dir == Right)
				return false;
			next_node_front_x = node_front->x - 1;
			next_node_front_y = node_front->y;
			break;
		case Right:
			if (dir == Left)
				return false;
			next_node_front_x = node_front->x + 1;
			next_node_front_y = node_front->y;
			break;
		case Up:
			if (dir == Down)
				return false;
			next_node_front_x = node_front->x;
			next_node_front_y = node_front->y - 1;
			break;
		case Down:
			if (dir == Up)
				return false;
			next_node_front_x = node_front->x;
			next_node_front_y = node_front->y + 1;
			break;
		}

		if (node_rear->prev == NULL) {
			next_dir = d;
			return true;
		}
		//如果走这一步能看到尾巴
		if (!isEat(d)) {
			if (findPathBFS(x, y, node_rear->prev->x, node_rear->prev->y, true)) {
				next_dir = d;
				return true;
			}
		}
		else {
			if (findPathBFS(x, y, node_rear->x, node_rear->y, true)) {
				next_dir = d;
				return true;
			}
		}
	}
	return false;
}

bool GreedySnake::findPath(direction &next_dir, int start_x, int start_y, int end_x, int end_y) {
	if (findPathBFS(start_x, start_y, end_x, end_y, false)) {
		//cout << "true";
		if (end_x >= start_x) {
			//Right&&Down
			if (end_y >= start_y) {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
					}
				}
			}
			//Right&&Up
			else {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
					}
				}
			}
		}
		else {
			//Left&&Down
			if (end_y >= start_y) {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
					}
				}
			}
			//Left&&Up
			else {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
					}
				}
			}
		}
	}
	else {
		if (end_x >= start_x) {
			//Right&&Down
			if (end_y >= start_y) {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
					}
				}
			}
			//Right&&Up
			else {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
					}
				}
			}
		}
		else {
			//Left&&Down
			if (end_y >= start_y) {
				if (abs(fruit_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
				}
				else if (abs(fruit_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
					}
				}
			}
			//Left&&Up
			else {
				if (abs(end_x - start_x) > abs(end_y - start_y)) {
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
				}
				else if (abs(end_x - start_x) < abs(end_y - start_y)) {
					if (judgeDir(start_x, start_y + 1, next_dir, Down))
						return true;
					if (judgeDir(start_x + 1, start_y, next_dir, Right))
						return true;
					if (judgeDir(start_x - 1, start_y, next_dir, Left))
						return true;
					if (judgeDir(start_x, start_y - 1, next_dir, Up))
						return true;
				}
				else {
					srand(time(0));
					int t = rand() % 2;
					if (t == 0) {
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
					}
					else {
						if (judgeDir(start_x, start_y + 1, next_dir, Down))
							return true;
						if (judgeDir(start_x + 1, start_y, next_dir, Right))
							return true;
						if (judgeDir(start_x - 1, start_y, next_dir, Left))
							return true;
						if (judgeDir(start_x, start_y - 1, next_dir, Up))
							return true;
					}
				}
			}
		}
	}
	return false;
}


bool GreedySnake::findPathBFS(int start_x, int start_y, int end_x, int end_y, bool flag) {
	assume_visited_matrix = new bool[max_hight*max_width];
	for (int i = 0; i < max_hight*max_width; i++) {
		assume_visited_matrix[i] = visited_matrix[i];
	}
	assume_visited_matrix[end_y*max_width + end_x] = false;
	if (flag) {
		assume_visited_matrix[node_rear->y*max_width + node_rear->x] = false;
	}
	Point p(start_x, start_y);
	queue<Point> queueBFS;
	queueBFS.push(p);

	assume_visited_matrix[start_y*max_width + start_x] = true;

	while (!queueBFS.empty()) {
		Point p = queueBFS.front();
		int temp_x = p.x;
		int temp_y = p.y;
		queueBFS.pop();

		if (temp_x == end_x && temp_y == end_y) {
			return true;
		}
		//Right
		Point p1(temp_x + 1, temp_y);
		if (p1.x >= 0 && p1.x < max_width && p1.y >= 0 && p1.y < max_hight) {
			if (!assume_visited_matrix[p1.y * max_width + p1.x]) {
				queueBFS.push(p1);
				assume_visited_matrix[p1.y * max_width + p1.x] = true;
			}
		}
		//Left
		Point p2(temp_x - 1, temp_y);
		if (p2.x >= 0 && p2.x < max_width && p2.y >= 0 && p2.y < max_hight) {
			if (!assume_visited_matrix[p2.y * max_width + p2.x]) {
				queueBFS.push(p2);
				assume_visited_matrix[p2.y * max_width + p2.x] = true;
			}
		}
		//Up
		Point p3(temp_x, temp_y - 1);
		if (p3.x >= 0 && p3.x < max_width && p3.y >= 0 && p3.y < max_hight) {
			if (!assume_visited_matrix[p3.y * max_width + p3.x]) {
				queueBFS.push(p3);
				assume_visited_matrix[p3.y * max_width + p3.x] = true;
			}
		}
		//Down
		Point p4(temp_x, temp_y + 1);
		if (p4.x >= 0 && p4.x < max_width && p4.y >= 0 && p4.y < max_hight) {
			if (!assume_visited_matrix[p4.y * max_width + p4.x]) {
				queueBFS.push(p4);
				assume_visited_matrix[p4.y * max_width + p4.x] = true;
			}
		}
	}
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
				cout << "* ";
			else if (flag2)
				cout << "# ";
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