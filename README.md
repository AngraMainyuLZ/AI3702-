# AI3702-Mobile Robotic
SJTU移动机器人Final仿真

## 直接使用
将src放入自己VMware中{task}_ws文件夹中，catkin_make 编译即可。

如果仿真时发现，设定目标点后，小车没反应，请 $\text{\textcolor{yellow}{\textbf{重试一下}}}$。
## 一些代码的解释
1. 在`astar/src/main.cpp`的`MapCallBack`函数中最后几行是关于地图膨胀的代码，膨胀半径(inflation_radius)可以修改。目前是5.
2. Astar算法做了一些改进，在膨胀图中，障碍膨胀出来的区域将被设为128，这样的点的路径代价（gcost）被设为了一个很大的数，保证不会卡死的情况下尽量不走膨胀区。


