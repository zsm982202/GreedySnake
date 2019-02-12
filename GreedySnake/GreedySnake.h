#ifndef _GREEDYSNAKE_H
#define _GREEDYSNAKE_H
#include <queue>
class Node {
public:
	Node *next;
	Node *prev;
	int x;
	int y;
};

class GreedySnake {
public:
	enum direction {
		Left, Right, Up, Down
	};
	GreedySnake(int hight = 10, int width = 10);
	int randomNum(int min_num, int max_num);//[0,max_num-1]
	void initSnake();
	void setRandomFruit();
	void updateVisitedMatrix();
	void move();
	bool isEat();
	bool isGameOver();
	void run();
	void show();
	void solveAI();
	bool findPath(direction &next_dir);
	void findPathDFS(int x, int y, int step, bool flag);
	bool findPathBFS(int x, int y);
private:
	int max_hight;
	int max_width;
	direction dir;
	int fruit_x;
	int fruit_y;
	Node *node_front;
	Node *node_rear;
	bool *visited_matrix;
	bool *assume_visited_matrix;
	int min_step;
	direction next_direction;
	bool isGetNextDirection;
	bool canFindPath;
	//queue<int *> queueBFS;
};
#endif