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

int Kruskal(Graph g);

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

int Kruskal(Graph g)
{
    int sum = 0;        //最小生成树总权
    int count = g.N;    //连通分量数
    int* visited = (int*)calloc(g.N, sizeof(int));  //标记各点的连通分量
    for (int i = 0; i < g.N; i++)   visited[i] = i; //初始化

    int temp;
    int v1, v2;
    while (count > 1)
    {
        temp = max_dis;
        v1 = v2 = 0;
        for (int i = 0; i < g.N; i++)
        {
            for (int j = i + 1; j < g.N; j++)
            {
                if (g.matrix[i][j] < temp)
                {
                    temp = g.matrix[i][j];
                    v1 = i, v2 = j;
                }
            }
        }//遍历搜索最小边
        if (visited[v1] != visited[v2])     //如果两个点属于不同的连通分量
        {
            for (int i = 0; i < g.N; i++)
            {
                if (visited[i] == visited[v1] && i != v1)
                visited[i] = visited[v2];
            }
            visited[v1] = visited[v2];
            count--;
            sum += temp;
        }//合并两个连通分量
        g.matrix[v1][v2] = max_dis;
    }
    return sum;
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

    printf("%d", Kruskal(g));
    return 0;
}