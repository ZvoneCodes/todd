#include "todo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Memory management
void todo_free_item(TodoItem *item) {
  free(item->title);
}

// Item operations
TodoItem todo_create_item(char *title) {
  TodoItem item;
  item.title_length = strlen(title);
  item.title = malloc(item.title_length + 1);
  strcpy(item.title, title);
  item.completed = false;

  return item;
}

void todo_mark_item(TodoItem *item, bool_t completed) {
  item->completed = completed;
}

void todo_print_item(TodoItem *item) {
  printf("%s - ", item->title);
  if (item->completed) {
    printf("Completed\n");
  } else {
    printf("Not Completed\n");
  }
}

// Serialization
char *todo_item_serialize(TodoItem *item, int *buffer_size_out) {
  // Layout of the serialized item:
  // title_length - 1 byte
  // title - title_length bytes
  // completed - 1 byte
  unsigned char title_length = item->title_length;  unsigned char completed_length = sizeof(item->completed);
  int buffer_size = sizeof(unsigned char) + title_length + completed_length;

  char *buffer = malloc(buffer_size);

  // copy title length
  memcpy(buffer, &title_length, sizeof(unsigned char));
  // copy title
  memcpy(buffer + sizeof(unsigned char), item->title, title_length);
  // copy completed
  memcpy(buffer + sizeof(unsigned char) + title_length, &item->completed, completed_length);

  *buffer_size_out = buffer_size;

  return buffer;
}
