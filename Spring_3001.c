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
void nodeDegree(Graph g, int* node_degree);
double clusteringCoefficient(Graph g);


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
 * 计算每个点的度
 * @param g 图
 * @param node_degree 将每个点的度写到这个数组中
 */
void nodeDegree(Graph g, int *node_degree) 
{
    int temp = 0;
    for (int i = 0; i < g.N; i++)
    {
        temp = 0;
        for (int j = 0; j < g.N; j++)
        {
            if (g.matrix[i][j] != max_dis && g.matrix[i][j])
            temp++;
        }
        node_degree[i] = temp;
    }
}

/**
 * 计算图的聚类系数
 * @param g 图
 * @return 返回聚类系数
 */
double clusteringCoefficient(Graph g) 
{
    double sum = 0;         //总聚类系数
    int vex[10];            //与某点邻接的所有点
    int degree;             //邻接点的个数
    int count;              //邻接点之间边的个数
    int max;                //邻接点之间可连边总数
    
    for (int i = 0; i < g.N; i++)       //对每个点求取聚类系数
    {
        degree = count = max = 0;       //归零
        for (int j = 0; j < g.N; j++)   //找到所有邻接点
        {
            if (g.matrix[i][j] != max_dis && g.matrix[i][j])
            {
                vex[degree++] = j;
            }
        }
        
        if (degree <= 1)    continue;   //度数小于等于1则聚类系数为0
        for (int j = 0; j < degree - 1; j++)    //遍历所有邻接点
        {
            for (int k = j + 1; k < degree; k++)
            {
                if (g.matrix[vex[j]][vex[k]] != max_dis && g.matrix[vex[j]][vex[k]])
                count++;
            }
        }

        for (int j = 1; j < degree; j++)        //计算可连边的总数
        {
            max += j;
        }
        
        sum += (double)count / max;
    }
    return (sum / g.N);    
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

    int *degree = (int *)malloc(sizeof(int) * g.N);
    nodeDegree(g, degree);
    printf("degree distribution:\n");
    for(int i=0; i<g.N; i++)
    {
        printf("node%s:%d,", g.vertex[i], degree[i]);
    }
    printf("\n");
    double c = clusteringCoefficient(g);
    printf("clustering coefficient:%f\n", c);

    return 0;
}