#ifndef _SNK_STATE_H
#define _SNK_STATE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct snake_t
{
  unsigned int tail_x;
  unsigned int tail_y;
  unsigned int head_x;
  unsigned int head_y;

  bool live;
} snake_t;

typedef struct game_state_t
{
  unsigned int num_rows; // The number of rows in the game board.
  char **board;          // he game board in memory. Each element of the board array is a char* pointer to a character array containing a row of the map.

  unsigned int num_snakes; // The number of snakes on the board.
  snake_t *snakes;         // An array of snake_t structs.
} game_state_t;

game_state_t *create_default_state();
void free_state(game_state_t *state);
void print_board(game_state_t *state, FILE *fp);
void save_board(game_state_t *state, char *filename);
void update_state(game_state_t *state, int (*add_food)(game_state_t *state));
char get_board_at(game_state_t *state, unsigned int x, unsigned int y);
game_state_t *initialize_snakes(game_state_t *state);
game_state_t *load_board(char *filename);

#endif
