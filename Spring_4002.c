#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct student{
	int Code;
	int Score[5];
} Student;

typedef struct rank{
	int Rank;
	int Code;
}Rank;

Student Students[50];
Rank 	Ranks[50];

int 	Priority[5];
double 	Weight[5];
int 	N;

typedef struct Total
{
    double score;
    Student* stu;
    int even;
}Total;

Total Totals[50];

void CalcAndWriteRanks()
{
	//初始化
    for (int i = 0; i < N; i++)
    {
        Totals[i].score = 0;
        Totals[i].stu = NULL;
        Totals[i].even = 0;
    }
    
    //计算总分
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            Totals[i].score += Students[i].Score[j] * Weight[j];
        }
        Totals[i].stu = &Students[i];
    }

    //按总分字典归并排序为降序
    MergeSort(Totals, N);

    //对同分学生进行第二轮排序
    EvenRank(Totals, N);

    //将排序结果写入Rank中
    for (int i = 0; i < N; i++)
    {
        Ranks[i].Rank = i + 1;
        Ranks[i].Code = Totals[i].stu->Code;
        
        if (Totals[i].even)//所有分数均相同的情况
            Ranks[i].Rank = Ranks[i - 1].Rank;
    }

    int swap_time;      //产生交换的次数
    do
    {
        swap_time = 0;
        for (int i = 0; i < N - 1; i++)
        {
            if (Ranks[i].Rank == Ranks[i + 1].Rank)
            {
                if (Ranks[i].Code > Ranks[i + 1].Code)
                {
                    int temp = Ranks[i + 1].Code;
                    Ranks[i + 1].Code = Ranks[i].Code;
                    Ranks[i].Code = temp;
                    swap_time++;
                }//按学号升序整理结果
            }
        }
    } while (swap_time);
}

//归并排序
void MergeSort(Total* Totals, int N)
{
    Total* temp = (Total*)calloc(sizeof(Total), N);
    Merge(Totals, 0, N - 1, temp);
    free(temp);
}

//归并排序的递归算法
void Merge(Total* Totals, int left, int right, Total* temp)
{
    if (left >= right)  return;             //如果区间不存在或只有一个数

    int mid = (left + right) / 2;
    //开始递归
    Merge(Totals, left, mid, temp);         //左区间
    Merge(Totals, mid + 1, right, temp);    //右区间

    //归并算法
    int begin1 = left, end1 = mid;
    int begin2 = mid + 1, end2 = right;
    //从left位置插入临时数组
    int flag = left;

    while (begin1 <= end1 && begin2 <= end2)
    {
        if (Totals[begin1].score > Totals[begin2].score)
        {
            temp[flag].score = Totals[begin1].score;
            temp[flag].stu = Totals[begin1].stu;
            flag++;
            begin1++;
        }
        else
        {
            temp[flag].score = Totals[begin2].score;
            temp[flag].stu = Totals[begin2].stu;
            flag++;
            begin2++;
        }
    }

    //将未遍历完的区间按序插入临时数组
    while (begin1 <= end1)
    {
        temp[flag].score = Totals[begin1].score;
        temp[flag].stu = Totals[begin1].stu;
        flag++;
        begin1++;
    }
    while (begin2 <= end2)
    {
        temp[flag].score = Totals[begin2].score;
        temp[flag].stu = Totals[begin2].stu;
        flag++;
        begin2++;
    }

    //将临时数组复制回原数组
    for (int i = left; i <= right; i++)
    {
        Totals[i].score = temp[i].score;
        Totals[i].stu = temp[i].stu;
    }
}

//对同分学生进行第二轮排序
void EvenRank(Total* Totals, int N)
{
    Total temp;
    int swap_time;      //产生交换的次数
    do
    {
        swap_time = 0;
        for (int i = 0; i < N - 1; i++)
        {
            if (fabs(Totals[i].score - Totals[i + 1].score) < 0.001)
            {
                for (int j = 1; j <= 5; j++)
                {
                    int k;
                    for (k = 0; k < 5; k++)
                    {
                        if (Priority[k] == j)
                            break;
                    }//查找对应优先级的科目

                    if (Totals[i].stu->Score[k] < Totals[i + 1].stu->Score[k])
                    {
                        temp.score = Totals[i].score;
                        temp.stu = Totals[i].stu;
                        
                        Totals[i].score = Totals[i + 1].score;
                        Totals[i].stu = Totals[i + 1].stu;
                        
                        Totals[i + 1].score = temp.score;
                        Totals[i + 1].stu = temp.stu;

                        swap_time++;
                        
                        int ii = i;
                        while (Totals[ii].even) //如果交换前该同学与前一个同学同分
                        {
                            temp.score = Totals[ii].score;
                            temp.stu = Totals[ii].stu;
                        
                            Totals[ii].score = Totals[ii - 1].score;
                            Totals[ii].stu = Totals[ii - 1].stu;
                        
                            Totals[ii - 1].score = temp.score;
                            Totals[ii - 1].stu = temp.stu;

                            Totals[ii].even = 0;
                            Totals[ii + 1].even = 1;

                            swap_time++;
                            ii--;
                        }
                        goto endcompare;
                    }//如果排在前面的同学分数分数更低则交换

                    else if (Totals[i].stu->Score[k] > Totals[i + 1].stu->Score[k])
                        goto endcompare;
                }//按优先级比较两学生对应科目分数
                Totals[i + 1].even = 1;     //指示两人所有分数均相同
            }//如果两学生总分相同
            endcompare:;
        }
    } while (swap_time);
}

void PrintRanks(){
	for (int loop = 0; loop < N; loop++){
		printf("%d %d\n", Ranks[loop].Rank, Ranks[loop].Code);
	}
}

int main(int argc, char *argv[]) {
	// Input part, do not modify
	scanf("%d", &N);
	for (int loop = 0; loop < 5; loop++){
		scanf("%d", Priority + loop);
	}
	for (int loop = 0; loop < 5; loop++){
		scanf("%lf", Weight + loop);
	}
	
	for (int loop = 0; loop < N; loop++){
		scanf("%d", &Students[loop].Code);
		for (int j = 0; j < 5; j++){
			scanf("%d", Students[loop].Score + j);
		}
	}
	// Main process part
	CalcAndWriteRanks();
	// Output part, do not modify
	PrintRanks();
	return 0;
}