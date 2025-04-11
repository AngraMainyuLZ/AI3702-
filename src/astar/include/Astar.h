#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <nav_msgs/OccupancyGrid.h>
#include <tf/tf.h>
#include <algorithm>
#include <queue>

using namespace std;
using namespace cv;

const int Dist = 5;  // 横向距离代价
const int DistX = 7; // 斜向

struct MapParamNode {
    Point StartPoint, TargetPoint;
    Mat Rotation;
    Mat Translation;
    double resolution;
    int height;
    int width;
    int x;
    int y;
};

struct node {
    int x, y;
    int g; // cost from start to node
    int h; // cost from node to target
    int f;
    node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h), f(g + h) {}

    bool operator>(const node &other) const {
        return f > other.f;
    }
};

int hScore(int x, int y, Point t) {
    return Dist * (abs(x - t.x) + abs(y - t.y));
}

int hScore(Point s, Point t) {
    return Dist * (abs(s.x - t.x) + abs(s.y - t.y));
}

vector<Point> Astar(Mat &Maptest, MapParamNode &MapParam) {
    cout<<"Start"<<endl;
    const int rows = MapParam.height; // 地图的行数
    const int cols = MapParam.width;  // 地图的列数

    vector<vector<int>> fMat(rows, vector<int>(cols, INT_MAX)); // 代价矩阵
    vector<vector<Point>> Parent(rows, vector<Point>(cols, Point(-1, -1))); // 父节点
    priority_queue<node, vector<node>, greater<node>> openList;

    Point start = MapParam.StartPoint;
    Point target = MapParam.TargetPoint;

    // 起点和终点边界检查
    if (start.x < 0 || start.x >= cols || start.y < 0 || start.y >= rows ||
        target.x < 0 || target.x >= cols || target.y < 0 || target.y >= rows) {
        ROS_ERROR("Start or target point is out of map bounds!");
        return vector<Point>();
    }

    // 初始化起点
    fMat[start.y][start.x] = hScore(start, target); // 使用 (row, col) 顺序
    openList.push(node(start.x, start.y, 0, hScore(start, target)));

    while (!openList.empty()) {
        node current = openList.top();
        openList.pop();
        // 到达目标点
        if (current.x == target.x && current.y == target.y) {
            vector<Point> path;
            Point current_point(current.x, current.y);
            while (current_point != start) {
                path.push_back(current_point);
                current_point = Parent[current_point.y][current_point.x]; // 使用 (row, col) 顺序
            }
            path.push_back(start);
            cout<<"path size: "<< path.size()<<endl;
            reverse(path.begin(), path.end());
            return path;
        }

        // 遍历八个方向
        const int dx[] = {1, -1, 0, 0, 1, 1, -1, -1}; 
        const int dy[] = {0, 0, 1, -1, 1, -1, 1, -1}; 

        for (int i = 0; i < 8; ++i) {
            int nx = current.x + dx[i]; // 新列
            int ny = current.y + dy[i]; // 新行
            int cost = (i < 4) ? Dist : DistX;
            // 边界检查
            if (nx < 0 || nx >= cols || ny < 0 || ny >= rows)
                {cout<<"out of bundary";
                continue;}

            // 障碍物检查

            
        
            if (Maptest.at<uchar>(nx, ny) == 0 ) {
                continue;
            }

            // 计算新节点代价
            node dnew(nx, ny, current.g + cost, hScore(nx, ny, target));

            // 更新代价矩阵和父节点
            if (dnew.f < fMat[ny][nx]) { 
                Parent[ny][nx] = Point(current.x, current.y); // 使用 (row, col) 顺序
                fMat[ny][nx] = dnew.f;
                openList.push(dnew);
            }
        }
    }

    return vector<Point>(); // 未找到路径
}

#endif
