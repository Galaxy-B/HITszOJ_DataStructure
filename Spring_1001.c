#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{
    char StuID[11];
    int Grade;
    struct node *next;
}StudentLinkedListNode;

/* 创建交叉列表 */
void createCrossLink(StudentLinkedListNode* a, StudentLinkedListNode* b, int beforeCross1, int beforeCross2){
    // a和b后面若干结点值相同
    // beforeCross1为跳过的a中的个数，从第beforeCross1 + 1个结点开始相交
    // beforeCross2为跳过的b中的个数，从第beforeCross2 + 1个结点开始相交
    // 相交方法是将b中的前一结点指向a中的首个相交结点
    StudentLinkedListNode *p, *q;
    while(beforeCross1--)a=a->next;
    while(--beforeCross2)b=b->next;
    p = b->next;
    b->next = a;
    //销毁野指针结点
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
}

/* 程序结束后销毁交叉列表 */
void destroyCrossLink(StudentLinkedListNode* a, StudentLinkedListNode* b, StudentLinkedListNode* crossNode){
    StudentLinkedListNode* p = crossNode->next, *q;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
    while(a != crossNode){
        q = a->next;
        free(a);
        a = q;
    }
    while(b != crossNode){
        q = b->next;
        free(b);
        b = q;
    }
    free(crossNode);
}

/* 打印单个结点 */
void printLinkedListNode(StudentLinkedListNode* node)
{
    printf("{ID:%s, Grade:%d}", node->StuID, node->Grade);
    if(node->next!=NULL){
        printf("->");
    }else{
        printf("\n");
    }
}

/** 打印整个列表 **/
void outputStudentLinkedList(StudentLinkedListNode* head)
{
    while (head != NULL)
    {
        printLinkedListNode(head);
        head = head->next;
    }
}

/** 新建一个链表node并返回 **/
StudentLinkedListNode* createStudentLinkedList(char student_id[], int grade) 
{
    StudentLinkedListNode* p = (StudentLinkedListNode*)malloc(sizeof(StudentLinkedListNode));
    strcpy(p->StuID, student_id);
    p->Grade = grade;
    return p;
}

/** 按照降序插入学生的成绩情况,并返回链表头指针 **/
StudentLinkedListNode* addStudentLinkedList(StudentLinkedListNode* head, StudentLinkedListNode* node) 
{
    if (head != NULL)
    {
        node->next = head;
        head = node;
    }
    else if (head == NULL)
    {
        node->next = NULL;
        head = node;
    }
    return head;
}

/** 反转链表 **/
StudentLinkedListNode* reverseStudentLinkedList(StudentLinkedListNode*head)
{
    StudentLinkedListNode* p = NULL;
    while(head != NULL)
    {
        if (p != NULL)
        {
            StudentLinkedListNode* temp = (StudentLinkedListNode*)malloc(sizeof(StudentLinkedListNode));
            temp->Grade = head->Grade;
            strcpy(temp->StuID, head->StuID);
            temp->next = p;
            p = temp;
            head = head->next;
        }
        else if (p == NULL)
        {
            StudentLinkedListNode* temp = (StudentLinkedListNode*)malloc(sizeof(StudentLinkedListNode));
            temp->next = NULL;
            temp->Grade = head->Grade;
            strcpy(temp->StuID, head->StuID);
            p = temp;
            head = head->next;
        }
    }
    free(head);
    return p;
}

/** 找到相交的第一个结点 **/
StudentLinkedListNode* findCrossBeginNode(StudentLinkedListNode* class1, StudentLinkedListNode* class2) 
{
    int count1 = 0;
    int count2 = 0;
    StudentLinkedListNode* head1 = class1;
    StudentLinkedListNode* head2 = class2;
    while (head1 != NULL)
    {
        count1++;
        head1 = head1->next;
    }
    while (head2 != NULL)
    {
        count2++;
        head2 = head2->next;
    } 
    
    if (count1 > count2)
    {
        int gap = count1 - count2;
        for (int i = 1; i <= gap; i++)
        {
            class1 = class1->next;
        }  
    }
    else
    {
        int gap = count2 - count1;
        for (int i = 1; i <= gap; i++)
        {
            class2 = class2->next;
        }
    }

    while (class1 != NULL&& class2 != NULL)
    {
        if (class1 == class2)
        {
            return class1;
        }
        class1 = class1->next;
        class2 = class2->next;
    }
    return NULL;
}

int main()
{
    StudentLinkedListNode *class1=NULL, *class2=NULL;
    int num1, num2, i;
    char student_id[11];
    int grade;
    int beforeCross1, beforeCross2;
    StudentLinkedListNode *node;

    // 读取两个班级的人数
    scanf("%d%d", &num1, &num2);
    class1 = class2 = NULL;
    // 读取班级1的成绩
    for(i = 0;i < num1;i++){
        scanf("%s%d", student_id, &grade);
        node = createStudentLinkedList(student_id, grade);
        class1 = addStudentLinkedList(class1, node);
    }
    // 读取班级2的成绩
    for(i = 0;i < num2;i++){
        scanf("%s%d", student_id, &grade);
        node = createStudentLinkedList(student_id, grade);
        class2 = addStudentLinkedList(class2, node);
    }
    // 读取链表交叉之前的结点个数
    scanf("%d%d",&beforeCross1,&beforeCross2);

    // 输出1：存储列表结果
    printf("Store Linkedlist\n");
    outputStudentLinkedList(class1);
    outputStudentLinkedList(class2);

    // 执行反转链表操作
    class1 = reverseStudentLinkedList(class1);
    class2 = reverseStudentLinkedList(class2);

    // 输出2：反转列表结果
    printf("Reverse Linkedlist\n");
    outputStudentLinkedList(class1);
    outputStudentLinkedList(class2);

    // 创建交叉链表并找到第一个交点
    createCrossLink(class1, class2, beforeCross1, beforeCross2);
    node = findCrossBeginNode(class1, class2);

    // 输出3：链表交点
    printf("Cross node\n");
    printf("{ID:%s, Grade:%d}\n", node->StuID, node->Grade);

    // 销毁交叉链表
    destroyCrossLink(class1, class2, node);

    return 0;
}