#ifndef WIDGET_H
#define WIDGET_H

#pragma execution_character_set("utf-8")

#define MAX_X 10
#define MAX_Y 10

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <vector>
using namespace std;

class Node {
  public:
    Node *next;
    Node *prev;
    int x;
    int y;
};

class Point {
  public:
    Point(int xx, int yy) {
        x = xx;
        y = yy;
    }
    int x;
    int y;
};

class widget : public QWidget
{
    Q_OBJECT
public:
    explicit widget(QWidget *parent = nullptr);
    void init();
    enum direction { Left, Right, Up, Down };
    void updateShowMatrix();
private:
    const int max_height;
    const int max_width;
    int fruit_x;
    int fruit_y;
    int min_step;
    int gridSize;
    bool isGetNextDirection;
    bool canFindPath;
    bool isWin;
    direction dir;
    direction next_direction;
    Node *node_front;
    Node *node_rear;
    vector<bool> visited_matrix;
    vector<bool> assume_visited_matrix;
    vector<int> showMatrix;

    inline int randomNum(int min_num, int max_num); //[0,max_num-1]
    void initSnake();
    void setRandomFruit();
    void updateVisitedMatrix();
    void move(direction d);
    bool isEat(direction d);
    bool isGameOver();
    inline bool judgeDir(int x, int y, direction &next_dir, direction dir);
    int dist(int start_x, int start_y, int end_x, int end_y);
    void findPath(direction &next_dir, int start_x, int start_y, int end_x,
                  int end_y);
    bool findPathBFS(int start_x, int start_y, int end_x, int end_y, bool flag);
signals:

public slots:
    void slotMoveStep();//槽函数，可以当做普通函数使用


private:
    QLabel *matrix[MAX_X][MAX_Y];
    QLabel *cornerMatrix[MAX_X][MAX_Y][2];
    QTimer timer;
};

#endif // WIDGET_H
