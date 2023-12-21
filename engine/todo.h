#ifndef TODO_H
#define TODO_H
#define TODO_MAX_TITLE_LENGTH 255

#define true 1
#define false 0
typedef unsigned char bool_t;

typedef struct {
  unsigned char title_length;
  char *title;
  bool_t completed;
} TodoItem;

// Memory management
void todo_free_item(TodoItem *item);

// Item operations
TodoItem todo_create_item(char *title);
void todo_mark_item(TodoItem *item, bool_t completed);
void todo_print_item(TodoItem *item);

// Serialization
char *todo_item_serialize(TodoItem *item, int *buffer_size_out);
#endif
