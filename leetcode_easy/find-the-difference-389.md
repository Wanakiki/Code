# 389. Find the Difference

2018年8月14日

## 题目

Given two strings s and t which consist of only lowercase letters.

String t is generated by random shuffling string s and then add one more letter at a random position.

Find the letter that was added in t.

Example:

```no
Input:
s = "abcd"
t = "abcde"

Output:
e

Explanation:
'e' is the letter that was added.
```

## 分析

因为第二个字符串是原有字符串打乱顺序添加一个字符得到的，所以对两个字符串进行排序，接着遍历两个字符串，不相同的元素就是添加的字符。

另外，根据与运算的性质，也可以对两个数组所有元素进行与运算。

## 示例代码

C++

```cpp
class Solution {
public:
    static char findTheDifference(const string& s, const string& t) {
        char addedChar = 0;
        for (auto ch : s) {
            addedChar ^= ch;
        }
        
        for (auto ch : t) {
            addedChar ^= ch;
        }
        
        return addedChar;
    }
};
```

```cpp
class Solution {
public:
    char findTheDifference(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        int len = t.size();
        for(int i = 0; i < len - 1; i++){
            if(s[i] != t[i])
                return t[i];
        }
        return t[len-1];
    }
};
```