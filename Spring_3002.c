#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define max_dis 100000

typedef char vextype[20];

typedef struct {
    int N, E;//N是顶点数，E是边数
    int** matrix;//储存邻接矩阵
    vextype* vertex;//存储节点的名字
} Graph;

Graph createGraph(int n);
int isConnected(Graph g);
int Diameter(Graph g);
int Radius(Graph g);
int dijkstra(Graph g, int start, int end, int* path);
void printPath(int d, int* diameter_path, Graph g);

void DFS(Graph g, int i, int* visited);
int DFSTraverse(Graph g);
void floyd(Graph g, int** dist, int** path);


/**
 * 创建一个节点数为n的图
 * @param n 节点个数
 * @return 返回这个图
 */
Graph createGraph(int n) {
    int i, j;
    Graph g;
    g.N = n;
    g.matrix = (int**)malloc(sizeof(int*) * g.N);
    for (i = 0; i < n; i++) {
        g.matrix[i] = (int*)malloc(sizeof(int) * g.N);
    }
    for (i = 0; i < g.N; i++) {
        for (j = 0; j < g.N; j++) {
            g.matrix[i][j] = max_dis;
        }
    }
    for (i = 0; i < g.N; i++) {
        g.matrix[i][i] = 0;
    }
    g.vertex = (vextype*)malloc(sizeof(vextype) * g.N);
    return g;
}


/**
 * 根据距离d和路径数组path输出路径，这样就不需要路径的节点数也能正确输出路径
 * @param d 路径长度
 * @param diameter_path 储存路径的数组
 * @param g 图
 */
void printPath(int d, int *diameter_path, Graph g)
{
    int k = 0;
    int path_length = 0;
    printf("Path: ");
    do {
        printf("%s->", g.vertex[diameter_path[k]]);
        path_length += g.matrix[diameter_path[k]][diameter_path[k + 1]];
        k++;
    } while (path_length < d);
    printf("%s\n", g.vertex[diameter_path[k]]);
}

/**
 * 判断图是否连通
 * @param g 图
 * @return 连通返回1，否则返回0
 */
int isConnected(Graph g) 
{
    int* connect = (int*)malloc(sizeof(int) * g.N); //指示是否与起点位于同一连通分支的数组
    for (int i = 0; i < g.N; i++)   connect[i] = 0; //初始化
    
    DFS(g, 0, connect);

    int flag = 1;
    for (int i = 0; i < g.N; i++)               //判断是否所有点都在一个连通分支内
    {
        if (!connect[i])
        {
            flag = 0;
            break;
        }
    }
    free(connect);
    return flag;
}

void DFS(Graph g, int i, int* visited)
{
    visited[i] = 1;
    for (int j = 0; j < g.N; j++)
    {
        if (!visited[j] && g.matrix[i][j] != max_dis)
        DFS(g, j, visited);
    }
}

/**
 * 使用dijkstra算法计算单源最短路径
 * @param g 图
 * @param start 起点
 * @param end 终点
 * @param path 从start到end的路径, [start,...,end]
 * @return 路径长度
 */
int dijkstra(Graph g, int start, int end, int *path)
{
    int* weight = (int*)calloc(g.N, sizeof(int));   //长度
    int* set = (int*)calloc(g.N, sizeof(int));      //已完成的点的集合
    int* P = (int*)calloc(g.N, sizeof(int));        //路径

    for (int i = 0; i < g.N; i++)                   //初始化
    {
        weight[i] = g.matrix[start][i];
        set[i] = 0;
        P[i] = start;
    }
    set[start] = 1;

    int temp, flag, sum;
    for (int i = 1; i < g.N; i++)
    {
        temp = max_dis;
        flag = -1;
        for (int j = 0; j < g.N; j++)
        {
            if (weight[j] <= temp && weight[j] && !set[j])
            {
                temp = weight[j];
                flag = j;
            }
        }
        set[flag] = 1;

        for (int j = 0; j < g.N; j++)
        {
            if (set[j] != 1)
            {
                sum = weight[flag] + g.matrix[flag][j];
                if (sum < weight[j])
                {
                    weight[j] = sum;
                    P[j] = flag;
                }
            }
        }
    }

    int dummyend = end;         //保护原end的数据
    int stack[20], top = -1;    //路径栈
    stack[++top] = dummyend;         //开始入栈
    while (P[dummyend] != start)
    {
        stack[++top] = P[dummyend];
        dummyend = P[dummyend];
    }
    stack[++top] = start;       //结束入栈
    int i = 0;
    while (top >= 0)            //出栈
    {
        path[i++] = stack[top--];
    }

    free(set);
    free(weight);
    free(P);
    return weight[end];
}

/**
 * 计算图的直径。提示：Floyd算法
 * @param g 图
 * @return 直径的长度
 */
int Diameter(Graph g) 
{
    int** temp = (int**)calloc(g.N, sizeof(int*));
    for (int i = 0; i < g.N; i++)
    {
        temp[i] = (int*)calloc(g.N, sizeof(int));
    }
    
    for (int i = 0; i < g.N; i++)
    {
        for (int j = 0; j < g.N; j++)
        {
            temp[i][j] = g.matrix[i][j];
        }
    }

    for (int k = 0; k < g.N; k++)   //floyd算法
    {
        for (int i = 0; i < g.N; i++)
        {
            for (int j = 0; j < g.N; j++)
            {
                if (temp[i][k] + temp[k][j] < temp[i][j])
                temp[i][j] = temp[i][k] + temp[k][j];
            }
        }
    }

    int target = 0;
    for (int i = 0; i < g.N; i++)
    {
        for (int j = 0; j < g.N; j++)
        {
            if (temp[i][j] > target && temp[i][j] != max_dis)
            target = temp[i][j];
        }
    }
    free(temp);
    return target;
}


/**
 * 计算图的半径
 * @param g 图
 * @return 半径长度
 */
int Radius(Graph g) 
{
    int** temp = (int**)calloc(g.N, sizeof(int*));
    for (int i = 0; i < g.N; i++)
    {
        temp[i] = (int*)calloc(g.N, sizeof(int));
    }
    
    for (int i = 0; i < g.N; i++)
    {
        for (int j = 0; j < g.N; j++)
        {
            temp[i][j] = g.matrix[i][j];
        }
    }

    for (int k = 0; k < g.N; k++)   //floyd算法
    {
        for (int i = 0; i < g.N; i++)
        {
            for (int j = 0; j < g.N; j++)
            {
                if (temp[i][k] + temp[k][j] < temp[i][j])
                temp[i][j] = temp[i][k] + temp[k][j];
            }
        }
    }

    int* radiu = (int*)calloc(g.N, sizeof(int));
    int target = 0;
    for (int i = 0; i < g.N; i++)
    {
        target = 0;
        for (int j = 0; j < g.N; j++)
        {
            if (temp[i][j] > target && temp[i][j] != max_dis)
            target = temp[i][j];
        }
        radiu[i] = target;
    }
    target = max_dis;
    for (int i = 0; i < g.N; i++)
    {
        if (radiu[i] < target)  target = radiu[i];
    }
    
    free(temp);
    free(radiu);
    return target;
}


int main() {
    int node_num;
    int edge_num;


    scanf("%d %d", &node_num, &edge_num);

    Graph g = createGraph(node_num);
    for(int i = 0; i < node_num; i++) {
        sprintf(g.vertex[i], "%d", i);
    }
    for (int i = 0; i < edge_num; i++) {
        int start_idx, end_idx, weight;
        scanf("%d %d %d", &start_idx, &end_idx, &weight);
        g.matrix[start_idx][end_idx] = weight;
        g.matrix[end_idx][start_idx] = weight;
    }
    printf("connected: %d\n", isConnected(g));

    if (isConnected(g))
    {
        int* short_path = (int*)malloc(sizeof(int) * g.N);
        int dis = dijkstra(g, 1, 3, short_path);
        printf("the minimum fare between 1 and 3: %d\n", dis);
        printPath(dis, short_path, g);
        free(short_path);
        int d = Diameter(g);
        printf("diameter:%d\n", d);

        int r = Radius(g);
        printf("radius:%d\n", r);
    }

    return 0;
}