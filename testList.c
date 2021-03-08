#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>


struct element
{
    int value;

    LIST_ENTRY(element) node;
};

//定义sample_list链表 类型为结构体element
LIST_HEAD(sample_list, element);

//静态初始化 list1
static struct sample_list list1 = LIST_HEAD_INITIALIZER(list1);


int main(int argc, char** argv)
{
    int i = 0;
    struct element* e1 = NULL;
    struct element* e2 = NULL;
    struct sample_list* list = NULL;

    list = &list1;

    for (i = 0; i < 4; i++) {
        e1 = calloc(1, sizeof(*e1));
        e1->value = i;
		//将元素添加至链表前端
        LIST_INSERT_HEAD(list, e1, node);
    }
	
	//获取链表的第一个元素
    e1 = LIST_FIRST(list);
    printf("The first element is %d.\n", e1->value);

    printf("list1: ");
	//遍历链表
    LIST_FOREACH(e1, list, node) {
        if (LIST_NEXT(e1, node) != NULL) {
            printf("[ %2d ] -> ", e1->value);
        } else {
            printf("[ %2d ]", e1->value);//最后一个
        }
    }
    printf("\n");

    // Remove all elements from list1
    while (!LIST_EMPTY(list)) {
		//获取链表第一个元素
        e1 = LIST_FIRST(list);
		//特定元素从LIST中移除
        LIST_REMOVE(e1, node);
        free(e1);
    }

    list = malloc(sizeof(*list));

    // Initialize the head of list2
    LIST_INIT(list);

    e1 = calloc(1, sizeof(*e1));
    e1->value = -2;
	//将元素添加至链表前端
    LIST_INSERT_HEAD(list, e1, node);

    e2 = calloc(1, sizeof(*e2));
    e2->value = -1;
	//将元素插入至某个元素之前
	//e2插入至e1之前
    LIST_INSERT_BEFORE(e1, e2, node);

    e2 = calloc(1, sizeof(*e2));
    e2->value = -3;
	//将元素插入至某个元素之后
    LIST_INSERT_AFTER(e1, e2, node);

    printf("list2: ");
	//遍历链表
    LIST_FOREACH(e1, list, node) {
        if (LIST_NEXT(e1, node) != NULL) {
            printf("[ %2d ] -> ", e1->value);
        } else {
            printf("[ %2d ]", e1->value);//最后一个
        }
    }
    printf("\n");

    // Remove all elements from list2
	//获取链表第一个元素
    e1 = LIST_FIRST(list);
    while (e1 != NULL) {
        e2 = LIST_NEXT(e1, node);
     free(e1);
     e1 = e2;
    }
    free(list);

    return 0;
}
