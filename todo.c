#include "todo.h"
#include <string.h>
#include <stdio.h>

TodoItem create_todo(const char *title) {
  TodoItem item;
  strncpy(item.title, title, MAX_TODO_TITLE_LENGTH);
  item.completed = false;
  return item;
}

void mark_todo(TodoItem *item, bool completed) {
  (*item).completed = completed;
}

void print_todo(TodoItem *item) {
  printf("%s - ", item->title);
  if (item->completed) {
    printf("Completed\n");
  } else {
    printf("Not Completed\n");
  }
}
