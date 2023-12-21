/* llist.c
 * Generic Linked List implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include "llist.h"

llist *llist_create(TodoItem *new_item)
{
    TodoItemNode *new_node;

    llist *new_list = (llist *)malloc(sizeof (llist));
    *new_list = (TodoItemNode *)malloc(sizeof (TodoItemNode));
    
    new_node = *new_list;
    new_node->item = new_item;
    new_node->next = NULL;
    return new_list;
}

void llist_free(llist *list)
{
    TodoItemNode *curr = *list;
    TodoItemNode *next;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

void llist_push(llist *list, TodoItem *item)
{
    TodoItemNode *head;
    TodoItemNode *new_node;
    if (list == NULL || *list == NULL) {
        fprintf(stderr, "llist_add_inorder: list is null\n");
    }

    head = *list;
    
    // Head is empty node
    if (head->item == NULL)
        head->item = item;

    // Head is not empty, add new node to front
    else {
        new_node = malloc(sizeof (TodoItemNode));
        new_node->item = item;
        new_node->next = head;
        *list = new_node;
    }
}

TodoItem *llist_pop(llist *list)
{
    TodoItem *popped_item;
    TodoItemNode *head = *list;

    if (list == NULL || head->item == NULL)
        return NULL;
    
    popped_item = head->item;
    *list = head->next;

    free(head);

    return popped_item;
}

void llist_print(llist *list, void (*print)(TodoItem *))
{
    TodoItemNode *curr = *list;
    while (curr != NULL) {
        print(curr->item);
        printf(" ");
        curr = curr->next;
    }
    putchar('\n');
}

unsigned int llist_length(llist *list) {
  if (list == NULL) {
    return 0;
  }

  TodoItemNode *curr = *list;
  unsigned int length = 0;
  while (curr != NULL) {
    length++;
    curr = curr->next;
  }

  return length;
}
