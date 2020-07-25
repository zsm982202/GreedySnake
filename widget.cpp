#include "widget.h"
#include <QTimer>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <random>
#include <QWidget>
#include <queue>
#include <vector>
using namespace std;


widget::widget(QWidget *parent) :
      QWidget(parent), max_height(MAX_Y), max_width(MAX_X)
{
    this->setWindowTitle("贪吃蛇");
    init();
    connect(&timer, SIGNAL(timeout()), this, SLOT(slotMoveStep()));
    timer.start(30);
}

void widget::init(){
    visited_matrix = vector<bool>(max_height * max_width, false);
    dir = Left;
    showMatrix = vector<int>(max_width * max_height);
    gridSize = 40;
    for(int x = 0; x < max_width; x++){
        for(int y = 0; y < max_height; y++){
            QLabel *label = new QLabel(this);
            label->setStyleSheet("background:white");
            label->setGeometry(x * gridSize, y * gridSize, gridSize, gridSize);
            matrix[x][y] = label;
            QLabel *cornerLabel = new QLabel(this);
            cornerLabel->setStyleSheet("background:blue");
            cornerLabel->setGeometry(x * gridSize, y * gridSize, gridSize, gridSize);
            cornerLabel->hide();
            QLabel *cornerLabel2 = new QLabel(this);
            cornerLabel2->setStyleSheet("background:blue");
            cornerLabel2->setGeometry(x * gridSize, y * gridSize, gridSize, gridSize);
            cornerLabel2->hide();
            cornerMatrix[x][y][0] = cornerLabel;
            cornerMatrix[x][y][1] = cornerLabel2;
        }
    }
    initSnake();
}

void widget::slotMoveStep(){
    if(!isWin) {
        direction d;
        findPath(d, node_front->x, node_front->y, fruit_x, fruit_y);
        dir = d;
        move(dir);
        updateShowMatrix();
    }
}

void widget::initSnake() {
    isWin = false;
    int temp_x = randomNum(0, max_width);
    int temp_y = randomNum(0, max_height);
    Node *temp_node = new Node;
    temp_node->next = NULL;
    temp_node->prev = NULL;
    temp_node->x = temp_x;
    temp_node->y = temp_y;
    visited_matrix[temp_y * max_width + temp_x] = true;
    node_front = node_rear = temp_node;
    setRandomFruit();
    updateVisitedMatrix();
}

void widget::updateVisitedMatrix() {
    for (int i = 0; i < max_height * max_width; i++) {
            visited_matrix[i] = false;
    }
    Node *p = node_front;
    while (p != NULL) {
        visited_matrix[p->y * max_width + p->x] = true;
        p = p->next;
    }
}

void widget::move(direction d) {
    if (!isGameOver()) {
        if (!isEat(d)) {
            Node *node_temp_front = new Node;
            switch (dir) {
            case widget::Left:
                node_temp_front->x = node_front->x - 1;
                node_temp_front->y = node_front->y;
                break;
            case widget::Right:
                node_temp_front->x = node_front->x + 1;
                node_temp_front->y = node_front->y;
                break;
            case widget::Up:
                node_temp_front->x = node_front->x;
                node_temp_front->y = node_front->y - 1;
                break;
            case widget::Down:
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
        } else {
            Node *node_temp_front = new Node;
            node_temp_front->x = fruit_x;
            node_temp_front->y = fruit_y;
            node_temp_front->prev = NULL;
            node_temp_front->next = node_front;
            node_front->prev = node_temp_front;
            node_front = node_temp_front;
            Node *p = node_front;
            int count = 0;
            while(p) {
                count++;
                p = p->next;
            }
            if(count != max_width * max_height)
                setRandomFruit();
            else {
                updateVisitedMatrix();
                isWin = true;
                updateShowMatrix();
            }
        }
        updateVisitedMatrix();
    } else {
        cout << "GameOver!" << endl;
        exit(0);
    }
}

bool widget::isEat(direction d) {
    switch (d) {
    case widget::Left:
        if (node_front->x - 1 == fruit_x && node_front->y == fruit_y) {
            return true;
        } else
            return false;
    case widget::Right:
        if (node_front->x + 1 == fruit_x && node_front->y == fruit_y) {
            return true;
        } else
            return false;
    case widget::Up:
        if (node_front->x == fruit_x && node_front->y - 1 == fruit_y) {
            return true;
        } else
            return false;
    case widget::Down:
        if (node_front->x == fruit_x && node_front->y + 1 == fruit_y) {
            return true;
        } else
            return false;
    default:
        return false;
    }
}

bool widget::isGameOver() {
    if (dir == Left && node_front->x == 0) {
        return true;
    } else if (dir == Right && node_front->x == max_width - 1) {
        return true;
    } else if (dir == Up && node_front->y == 0) {
        return true;
    } else if (dir == Down && node_front->y == max_height - 1) {
        return true;
    }
    vector<bool> flag(max_height * max_width, false);
    Node *p = node_front;
    while (p != NULL) {
        if (flag[p->y * max_width + p->x])
            return true;
        flag[p->y * max_width + p->x] = true;
        p = p->next;
    }
    return false;
}

inline int widget::randomNum(int min_num, int max_num) {
    return rand() % (max_num - min_num) + min_num;
}

void widget::setRandomFruit() {
    int count = 0;
    Node *p = node_front;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    updateVisitedMatrix();
    srand(unsigned(time(0)));
    int ranNum = randomNum(0, max_width * max_height - count);
    int temp_x = 0, temp_y = 0;
    if(ranNum == 0) {
        while (visited_matrix[temp_y * max_width + temp_x]) {
            if (temp_x < max_width - 1)
                temp_x++;
            else {
                temp_x = 0;
                temp_y++;
            }
        }
    }
    while (ranNum > 0) {
        do {
            if (temp_x < max_width - 1)
                temp_x++;
            else {
                temp_x = 0;
                temp_y++;
            }
        } while (visited_matrix[temp_y * max_width + temp_x]);
        ranNum--;
    }
    fruit_x = temp_x;
    fruit_y = temp_y;
}

inline bool widget::judgeDir(int x, int y, direction &next_dir,
                                  direction d) {
    if (x < 0 || x >= max_width || y < 0 || y >= max_height)
        return false;
    if (!visited_matrix[y * max_width + x] ||
        (x == node_rear->x && y == node_rear->y)) {
        switch (d) {
        case Left:
            if (dir == Right)
                return false;
            break;
        case Right:
            if (dir == Left)
                return false;
            break;
        case Up:
            if (dir == Down)
                return false;
            break;
        case Down:
            if (dir == Up)
                return false;
            break;
        }

        if (node_rear->prev == NULL) {
            next_dir = d;
            return true;
        }
        //如果走这一步能看到尾巴
        if (!isEat(d)) {
            if (findPathBFS(x, y, node_rear->prev->x, node_rear->prev->y,
                            true)) {
                next_dir = d;
                return true;
            }
        } else {
            if (findPathBFS(x, y, node_rear->x, node_rear->y, true)) {
                next_dir = d;
                return true;
            }
        }
    }
    return false;
}

int widget::dist(int start_x, int start_y, int end_x, int end_y) {
    return abs(end_x - start_x) + abs(end_y - start_y);
}

void widget::findPath(direction &next_dir, int start_x, int start_y, int end_x,
                      int end_y) {
    srand(unsigned(time(0)));
    vector<vector<int>> offSet({{-1, 0}, {1, 0}, {0, -1}, {0, 1}});
    vector<pair<direction, int>> vecDir;
    for (int i = Left; i <= Down; i++) {
        pair<direction, int> t(
            direction(i),
            dist(start_x + offSet[i][0], start_y + offSet[i][1], end_x, end_y));
        vecDir.push_back(t);
    }
    sort(vecDir.begin(), vecDir.end(),
         [](const pair<direction, int> &lhs, const pair<direction, int> &rhs) {
             return lhs.second < rhs.second;
         });
    if (findPathBFS(start_x, start_y, end_x, end_y, false)) {
        if (vecDir[1].second == vecDir[2].second &&
            vecDir[1].second == vecDir[3].second) {
            srand(unsigned(time(0)));
            for (int i = 3; i >= 2; i--) {
                int t = rand() % i + 1;
                swap(vecDir[t], vecDir[i]);
            }
            if (judgeDir(start_x + offSet[int(vecDir[0].first)][0],
                         start_y + offSet[int(vecDir[0].first)][1], next_dir,
                         vecDir[0].first))
                return;
            if (judgeDir(start_x + offSet[int(vecDir[1].first)][0],
                         start_y + offSet[int(vecDir[1].first)][1], next_dir,
                         vecDir[1].first))
                return;
            if (judgeDir(start_x + offSet[int(vecDir[2].first)][0],
                         start_y + offSet[int(vecDir[2].first)][1], next_dir,
                         vecDir[2].first))
                return;
            if (judgeDir(start_x + offSet[int(vecDir[3].first)][0],
                         start_y + offSet[int(vecDir[3].first)][1], next_dir,
                         vecDir[3].first))
                return;
        } else {
            int t = rand() % 2;
            if (t == 0) {
                if (judgeDir(start_x + offSet[int(vecDir[0].first)][0],
                             start_y + offSet[int(vecDir[0].first)][1],
                             next_dir, vecDir[0].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[1].first)][0],
                             start_y + offSet[int(vecDir[1].first)][1],
                             next_dir, vecDir[1].first))
                    return;
            } else {
                if (judgeDir(start_x + offSet[int(vecDir[1].first)][0],
                             start_y + offSet[int(vecDir[1].first)][1],
                             next_dir, vecDir[1].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[0].first)][0],
                             start_y + offSet[int(vecDir[0].first)][1],
                             next_dir, vecDir[0].first))
                    return;
            }
            t = rand() % 2;

            if (t == 0) {
                if (judgeDir(start_x + offSet[int(vecDir[2].first)][0],
                             start_y + offSet[int(vecDir[2].first)][1],
                             next_dir, vecDir[2].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[3].first)][0],
                             start_y + offSet[int(vecDir[3].first)][1],
                             next_dir, vecDir[3].first))
                    return;
            } else {
                if (judgeDir(start_x + offSet[int(vecDir[3].first)][0],
                             start_y + offSet[int(vecDir[3].first)][1],
                             next_dir, vecDir[3].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[2].first)][0],
                             start_y + offSet[int(vecDir[2].first)][1],
                             next_dir, vecDir[2].first))
                    return;
            }
        }
    } else {
        if (vecDir[1].second == vecDir[2].second &&
            vecDir[1].second == vecDir[3].second) {
            srand(unsigned(time(0)));
            for (int i = 3; i >= 2; i--) {
                int t = rand() % i + 1;
                swap(vecDir[t], vecDir[i]);
            }
            if (judgeDir(start_x + offSet[int(vecDir[1].first)][0],
                         start_y + offSet[int(vecDir[1].first)][1], next_dir,
                         vecDir[1].first))
                return;
            if (judgeDir(start_x + offSet[int(vecDir[2].first)][0],
                         start_y + offSet[int(vecDir[2].first)][1], next_dir,
                         vecDir[2].first))
                return;
            if (judgeDir(start_x + offSet[int(vecDir[3].first)][0],
                         start_y + offSet[int(vecDir[3].first)][1], next_dir,
                         vecDir[3].first))
                return;
            if (judgeDir(start_x + offSet[int(vecDir[0].first)][0],
                         start_y + offSet[int(vecDir[0].first)][1], next_dir,
                         vecDir[0].first))
                return;
        } else {
            int t = rand() % 2;
            if (t == 0) {
                if (judgeDir(start_x + offSet[int(vecDir[2].first)][0],
                             start_y + offSet[int(vecDir[2].first)][1],
                             next_dir, vecDir[2].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[3].first)][0],
                             start_y + offSet[int(vecDir[3].first)][1],
                             next_dir, vecDir[3].first))
                    return;
            } else {
                if (judgeDir(start_x + offSet[int(vecDir[3].first)][0],
                             start_y + offSet[int(vecDir[3].first)][1],
                             next_dir, vecDir[3].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[2].first)][0],
                             start_y + offSet[int(vecDir[2].first)][1],
                             next_dir, vecDir[2].first))
                    return;
            }
            t = rand() % 2;
            if (t == 0) {
                if (judgeDir(start_x + offSet[int(vecDir[0].first)][0],
                             start_y + offSet[int(vecDir[0].first)][1],
                             next_dir, vecDir[0].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[1].first)][0],
                             start_y + offSet[int(vecDir[1].first)][1],
                             next_dir, vecDir[1].first))
                    return;
            } else {
                if (judgeDir(start_x + offSet[int(vecDir[1].first)][0],
                             start_y + offSet[int(vecDir[1].first)][1],
                             next_dir, vecDir[1].first))
                    return;
                if (judgeDir(start_x + offSet[int(vecDir[0].first)][0],
                             start_y + offSet[int(vecDir[0].first)][1],
                             next_dir, vecDir[0].first))
                    return;
            }
        }
    }
}

bool widget::findPathBFS(int start_x, int start_y, int end_x, int end_y,
                              bool flag) {
    if (start_x < 0 || start_x >= max_width || start_y < 0 ||
        start_y >= max_height)
        return false;
    assume_visited_matrix = vector<bool>(visited_matrix);
    assume_visited_matrix[end_y * max_width + end_x] = false;

    if (flag) {
        assume_visited_matrix[fruit_y * max_width + fruit_x] = false;
        assume_visited_matrix[node_rear->y * max_width + node_rear->x] = false;
    }
    Point p(start_x, start_y);
    queue<Point> queueBFS;
    queueBFS.push(p);

    assume_visited_matrix[start_y * max_width + start_x] = true;

    while (!queueBFS.empty()) {
        Point p = queueBFS.front();
        int temp_x = p.x;
        int temp_y = p.y;
        queueBFS.pop();

        if (temp_x == end_x && temp_y == end_y) {
            return true;
        }
        // Right
        Point p1(temp_x + 1, temp_y);
        if (p1.x >= 0 && p1.x < max_width && p1.y >= 0 && p1.y < max_height) {
            if (!assume_visited_matrix[p1.y * max_width + p1.x]) {
                queueBFS.push(p1);
                assume_visited_matrix[p1.y * max_width + p1.x] = true;
            }
        }
        // Left
        Point p2(temp_x - 1, temp_y);
        if (p2.x >= 0 && p2.x < max_width && p2.y >= 0 && p2.y < max_height) {
            if (!assume_visited_matrix[p2.y * max_width + p2.x]) {
                queueBFS.push(p2);
                assume_visited_matrix[p2.y * max_width + p2.x] = true;
            }
        }
        // Up
        Point p3(temp_x, temp_y - 1);
        if (p3.x >= 0 && p3.x < max_width && p3.y >= 0 && p3.y < max_height) {
            if (!assume_visited_matrix[p3.y * max_width + p3.x]) {
                queueBFS.push(p3);
                assume_visited_matrix[p3.y * max_width + p3.x] = true;
            }
        }
        // Down
        Point p4(temp_x, temp_y + 1);
        if (p4.x >= 0 && p4.x < max_width && p4.y >= 0 && p4.y < max_height) {
            if (!assume_visited_matrix[p4.y * max_width + p4.x]) {
                queueBFS.push(p4);
                assume_visited_matrix[p4.y * max_width + p4.x] = true;
            }
        }
    }
    return false;
}


void widget::updateShowMatrix() {
    // 0:null  1:left&right   2:up&down    3:corner   4:front    5:rear 6:fruit
    for (int i = 0; i < max_width * max_height; i++)
        showMatrix[i] = 0;
    Node *p = node_front;
    showMatrix[p->y * max_width + p->x] = 4;
    vector<vector<direction>> cornerDir(max_width * max_height,
                                        vector<direction>({Left, Up}));
    p = p->next;
    while (p != NULL) {
        if (p->prev) {
            if (p->prev->x == p->x) {
                if (p->prev->y == p->y - 1)
                    cornerDir[p->y * max_width + p->x][0] = Up;
                else
                    cornerDir[p->y * max_width + p->x][0] = Down;
            } else {
                if (p->prev->x == p->x - 1)
                    cornerDir[p->y * max_width + p->x][0] = Left;
                else
                    cornerDir[p->y * max_width + p->x][0] = Right;
            }
        }
        if (p->next) {
            if (p->next->x == p->x) {
                if (p->next->y == p->y - 1)
                    cornerDir[p->y * max_width + p->x][1] = Up;
                else
                    cornerDir[p->y * max_width + p->x][1] = Down;
            } else {
                if (p->next->x == p->x - 1)
                    cornerDir[p->y * max_width + p->x][1] = Left;
                else
                    cornerDir[p->y * max_width + p->x][1] = Right;
            }
        }
        if (p->next != NULL) {
            if (p->prev->x == p->x && p->x == p->next->x)
                showMatrix[p->y * max_width + p->x] = 2;
            else if (p->prev->y == p->y && p->y == p->next->y)
                showMatrix[p->y * max_width + p->x] = 1;
            else {
                showMatrix[p->y * max_width + p->x] = 3;
            }
        } else {
            showMatrix[p->y * max_width + p->x] = 5;
        }
        p = p->next;
    }
    if(!isWin)
        showMatrix[fruit_y * max_width + fruit_x] = 6;

    float len = gridSize * 0.7;
    const QString m_red_SheetStyle =
        "min-width: 32px; min-height: 32px;max-width:32px; max-height: "
        "32px;border-radius: 16px;  border:1px solid black;background:red";
    for (int i = 0; i < max_height; i++) {
        for (int j = 0; j < max_width; j++) {
            if (showMatrix[i * max_width + j] == 3) {
                cornerMatrix[j][i][0]->show();
                cornerMatrix[j][i][1]->show();
            } else {
                cornerMatrix[j][i][0]->hide();
                cornerMatrix[j][i][1]->hide();
            }

            switch (showMatrix[i * max_width + j]) {
            case 0:
                matrix[j][i]->hide();
                break;
            case 1:
                matrix[j][i]->show();
                matrix[j][i]->setStyleSheet("background:blue");
                matrix[j][i]->setGeometry(j * gridSize,
                                          i * gridSize + (gridSize - len) / 2,
                                          gridSize, len);
                break;
            case 2:
                matrix[j][i]->show();
                matrix[j][i]->setStyleSheet("background:blue");
                matrix[j][i]->setGeometry(j * gridSize + (gridSize - len) / 2,
                                          i * gridSize, len, gridSize);
                break;
            case 3:
                matrix[j][i]->show();
                matrix[j][i]->setStyleSheet("background:blue");
                matrix[j][i]->setGeometry(j * gridSize + (gridSize - len) / 2,
                                          i * gridSize + (gridSize - len) / 2,
                                          len, len);
                cornerMatrix[j][i][0]->setStyleSheet("background:blue");
                cornerMatrix[j][i][1]->setStyleSheet("background:blue");
                for (int k = 0; k < 2; k++) {
                    switch (cornerDir[i * max_width + j][k]) {
                    case Left:
                        cornerMatrix[j][i][k]->setGeometry(
                            j * gridSize, i * gridSize + (gridSize - len) / 2,
                            gridSize / 2, len);
                        break;
                    case Right:
                        cornerMatrix[j][i][k]->setGeometry(
                            (j + 0.5) * gridSize,
                            i * gridSize + (gridSize - len) / 2, gridSize / 2,
                            len);

                        break;
                    case Up:
                        cornerMatrix[j][i][k]->setGeometry(
                            j * gridSize + (gridSize - len) / 2, i * gridSize,
                            len, gridSize / 2);
                        break;
                    case Down:
                        cornerMatrix[j][i][k]->setGeometry(
                            j * gridSize + (gridSize - len) / 2,
                            (i + 0.5) * gridSize, len, gridSize / 2);
                        break;
                    }
                }
                break;
            case 4:
                matrix[j][i]->show();
                matrix[j][i]->setStyleSheet("background:green");
                matrix[j][i]->setGeometry(j * gridSize + (gridSize - len) / 2,
                                          i * gridSize + (gridSize - len) / 2,
                                          len, len);
                break;

            case 5:
                matrix[j][i]->show();
                matrix[j][i]->setStyleSheet("background:orange");
                matrix[j][i]->setGeometry(j * gridSize + (gridSize - len) / 2,
                                          i * gridSize + (gridSize - len) / 2,
                                          len, len);
                break;
            case 6:
                matrix[j][i]->show();
                matrix[j][i]->setStyleSheet(m_red_SheetStyle);
                matrix[j][i]->setGeometry((j + 0.5) * gridSize - 16,
                                          (i + 0.5) * gridSize - 16, gridSize,
                                          gridSize);
                break;
            }
        }
    }
}
