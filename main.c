#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine/todo.h"
#include "engine/llist.h"

#define MAX_TODOS 10
#define SAVE_FILE "todos.todd"

enum Command {
  ADD = 'a',
  MARK = 'm',
  PRINT = 'p',
  REMOVE = 'r',
  QUIT = 'q',

  WRITE_TO_FILE = 'w',
  LOAD_FROM_FILE = 'l',
};

llist *todos;

void add_command_handler() {
  char title[TODO_MAX_TITLE_LENGTH];
  printf("Enter todo title: ");
  fgets(title, TODO_MAX_TITLE_LENGTH - 1, stdin);
  // remove the newline character from the buffer
  title[strlen(title) - 1] = '\0';

  TodoItem item = todo_create_item(title);
  if (llist_length(todos) == 0) {
    todos = llist_create(&item);
  } else {
    llist_push(todos, &item);
  }
}

void mark_command_handler() {
  int op_index = -1;
  do {
    printf("Enter todo index: ");
    op_index = getc(stdin) - '0';
    getc(stdin); // remove the newline character from the buffer
  } while (op_index < 0 || op_index >= llist_length(todos));

  todo_mark_item(todos[op_index]->item, !todos[op_index]->item->completed);
}

void print_command_handler() {
  llist_print(todos, todo_print_item);
}

void remove_todo_at_index(int index) {
  // TODO!: Implement this
  /* for (int i = index; i < todos_count - 1; i++) { */
  /*   todos[i] = todos[i + 1]; */
  /* } */
  /* todos_count--; */
}

void write_to_file_handler() {
  FILE *file = fopen(SAVE_FILE, "wb");
  int todos_count = llist_length(todos);
  int bs = 0;
  // write a magic value "TODD" to the file
  fwrite("TODD", sizeof(char), 4, file);
  // write the number of todos to the file
  fwrite(&todos_count, sizeof(int), 1, file);
  // write each todo to the file
  for (int i = 0; i < todos_count; i++) {
    char *buffer = todo_item_serialize(todos[i]->item, &bs);
    fwrite(buffer, sizeof(char), bs, file);
    free(buffer);
  }
  fclose(file);
}

int main(int argc, char **argv) {
  char cmd;

  do {
    printf("Enter command: ");
    cmd = getc(stdin);
    getc(stdin); // remove the newline character from the buffer

    switch (cmd) {
      case ADD:
        if (llist_length(todos) + 1 > MAX_TODOS) {
          printf("No more space for new todos\n");
          break;
        }
        add_command_handler();
        break;
      case MARK:
        mark_command_handler();
        break;
      case PRINT:
        print_command_handler();
        break;
      case REMOVE:
        break;
      case WRITE_TO_FILE:
        write_to_file_handler();
        break;
      case QUIT:
        break;
      default:
        printf("Invalid command\n");
        break;
    }

  } while (cmd != QUIT);

  // free all the todos
  int todos_count = llist_length(todos);

  for (int i = 0; i < todos_count; i++) {
    todo_free_item(todos[i]->item);
    llist_pop(todos);
  }
}
