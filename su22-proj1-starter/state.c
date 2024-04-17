#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state()
{
  game_state_t *state = malloc(sizeof(game_state_t));
  state->num_rows = 18;
  state->board = malloc(state->num_rows * sizeof(char *));
  for (int i = 0; i < state->num_rows; i++)
  {
    state->board[i] = malloc(20 * sizeof(char));
    if (i == 0 || i == state->num_rows - 1)
      strcpy(state->board[i], "####################");
    else if (i == 2)
      strcpy(state->board[i], "# d>D    *         #");
    else
      strcpy(state->board[i], "#                  #");
  }
  state->num_snakes = 1;
  state->snakes = malloc(state->num_snakes * sizeof(snake_t));
  state->snakes->head_x = 4;
  state->snakes->head_y = 2;
  state->snakes->tail_x = 2;
  state->snakes->tail_y = 2;
  state->snakes->live = true;
  return state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  for (int i = 0; i < state->num_rows; i++)
    free(state->board[i]);
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  for (int i = 0; i < state->num_rows; i++)
  {
    fprintf(fp, state->board[i]);
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
char get_board_at(game_state_t *state, unsigned int x, unsigned int y)
{
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int x, unsigned int y, char ch)
{
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
  if (c == 'w' || c == 's' || c == 'd' || c == 'a')
    return true;
  else
    return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
  if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x')
    return true;
  else
    return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>vWASDx"
*/
static bool is_snake(char c)
{
  if (is_head(c) || is_tail(c) || c == '^' || c == '<' || c == '>' || c == 'v')
    return true;
  else
    return false;
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
  switch (c)
  {
  case '^':
    return 'w';
  case '<':
    return 'a';
  case 'v':
    return 's';
  case '>':
    return 'd';
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
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
  }
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c)
{
  if (c == '>' || c == 'd' || c == 'D')
    return cur_x + 1;
  if (c == '<' || c == 'a' || c == 'A')
    return cur_x - 1;
  return cur_x;
}

/*
  Returns cur_y + 1 if c is '^' or 'w' or 'W'.
  Returns cur_y - 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c)
{
  if (c == '^' || c == 'w' || c == 'W')
    return cur_y - 1;
  if (c == 'v' || c == 's' || c == 'S')
    return cur_y + 1;
  return cur_y;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
  int x = get_next_x(state->snakes[snum].head_x, state->board[state->snakes[snum].head_y][state->snakes[snum].head_x]);
  int y = get_next_y(state->snakes[snum].head_y, state->board[state->snakes[snum].head_y][state->snakes[snum].head_x]);
  return state->board[y][x];
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the x and y coordinates of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
  int cur_x = state->snakes[snum].head_x;
  int cur_y = state->snakes[snum].head_y;
  int next_x = get_next_x(cur_x, state->board[cur_y][cur_x]);
  int next_y = get_next_y(cur_y, state->board[cur_y][cur_x]);
  state->board[next_y][next_x] = state->board[cur_y][cur_x];
  state->board[cur_y][cur_x] = head_to_body(state->board[next_y][next_x]);
  state->snakes[snum].head_x = next_x;
  state->snakes[snum].head_y = next_y;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)

  ...in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
  int cur_x = state->snakes[snum].tail_x;
  int cur_y = state->snakes[snum].tail_y;
  int next_x = get_next_x(cur_x, state->board[cur_y][cur_x]);
  int next_y = get_next_y(cur_y, state->board[cur_y][cur_x]);
  state->board[next_y][next_x] = body_to_tail(state->board[next_y][next_x]);
  state->board[cur_y][cur_x] = ' ';
  state->snakes[snum].tail_x = next_x;
  state->snakes[snum].tail_y = next_y;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  for (int i = 0; i < (state->num_snakes); i++)
  {
    char next = next_square(state, i);
    if (is_snake(next) || next == '#')
    {
      // 如果下一个位置是蛇或者墙
      state->board[state->snakes[i].head_y][state->snakes[i].head_x] = 'x';
      state->snakes[i].live = false;
    }
    else if (next == '*')
    {
      // 如果下一个位置是食物
      update_head(state, i);
      add_food(state);
    }
    else
    {
      // 什么都没有
      update_head(state, i);
      update_tail(state, i);
    }
  }
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    return -1; // 打开文件失败
  }

  int numRows = 0; // 行数
  char buffer[1000];

  // 统计行数
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
    numRows++;
  }
  // 重置文件指针
  fseek(fp, 0, SEEK_SET);
  // 分配内存
  char **board = (char **)malloc(numRows * sizeof(char *));
  if (board == NULL)
  {
    fclose(fp);
    return -1; // 内存分配失败
  }
  // 逐行读取数据
  for (int i = 0; i < numRows; i++)
  {
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
      int len = strlen(buffer);
      board[i] = (char *)malloc((len + 1) * sizeof(char));
      if (board[i] == NULL)
      {
        fclose(fp);
        // TODO:加上Free
        return -1; // 内存分配失败
      }
      strcpy(board[i], buffer);
      board[i][len - 1] = '\0'; // 移除换行符
    }
  }

  game_state_t *state = malloc(sizeof(game_state_t));
  state->num_rows = numRows;
  state->board = board;

  fclose(fp);

  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
  int target_x = state->snakes[snum].tail_x;
  int target_y = state->snakes[snum].tail_y;
  while (state->board[target_y][target_x] != 'W' && state->board[target_y][target_x] != 'A' &&
         state->board[target_y][target_x] != 'S' && state->board[target_y][target_x] != 'D')
  {
    char res = state->board[target_y][target_x];
    switch (state->board[target_y][target_x])
    {
    case 'w':
    case '^':
      target_y--;
      break;
    case 'a':
    case '<':
      target_x--;
      break;
    case 's':
    case 'v':
      target_y++;
      break;
    case 'd':
    case '>':
      target_x++;
      break;
    }
  }
  state->snakes[snum].head_x = target_x;
  state->snakes[snum].head_y = target_y;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  // TODO: Implement this function.
  return NULL;
}
