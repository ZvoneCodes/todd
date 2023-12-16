#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TODO_TITLE_LENGTH 32
#define MAX_TODOS 10

typedef struct {
  char title[MAX_TODO_TITLE_LENGTH];
  bool completed;
} TodoItem;

enum Command {
  ADD = 'a',
  MARK = 'm',
  PRINT = 'p',
  QUIT = 'q'
};

TodoItem create_todo(const char *title) {
  TodoItem item;
  strncpy(item.title, title, MAX_TODO_TITLE_LENGTH);
  item.completed = false;
  return item;
}

void mark_todo(TodoItem *item, bool completed) {
  (*item).completed = completed;
}

void print_todo(TodoItem item) {
  printf("%s - ", item.title);
  if (item.completed) {
    printf("Completed\n");
  } else {
    printf("Not Completed\n");
  }
}

TodoItem todos[MAX_TODOS];
int todos_count = 0;

void add_todo() {
  char title[MAX_TODO_TITLE_LENGTH];
  printf("Enter todo title: ");
  fgets(title, MAX_TODO_TITLE_LENGTH - 1, stdin);

  TodoItem item = create_todo(title);
  todos[todos_count] = item;
  todos_count++;
}

void print_all_todos() {
  for (int i = 0; i < todos_count; i++) {
    print_todo(todos[i]);
  }
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
      case QUIT:
        break;
      default:
        printf("Invalid command\n");
        break;
    }

  } while (cmd != QUIT);
}
