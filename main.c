#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine/todo.h"

#define MAX_TODOS 10
#define SAVE_FILE "todos.todd"

enum Command {
  ADD = 'a',
  MARK = 'm',
  PRINT = 'p',
  REMOVE = 'r',
  QUIT = 'q',

  SERIALIZE = 's'
};

TodoItem todos[MAX_TODOS];
int todos_count = 0;

void add_command_handler() {
  char title[TODO_MAX_TITLE_LENGTH];
  printf("Enter todo title: ");
  fgets(title, TODO_MAX_TITLE_LENGTH - 1, stdin);
  // remove the newline character from the buffer
  title[strlen(title) - 1] = '\0';

  TodoItem item = todo_create_item(title);
  todos[todos_count] = item;
  todos_count++;
}

void print_all_todos() {
  for (int i = 0; i < todos_count; i++) {
    todo_print_item(&todos[i]);
  }
}

void remove_todo_at_index(int index) {
  for (int i = index; i < todos_count - 1; i++) {
    todos[i] = todos[i + 1];
  }
  todos_count--;
}

int main(int argc, char **argv) {
  char cmd;
  int op_index = -1;
  int bs;
  FILE *file;

  do {
    printf("Enter command: ");
    cmd = getc(stdin);
    getc(stdin); // remove the newline character from the buffer

    switch (cmd) {
      case ADD:
        if (todos_count + 1 > MAX_TODOS) {
          printf("No more space for new todos\n");
          break;
        }
        add_command_handler();
        break;
      case MARK:
        do {
          printf("Enter todo index: ");
          op_index = getc(stdin) - '0';
          getc(stdin); // remove the newline character from the buffer
        } while (op_index < 0 || op_index >= todos_count);
        todo_mark_item(&todos[op_index], !todos[op_index].completed);
        break;
      case PRINT:
        print_all_todos();
        break;
      case REMOVE:
        do {
          printf("Enter todo index: ");
          op_index = getc(stdin) - '0';
          getc(stdin); // remove the newline character from the buffer
        } while (op_index < 0 || op_index >= todos_count);
        remove_todo_at_index(op_index);
      case SERIALIZE:
        file = fopen(SAVE_FILE, "wb");
        // write a magic value "TODD" to the file
        fwrite("TODD", sizeof(char), 4, file);
        // write the number of todos to the file
        fwrite(&todos_count, sizeof(int), 1, file);
        // write each todo to the file
        for (int i = 0; i < todos_count; i++) {
          char *buffer = todo_item_serialize(&todos[i], &bs);
          fwrite(buffer, sizeof(char), bs, file);
          free(buffer);
        }
        fclose(file);
        break;
      case QUIT:
        break;
      default:
        printf("Invalid command\n");
        break;
    }

  } while (cmd != QUIT);

  for (int i = 0; i < todos_count; i++) {
    todo_free_item(&todos[i]);
  }
}
