#include <stdio.h>
#include <string.h>
#include "todo.h"

enum Command {
  ADD = 'a',
  MARK = 'm',
  PRINT = 'p',
  REMOVE = 'r',
  QUIT = 'q'
};

TodoItem todos[MAX_TODOS];
int todos_count = 0;

void add_todo() {
  char title[MAX_TODO_TITLE_LENGTH];
  printf("Enter todo title: ");
  fgets(title, MAX_TODO_TITLE_LENGTH - 1, stdin);
  // remove the newline character from the buffer
  title[strlen(title) - 1] = '\0';

  TodoItem item = create_todo(title);
  todos[todos_count] = item;
  todos_count++;
}

void print_all_todos() {
  for (int i = 0; i < todos_count; i++) {
    print_todo(&todos[i]);
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
        add_todo();
        break;
      case MARK:
        do {
          printf("Enter todo index: ");
          op_index = getc(stdin) - '0';
          getc(stdin); // remove the newline character from the buffer
        } while (op_index < 0 || op_index >= todos_count);
        mark_todo(&todos[op_index], !todos[op_index].completed);
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
      case QUIT:
        break;
      default:
        printf("Invalid command\n");
        break;
    }

  } while (cmd != QUIT);
}
