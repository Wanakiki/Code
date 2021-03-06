## 836. 矩形重叠

2020年3月18日

### 题目

矩形以列表 [x1, y1, x2, y2] 的形式表示，其中 (x1, y1) 为左下角的坐标，(x2, y2) 是右上角的坐标。

如果相交的面积为正，则称两矩形重叠。需要明确的是，只在角或边接触的两个矩形不构成重叠。

给出两个矩形，判断它们是否重叠并返回结果。

示例 1：
```cpp
输入：rec1 = [0,0,2,2], rec2 = [1,1,3,3]
输出：true
```

示例 2：
```cpp
输入：rec1 = [0,0,1,1], rec2 = [1,0,2,1]
输出：false
```
说明：

1. 两个矩形 rec1 和 rec2 都以含有四个整数的列表的形式给出。
2. 矩形中的所有坐标都处于 -10^9 和 10^9 之间。

来源：力扣（LeetCode）  
链接：https://leetcode-cn.com/problems/rectangle-overlap

### 代码

将矩形的边投影到x轴和y轴，对应四条线段，再判断同一坐标轴上的线段是否重叠，若两个坐标轴均重叠，则证明两个矩形重叠。

判断线段是否重叠的做法是判断左端点的最大值是否小于右端点的最小值。

```cpp
class Solution {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        return (max(rec1[0], rec2[0]) < min(rec1[2], rec2[2])) && (max(rec1[1], rec2[1]) < min(rec1[3], rec2[3])); 
    }
};
```