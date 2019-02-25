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

class Point {
public:
	Point(int xx, int yy) { x = xx; y = yy; }
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
	void move(direction d);
	bool isEat(direction d);
	bool isGameOver();
	void run();
	void show();
	inline bool judgeDir(int x, int y, direction &next_dir, direction dir);
	void findPath(direction &next_dir, int start_x, int start_y, int end_x, int end_y);
	bool findPathBFS(int start_x, int start_y, int end_x, int end_y, bool flag);
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
};
#endif