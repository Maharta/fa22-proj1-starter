#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);
void setInitialSnakeState(game_state_t *default_state);

const static uint8_t head_start[2] = {2, 4};
const static uint8_t tail_start[2] = {2, 2};
const static uint8_t fruit_start[2] = {2, 9};
const static uint8_t body_start[2] = {2, 3};
const static uint8_t boardRow = 18;
const static uint8_t realCol = 20;
const static uint8_t boardCol = realCol + 1;

/* Task 1 */
game_state_t *create_default_state()
{
  game_state_t *default_state = (game_state_t *)malloc(sizeof(game_state_t));

  setInitialSnakeState(default_state);

  default_state->num_rows = boardRow;

  char **board = (char **)malloc((default_state->num_rows) * sizeof(char *));

  for (int i = 0; i < default_state->num_rows; i++)
  {
    board[i] = (char *)malloc(sizeof(char) * boardCol);
    for (int j = 0; j < boardCol; j++)
    {
      if (j == boardCol - 1)
      {
        board[i][j] = '\0';
      }
      else if ((j == 0 || j == boardCol - 2) || (i == 0 || i == default_state->num_rows - 1))
      {
        board[i][j] = '#';
      }
      else
      {
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

void setInitialSnakeState(game_state_t *default_state)
{
  default_state->snakes = (snake_t *)malloc(sizeof(snake_t));
  default_state->num_snakes = 1;
  default_state->snakes[0].live = true;
  default_state->snakes[0].head_row = head_start[0];
  default_state->snakes[0].head_col = head_start[1];
  default_state->snakes[0].tail_row = tail_start[0];
  default_state->snakes[0].tail_col = tail_start[1];
}

/* Task 2 */
void free_state(game_state_t *state)
{
  free(state->snakes);
  for (int i = 0; i < boardRow; i++)
  {
    char *boardPtr = state->board[i];
    free(boardPtr);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  for (int i = 0; i < state->num_rows; i++)
  {
    fprintf(fp, "%s", state->board[i]);
    fprintf(fp, "\n");
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col)
{
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
  // TODO: Implement this function.
  return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
  return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
  return (c == 'w' || c == 'a' || c == 's' || c == 'd' ||
          c == '^' || c == '<' || c == 'v' || c == '>' ||
          c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
  if (c == '^')
    return 'w';
  if (c == '<')
    return 'a';
  if (c == 'v')
    return 's';
  if (c == '>')
    return 'd';
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
  switch (c)
  {
  case 'W':
    return '^';
  case 'A':
    return '<';
  case 'S':
    return 'v';
  case 'D':
    return '>';
  default:
    return '?'; // Return a default character for invalid input
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
  if (c == 'v' || c == 's' || c == 'S')
    return cur_row + 1;
  if (c == '^' || c == 'w' || c == 'W')
    return cur_row == 0 ? 0 : cur_row - 1;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
  switch (c)
  {
  case '>':
  case 'd':
  case 'D':
    return cur_col + 1;
  case '<':
  case 'a':
  case 'A':
    return cur_col == 0 ? 0 : cur_col - 1;
  default:
    return cur_col;
  }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
  snake_t snake = state->snakes[snum];
  char head = get_board_at(state, snake.head_row, snake.head_col);
  unsigned int row = get_next_row(snake.head_row, head);
  unsigned int col = get_next_col(snake.head_col, head);

  return get_board_at(state, row, col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
  snake_t snake = state->snakes[snum];
  char head = get_board_at(state, snake.head_row, snake.head_col);
  unsigned int row = get_next_row(snake.head_row, head);
  unsigned int col = get_next_col(snake.head_col, head);

  set_board_at(state, row, col, head);
  set_board_at(state, snake.head_row, snake.head_col, head_to_body(head));

  state->snakes[snum].head_row = row;
  state->snakes[snum].head_col = col;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
  snake_t snake = state->snakes[snum];
  char tail = get_board_at(state, snake.tail_row, snake.tail_col);
  unsigned int row = get_next_row(snake.tail_row, tail);
  unsigned int col = get_next_col(snake.tail_col, tail);
  char body = get_board_at(state, row, col);

  set_board_at(state, row, col, body_to_tail(body));
  set_board_at(state, snake.tail_row, snake.tail_col, ' ');

  state->snakes[snum].tail_row = row;
  state->snakes[snum].tail_col = col;
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  // TODO: Implement this function.
  for (unsigned int i = 0; i < state->num_snakes; i++)
  {
    snake_t *snake = &state->snakes[i];
    char head = get_board_at(state, snake->head_row, snake->head_col);
    unsigned int row = get_next_row(snake->head_row, head);
    unsigned int col = get_next_col(snake->head_col, head);

    char next = get_board_at(state, row, col);

    if (next != ' ' && next != '*')
    {
      // set head to x, meaning snake died
      set_board_at(state, snake->head_row, snake->head_col, 'x');
      snake->live = false;
    }
    else if (next == '*')
    {
      update_head(state, i);
      add_food(state);
    }
    else
    {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
  game_state_t *state = malloc(sizeof(game_state_t));

  FILE *fptr = fopen(filename, "r");

  if (fptr == NULL)
  {
    return NULL;
  }

  unsigned int buffer_size = 100;
  char buffer[buffer_size];

  unsigned int row = 0;
  while (fgets(buffer, buffer_size, fptr))
  {
    row++;
  }
  rewind(fptr);

  state->num_rows = row;
  state->board = (char **)malloc(sizeof(char *) * state->num_rows);

  // setting row back to 0 to use it for index.
  row = 0;
  while (fgets(buffer, buffer_size, fptr))
  {
    state->board[row] = malloc(strlen(buffer) * sizeof(char));
    buffer[strlen(buffer) - 1] = '\0';
    strcpy(state->board[row], buffer);
    row++;
  }

  fclose(fptr);
  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
  snake_t *snake = &state->snakes[snum];

  unsigned int cur_row = snake->tail_row;
  unsigned int cur_col = snake->tail_col;

  char snake_body = get_board_at(state, cur_row, cur_col);

  while (!is_head(snake_body))
  {
    cur_row = get_next_row(cur_row, snake_body);
    cur_col = get_next_col(cur_col, snake_body);
    snake_body = get_board_at(state, cur_row, cur_col);
  }

  snake->head_row = cur_row;
  snake->head_col = cur_col;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  unsigned int snakes_num = 10;
  state->snakes = malloc(sizeof(snake_t) * snakes_num);

  unsigned int snake_idx = 0;

  // count the number of snakes by its tail and initialize its location.
  for (unsigned int i = 0; i < state->num_rows; i++)
  {
    char *ptr = state->board[i];
    unsigned int col = 0;
    while (ptr[col] != '\0')
    {
      if (snake_idx >= snakes_num)
      {
        snakes_num = snakes_num * 2;
        state->snakes = realloc(state->snakes, sizeof(snake_t) * snakes_num);
      }
      char snake_body = ptr[col];
      if (is_tail(snake_body))
      {
        state->snakes[snake_idx].live = true;
        state->snakes[snake_idx].tail_row = i;
        state->snakes[snake_idx].tail_col = col;
        find_head(state, snake_idx);
        snake_idx++;
      }
      col++;
    }
  }

  unsigned int snakes_count = snake_idx;
  // realloc once more time to use the exact amount of memory:
  state->snakes = realloc(state->snakes, snakes_count * sizeof(snake_t));
  state->num_snakes = snakes_count;

  return state;
}
