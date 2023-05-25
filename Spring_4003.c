#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100000
long long ans = 0;

void reversePairs(int *arr, int begin, int end) 
{
    int mid;
    if (begin < end)        //分治递归计算逆序对
    {
        mid = (begin + end) / 2;
        reversePairs(arr, begin, mid);  //左子序内部的逆序对数
        reversePairs(arr, mid + 1, end);//右子序内部的逆序对数
        //整个序列中左子序中一个数与右子序中一个数构成逆序对数
        ans += Reverse_Merge_Sort(arr, begin, mid + 1, end);
    }//归并排序
}

int Reverse_Merge_Sort(int* arr, int begin, int mid, int end)
{
    int assist[end - begin + 1]; //辅助数组
    int temp = 0, count = 0;
    int index = 0; //辅助数组的下标
    
    //归并排序的同时计算逆序对数
    int i, j;
    for (i = begin, j = mid; i < mid && j <= end;)
    {
        if (arr[i] > arr[j]) //若ai > aj则ai及其右侧所有元素均可与aj组成逆序对
        {
            assist[index++] = arr[j++];
            temp++;
        }
        if (arr[i] <= arr[j] || j == end + 1) //结算ai可组成的逆序对数
        {
            count += temp * (mid - i);
            assist[index++] = arr[i++];
            temp = 0; //将temp归零
        }
    }

    //将余下的元素插入辅助数组
    while (i < mid)
        assist[index++] = arr[i++];
    while (j <= end)
        assist[index++] = arr[j++];

    //将辅助数组拷贝回原数组
    int flag = begin;
    for (index = 0; flag <= end;)
        arr[flag++] = assist[index++];

    return count;
}

int main() {
    int n;
    int arr[MAXSIZE];

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    reversePairs(arr, 0, n - 1);
    printf("%lld\n", ans);
    return 0;
}