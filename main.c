#include "engine/todo.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MAX_TODOS UINT_MAX
#define MAX_PATH_LENGTH 1024
#define STB_DS_IMPLEMENTATION

#define BORDERS_PAIR 1
#define DEFAULT_PAIR 2
#define COMPLETED_PAIR 3

#include "3rd-party/stb-ds.h"
#define KEY_BINDINGS " [a]dd [v]view [m]ark [d]elete [w]rite [l]oad [q]uit"

char SAVE_FILE[MAX_PATH_LENGTH] = "";
TodoItem *todos = NULL;

enum Command {
  ADD = 'a',
  VIEW = 'v',
  MARK = 'm',
  REMOVE = 'd',
  QUIT = 'q',

  WRITE_TO_FILE = 'w',
  LOAD_FROM_FILE = 'l',

  UP = 'k',
  DOWN = 'j',

  UP_ARROW = KEY_UP,
  DOWN_ARROW = KEY_DOWN,
  SPACE = ' '
};

void clear_todos(void) {
  for (int i = 0; i < arrlen(todos); i++) {
    todo_free_item(&todos[i]);
  }

  arrfree(todos);
}

void initialize_curses(int *width, int *height) {
  initscr();
  curs_set(0);
  int tw, th;
  getmaxyx(stdscr, th, tw);

  *width = tw;
  *height = th;

  start_color();
  init_pair(BORDERS_PAIR, COLOR_BLACK, COLOR_WHITE); 
  init_pair(DEFAULT_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(COMPLETED_PAIR, COLOR_BLACK, COLOR_GREEN);
  // turn off echoing of keys, and enter cbreak mode,
  // where no buffering is performed on keyboard input
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

void draw_header(int width) {
  attron(COLOR_PAIR(BORDERS_PAIR));
  move(0, 0);
  addstr(" Todd - ");
  addstr(SAVE_FILE);
  for (int i = strlen(" Todd - ") + strlen(SAVE_FILE); i < width - 1; i++) {
    addch(' ');
  }
}

void draw_footer(int width, int height) {
  move(height - 1, 0);
  addch(' ');
  int todos_count = arrlen(todos);
  int marked_count = 0;

  for (int i = 0; i < todos_count; i++) {
    if (todos[i].completed) {
      marked_count++;
    }
  }

  // get the length of the string
  int length = snprintf(NULL, 0, "%d/%d", marked_count, todos_count);
  // print the string
  printw("%d/%d", marked_count, todos_count);

  // print the rest of the line but leave space for the key bindings
  for (unsigned long i = length; i < width - strlen(KEY_BINDINGS) - 1; i++) {
    addch(' ');
  }

  // print the key bindings
  printw(KEY_BINDINGS);

  attroff(COLOR_PAIR(BORDERS_PAIR));
}

void alert(const char *message, int terminal_width, int terminal_height) {
  int window_height = 5;
  int window_width = terminal_width - 4;
  int window_y = terminal_height / 2 - 1;
  int window_x = 2;

  int message_x = (terminal_width - 4 - strlen(message)) / 2;
  int message_y = 2;

  // create a new window
  WINDOW *alert_window = newwin(window_height, window_width, window_y, window_x);
  // draw a border around the window
  box(alert_window, 0, 0);
  // print the message in the middle of the window
  mvwprintw(alert_window, message_y, message_x, message);
  // refresh the window
  wrefresh(alert_window);
  // wait for a key press
  getch();

  // delete the window
  delwin(alert_window);
}

void input_string(int terminal_width, int terminal_height, char *buffer, int buffer_size) {
  int window_height = 5;
  int window_width = terminal_width - 4;
  int window_y = terminal_height / 2 - 1;
  int window_x = 2;

  int message_x = strlen("Enter todo title: ") + 1;
  int message_y = 2;

  curs_set(1);
  // create a new window
  WINDOW *input_window = newwin(window_height, window_width, window_y, window_x);
  // draw a border around the window
  box(input_window, 0, 0);
  // print the message in the middle of the window
  mvwprintw(input_window, message_y, message_x, "Enter todo title: ");
  // refresh the window
  wrefresh(input_window);
  // wait for a key press
  echo();
  mvwgetnstr(input_window, message_y, message_x + strlen("Enter todo title: "), buffer, buffer_size);
  noecho();

  // delete the window
  delwin(input_window);
  curs_set(0);
}

void display_todos(int width, int height, int current_line) {
  int current_y = 1;
  int current_index = 0;
  const char *padding = "...";
  unsigned long title_length = 0;
  unsigned long padding_length = strlen(padding);
  
  char *title = (char *)malloc(width);

  attron(COLOR_PAIR(DEFAULT_PAIR));

  // calculate the offset
  if (height - 4 <= current_line) {
    // ignore the first n todos
    current_index = current_line - (height - 4);
  }
  
  // while we have space to print todos
  while (current_y < height - 2) {
    // check if we're out of bounds
    if (current_index >= arrlen(todos)) {
      break;
    }

    // copy the title to the buffer
    title_length = strlen(todos[current_index].title);
    strncpy(title, todos[current_index].title, width - padding_length - 1);
    // append the padding if needed
    if (title_length > strlen(title)) {
      strcat(title, padding);
    }

    // move to the correct line
    move(current_y, 0);

    // if the current todo is completed, print it as green
    if (todos[current_index].completed) {
      attroff(COLOR_PAIR(DEFAULT_PAIR));
      attron(COLOR_PAIR(COMPLETED_PAIR));
    }

    // highlight the current line
    if (current_index == current_line) {
      attron(A_UNDERLINE);
    }

    printw("%d. %s", current_index + 1, title);

    // increment the current y and index
    current_y++;
    current_index++;

    attroff(A_UNDERLINE);
    attroff(COLOR_PAIR(COMPLETED_PAIR));
    attron(COLOR_PAIR(DEFAULT_PAIR));
  }

  free(title);
}

// Command handlers
void add_command_handler(int width, int height) {
  char title[TODO_MAX_TITLE_LENGTH];

  input_string(width, height, title, TODO_MAX_TITLE_LENGTH);

  TodoItem item = todo_create_item(title);
  arrput(todos, item);
}

void view_command_handler(int width, int height, int index) {
  TodoItem item = todos[index];
  // create an alert in the middle of the screen
  alert(item.title, width, height);
}

void mark_command_handler(int index) {
  TodoItem item = todos[index];

  todo_mark_item(&item, !item.completed);

  todos[index] = item;
}

void remove_command_handler(int index) {
  arrdel(todos, index);
}

void write_to_file_handler(void) {
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

void load_from_file_handler(void) {
  // clear the current todos
  clear_todos();

  FILE *file = fopen(SAVE_FILE, "rb");

  // if unable to open the file, return
  if (file == NULL) {
    return;
  }

  char magic[5] = {'\0'};
  int todos_count = 0;
  // check the "TODD" (without the \0) magic value
  fread(magic, sizeof(char), 4, file);
  if (strcmp(magic, "TODD") != 0) {
    printf("Invalid file format\nExpected TODD, got %s\n", magic);
    return;
  }

  // read the number of todos
  fread(&todos_count, sizeof(int), 1, file);

  // read each todo
  for (int i = 0; i < todos_count; i++) {
    // read the title length
    unsigned char title_length = 0;
    fread(&title_length, sizeof(unsigned char), 1, file);

    // read the title
    char *title = malloc(title_length + 1);
    fread(title, sizeof(char), title_length, file);
    title[title_length] = '\0';

    // read the completed flag
    bool_t completed = false;
    fread(&completed, sizeof(bool_t), 1, file);

    // create the todo item
    TodoItem item = todo_create_item(title);

    // mark the item as completed if needed
    if (completed) {
      todo_mark_item(&item, true);
    }

    // add the item to the list
    arrput(todos, item);
  }
}

int main(int argc, char **argv) {
  int terminal_width = 0;
  int terminal_height = 0;
  int key = 0;
  int running = 1;
  int current_line = 0;

  // set the save file path if provided
  if (argc > 1 && strlen(argv[1]) < MAX_PATH_LENGTH) {
    strncpy(SAVE_FILE, argv[1], MAX_PATH_LENGTH);
  } else {
    // load the default save file which is "~/.todos.todd"
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    // check if we have enough space to store the path
    assert(strlen(homedir) + strlen("/.todos.todd") < MAX_PATH_LENGTH);
    // create the path
    snprintf(SAVE_FILE, MAX_PATH_LENGTH, "%s/.todos.todd", homedir);
  }

  // if file exists, load it
  load_from_file_handler();
  initialize_curses(&terminal_width, &terminal_height);

  while (running) {
    clear();
    draw_header(terminal_width);
    draw_footer(terminal_width, terminal_height);
    display_todos(terminal_width, terminal_height, current_line);
    refresh();

    // wait for a key press
    key = getch();

    // handle the key press
    switch (key) {
      case LOAD_FROM_FILE:
        load_from_file_handler();
        alert("Loaded!", terminal_width, terminal_height);
        break;
      case WRITE_TO_FILE:
        write_to_file_handler();
        alert("Saved!", terminal_width, terminal_height);
        break;
      case ADD:
        add_command_handler(terminal_width, terminal_height);
        break;
      case VIEW:
        view_command_handler(terminal_width, terminal_height, current_line);
        break;
      case MARK:
      case SPACE:
        mark_command_handler(current_line);
        break;
      case REMOVE:
        remove_command_handler(current_line);
        if (current_line >= arrlen(todos)) {
          current_line = arrlen(todos) - 1;
        }
        break;
      case QUIT:
        running = 0;
        break;
      case UP:
      case UP_ARROW:
        current_line--;
        if (current_line < 0) {
          current_line = 0;
        }
        break;
      case DOWN:
      case DOWN_ARROW:
        current_line++;
        if (current_line >= arrlen(todos)) {
          current_line = arrlen(todos) - 1;
        }
        break;
    }
  }

  endwin();

  return 0;
}
