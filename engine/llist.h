#ifndef LLIST_H
#define LLIST_H
#include "todo.h"
/* llist.h
 * Generic Linked List
 */

typedef struct Node {
    TodoItem *item;
    struct Node *next;
} TodoItemNode;

typedef TodoItemNode *llist;

llist *llist_create(TodoItem *item);
void llist_free(llist *list);
void llist_push(llist *list, TodoItem *item);
TodoItem *llist_pop(llist *list);
void llist_print(llist *list, void (*print)(TodoItem *item));
unsigned int llist_length(llist *list);
#endif
