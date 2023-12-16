#ifndef TODO_H
#define TODO_H
#include <stdbool.h>
#define MAX_TODO_TITLE_LENGTH 32
#define MAX_TODOS 10

typedef struct {
  char title[MAX_TODO_TITLE_LENGTH];
  bool completed;
} TodoItem;

TodoItem create_todo(const char *title);
void mark_todo(TodoItem *item, bool completed);
void print_todo(TodoItem *item);
#endif
