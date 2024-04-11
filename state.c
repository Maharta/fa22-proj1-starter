#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);
void setInitialSnakeState(game_state_t *default_state);

const static uint8_t head_start[2] = {2, 4};
const static uint8_t tail_start[2] = {2, 2};
const static uint8_t fruit_start[2] = {2, 9};
const static uint8_t body_start[2] = {2, 3};
const static uint8_t boardRow = 18;
const static uint8_t boardCol = 20;


/* Task 1 */
game_state_t* create_default_state() {
  game_state_t* default_state = (game_state_t*) malloc(sizeof(game_state_t));

  setInitialSnakeState(default_state);

  default_state->num_rows = boardRow;
  default_state->num_snakes = 1;

  char** board = (char**) malloc((boardRow) * sizeof(char*));

  for (int i = 0; i < boardRow; i++) {
    board[i] = (char*) malloc(sizeof(char) * boardCol);
    for (int j = 0; j < boardCol; j++) {
      if ((j == 0 || j == boardCol - 1) || (i == 0 || i == boardRow - 1)) {
        board[i][j] = '#';
      } else {
        board[i][j] = ' ';
      }
    }
  }

  default_state->board = board;
  set_board_at(default_state, head_start[0], head_start[1], 'D');
  set_board_at(default_state, tail_start[0], tail_start[1], 'd');
  set_board_at(default_state, body_start[0], body_start[1], '>');
  set_board_at(default_state, fruit_start[0], fruit_start[1], '*');
  return default_state;
}

void setInitialSnakeState(game_state_t* default_state)
{
  default_state->snakes = malloc(sizeof(snake_t));
  default_state->snakes->live = true;
  default_state->snakes->head_row = head_start[0];
  default_state->snakes->head_col = head_start[1];
  default_state->snakes->tail_row = tail_start[0];
  default_state->snakes->tail_col = tail_start[1];
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  return true;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  return NULL;
}
