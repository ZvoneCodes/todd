#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine/todo.h"

#define MAX_TODOS 10
#define SAVE_FILE "todos.todd"
#define STB_DS_IMPLEMENTATION

#include "3rd-party/stb-ds.h"

TodoItem *todos = NULL;

enum Command {
  ADD = 'a',
  MARK = 'm',
  PRINT = 'p',
  REMOVE = 'r',
  QUIT = 'q',

  WRITE_TO_FILE = 'w',
  LOAD_FROM_FILE = 'l',
};

void add_command_handler() {
  char title[TODO_MAX_TITLE_LENGTH];
  printf("Enter todo title: ");
  fgets(title, TODO_MAX_TITLE_LENGTH - 1, stdin);
  // remove the newline character from the buffer
  title[strlen(title) - 1] = '\0';

  TodoItem item = todo_create_item(title);
  arrput(todos, item);
}

void mark_command_handler() {
  int op_index = -1;
  do {
    printf("Enter todo index: ");
    op_index = getc(stdin) - '0';
    getc(stdin); // remove the newline character from the buffer
  } while (op_index < 0 || op_index >= arrlen(todos));

  TodoItem item = todos[op_index];

  todo_mark_item(&item, !item.completed);

  todos[op_index] = item;
}

void print_command_handler() {
  for (int i = 0; i < arrlen(todos); i++) {
    printf("%d. ", i);
    todo_print_item(&todos[i]);
  }
}

void remove_command_handler() {
  int op_index = -1;
  do {
    printf("Enter todo index: ");
    op_index = getc(stdin) - '0';
    getc(stdin); // remove the newline character from the buffer
  } while (op_index < 0 || op_index >= arrlen(todos));

  arrdel(todos, op_index);
}

void write_to_file_handler() {
  FILE *file = fopen(SAVE_FILE, "wb");
  int bs = 0;
  int todos_count = arrlen(todos);
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
}

int main(int argc, char **argv) {
  char cmd;

  do {
    printf("Enter command: ");
    cmd = getc(stdin);
    getc(stdin); // remove the newline character from the buffer

    switch (cmd) {
      case ADD:
        /* if (list_length(&todos) + 1 > MAX_TODOS) { */
          /* printf("No more space for new todos\n"); */
          /* break; */
        /* } */
        add_command_handler();
        break;
      case MARK:
        mark_command_handler();
        break;
      case PRINT:
        print_command_handler();
        break;
      case REMOVE:
        remove_command_handler();
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
  
  arrfree(todos);

  /* for (int i = 0; i < list_length(&todos); i++) { */
  /*   todo_free_item(list_get(&todos, i)); */
  /* } */
  /* list_free(&todos); */
  /* free(todos); */
}
