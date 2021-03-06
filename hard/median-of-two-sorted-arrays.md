## 4. Median of Two Sorted Arrays
##### 2018年6月23日
##### 分治？ 查找中位数
***
## 题目
There are two sorted arrays nums1 and nums2 of size m and n respectively.

Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).

Example 1:
```
nums1 = [1, 3]
nums2 = [2]

The median is 2.0
```
Example 2:
```
nums1 = [1, 2]
nums2 = [3, 4]

The median is (2 + 3)/2 = 2.5
```
## 分析
既然学了c++，就采用C++的方式去解决。新建一个vecotr，将已有的数据全部填入，之后排序，计算中位数的位置。

如果不加小数点，a/2的值为整形，默认向下取整。（当然强转也可以避免这个问题）;
insert()函数：
>- single element (1)
iterator insert (const_iterator position, const value_type& val);
>- fill (2)
iterator insert (const_iterator position, size_type n, const value_type& val);
>- range (3)
template ``<class InputIterator>``
iterator insert (const_iterator position, InputIterator first, InputIterator last);
>- move (4)
iterator insert (const_iterator position, value_type&& val);
>- initializer list (5)
iterator insert (const_iterator position, initializer_list<value_type> il);
## 示例代码
```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> a;
        a.insert(a.end(),nums1.begin(),nums1.end());
        a.insert(a.end(),nums2.begin(),nums2.end());
        sort(a.begin(),a.end(),greater<int>());
        int len=a.size();
        double sum=0;
        if(len%2){
            return a[len/2];
        }
        else{
            if(len ==2 )
                sum = (a[0]+a[1])/2.0;  //不强转需要加小数点
            else
                sum=(a[len/2]+a[len/2-1])/2.0;
        }
        return sum;
    }
};
```
****
完全符合要求的代码：
```cpp
double findKth(vector<int>& nums1, int m, int start1, vector<int> &nusm2, int n, int start2, int k){
	if (m >n)
		return findKth(b, n, start2, a, m, start1, k);  //make m is equal or smaller than n
	if (m == 0)
		return nums2[k - 1];
	if (k == 1)
		return min(nums1[0], nums2[0]);


	int pa = min(k/2, m), pb = k - pa;
	if (nums1[star1 + pa - 1] < nums2[start2 + pb - 1])
		return findKth(nums1, m-pa, start1+pa, nums2, n, start2, k - pa);
	else if (nums1[start1 + pa - 1] > nums2[start2 + pb - 1])
		return findKth(nums1, m, start1, nums2, n-pb, start2+pb, k - pb);
	else
		return a[start1 + pa - 1];
}

class Solution
{
public:
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2){
		int total = nums1.size() + nums2.size();
		if (total & 0x1)  //Judging parity
			return findKth(nums1, nums1.size(), nums2, nums2.size(), total / 2 + 1);
		else
			return (findKth(nums1, nums1.size(), nums2, nums2.size(), total / 2)
					+ findKth(nums1, nums1.size(), nums2, nums2.size(), total / 2 + 1)) / 2;
	}
};
```
