#include "os.h"
#include "color.h"
#include <util/delay.h>
#define BOARD_COLOUMNS 7
#define BOARD_ROWS 6
#define PLAYER_1 1
#define PLAYER_2 2
#define GAME_LOOP 1
#define RESTART 0

const int BACKGROUND_COLOUR = BLACK;
const int TEXT_COLOUR = WHITE;
const int PLAYER_1_COLOUR = RED;
const int PLAYER_2_COLOUR = YELLOW;
const int BOARD_COLOUR = BLUE;
const int BOARD_OUTLINE_COLOUR = BLACK;

void print_board();
void draw_grid();
void draw_information();
void hide_information();
int checkWin();
int check_switches(int);
int change_position(int8_t changed, uint8_t max, int8_t move);
void move_cursor(int8_t);
void draw_square(uint8_t i, uint8_t j, uint16_t col_inside, uint16_t col_outside);
void draw_token(uint8_t i, uint8_t j, int turn);
void perform_action();
int takeTurn(int coloumn);
int check_board();
int check_win(int a, int b, int c, int d);

int8_t current_postition = 0;
int board[BOARD_COLOUMNS][BOARD_ROWS] = {0};
int8_t player = PLAYER_1;
int8_t game_state = GAME_LOOP;
//TODO
//Start screen
//End screen
//press any button to continue?
//readme file
//put in license
//aknowledge code used from others
//arrow for dropping instead of highlight
void main(void) {

    os_init();

    display.background = BACKGROUND_COLOUR;
    display.foreground = TEXT_COLOUR;
    clear_screen();

    os_add_task(check_switches ,100, 1);
    print_board();
    draw_information();
    sei();
    for(;;){}
}

void print_board() {
    uint8_t i, j;
    //draw grid
    draw_grid();
    //draw cursor
    draw_square(current_postition, 0, BOARD_OUTLINE_COLOUR, BOARD_COLOUR);
    //draw pieces
    for (j = 0; j < BOARD_ROWS; j++) {
      for (i = 0; i < BOARD_COLOUMNS; i++) {
        if (board[i][j] == 0)
        {
            draw_token(i,j, board[i][j]);
        }
      }
    }
}

//draw main grid
void draw_grid() {
    uint8_t i,j;
    //draw lines of the grid
    for (j = 0; j < BOARD_ROWS; j++) {
      for (i = 0; i < BOARD_COLOUMNS; i++) {
        draw_square(i, j, BOARD_COLOUR, BOARD_OUTLINE_COLOUR);
      }
    }
}

//show information about game
void draw_information(){
  display_string_xy("PLAYER  'S TURN", (LCDHEIGHT/2) - 45, 10);
  char val[2];
	sprintf(val, "%d", player);
  display_string_xy(val, (LCDHEIGHT/2) -3, 10);
  display_string_xy("USE <> TO MOVE CURSOR", (LCDHEIGHT/2) - 62, 30);
}

void hide_information() {
  display_string_xy("                   ", (LCDHEIGHT/2) - 50, 10);
  display_string_xy("                       ", (LCDHEIGHT/2) - 62, 30);
}


//cehck for buttons pressed and perform appropriate action
int check_switches(int state) {

    if (get_switch_press(_BV(SWE))) {
        move_cursor(1);
    }

    if (get_switch_press(_BV(SWW))) {
        move_cursor(-1); 
    }

    if (get_switch_press(_BV(SWC))) {
        perform_action();
    }
    return state;
}

int change_position(int8_t changed, uint8_t max, int8_t move) {
  if (move == -1) {
      if (current_postition == 0) {
        current_postition = (max + 1);
      }     
      current_postition--;
      changed = 1;
      return 1;
  }

  if (move == 1) {
      if (current_postition == max) {
        current_postition = -1;
      }
      current_postition++;
      changed = 1;
      return 1;
  }
  return 0;
}

//move the cursor to the correct position, different cursor for different game state
void move_cursor(int8_t move) {
  int8_t changed = 0;
  int8_t previous = current_postition;
  uint8_t max;

  if (game_state == GAME_LOOP) {
    max = BOARD_COLOUMNS - 1;
  }
  else
  {
    max = 1;
  }

  changed = change_position(changed,max,move);

  if (game_state == GAME_LOOP) {  
    while (board[current_postition][0] != 0)
    {
      changed = change_position(changed,max,move);;
    }
    if (changed == 1) {
        draw_square(previous, 0, BOARD_COLOUR, BOARD_OUTLINE_COLOUR);
        draw_square(current_postition, 0, BOARD_OUTLINE_COLOUR, BOARD_COLOUR);
        draw_token(previous, 0, board[previous][0]);
        draw_token(current_postition, 0, board[current_postition][0]);
    }
  }
  else
  {
      rectangle rect_prev = {
            ((LCDHEIGHT/2)* (previous + 1)) - 103,
            ((LCDHEIGHT/2)* (previous + 1)) - 57,
            37,
            37,
      };
      fill_rectangle(rect_prev,BACKGROUND_COLOUR);
      rectangle rect = {
            ((LCDHEIGHT/2)* (current_postition + 1)) - 103,
            ((LCDHEIGHT/2)* (current_postition + 1)) - 57,
            37,
            37,
      };
      fill_rectangle(rect,TEXT_COLOUR);
  }
  
}

//Draw a square for the grid
void draw_square(uint8_t i, uint8_t j, uint16_t col_inside, uint16_t col_outside) {
    rectangle rect_outline = {
            25 + (30 * (1 + i)),
            55 +(30 * (1 + i)),
            20 + (30 * (1 + j)),
            50 + (30 * (1 + j)),
    };
    fill_rectangle(rect_outline,col_outside);
    rectangle rect = {
            25 + (30 * (1 + i)) + 1,
            55 + (30 * (1 + i)) -1,
            20 + (30 * (1 + j)) + 1,
            50 + (30 * (1 + j)) -1,
    };
    fill_rectangle(rect,col_inside);
}

//draw a token that has been placed into the grid
void draw_token(uint8_t i, uint8_t j, int turn) {
    uint16_t x,y;
    x = 40 + (30 * (1 + i));
    y = 35 + (30 * (1 + j));
    if (turn == PLAYER_1)
    {
        draw_circle(x,y,PLAYER_1_COLOUR, 10);
    }
    else if (turn == PLAYER_2)   
    {
        draw_circle(x,y,PLAYER_2_COLOUR, 10);
    }
    else if (turn == 0)
    {
      draw_circle(x,y,WHITE, 10);
    }
    
}

//Updates the board
int takeTurn(int coloumn) {
    int j;
    int prev = -1;
    for (j = 0; j < BOARD_ROWS; j++) {
      if (board[coloumn][j] == 0) {
        if (prev != - 1){
          draw_token(coloumn,prev,0);
        }
        draw_token(coloumn,j,player);
        prev = j;
      }
      _delay_ms(50);
    }
    board[coloumn][prev] = player;
    return 0;
}

//Checks the state of the board, to see if a win is found
int check_board(){
    int i, j;
    uint16_t x0,y0,x1,y1;
    int col;
    if(player == PLAYER_1) {
      col = PLAYER_1_COLOUR;
    }
    else {
      col = PLAYER_2_COLOUR;
    }
    
    //Check horizontal 
    for (j = 0; j < BOARD_ROWS; j++) {
      for (i = 0; i < BOARD_COLOUMNS - 3; i++) {
        if (board[i][j] != 0){
          if (check_win(board[i][j],board[i+1][j],board[i+2][j],board[i+3][j])){
            x0 = 40 + (30 * (1 + i));
            y0 = 35 + (30 * (1 + j));
            x1 = 40 + (30 * (1 + (i+3)));
            y1 = 35 + (30 * (1 + j));
            draw_line(x0,y0,x1,y1,col);
            return 1;
          }
        }
      }
    }
    
    //Check Vertical
    for (j = 0; j < BOARD_ROWS - 3; j++) {
      for (i = 0; i < BOARD_COLOUMNS; i++) {
        if (board[i][j] != 0){
          if (check_win(board[i][j],board[i][j + 1],board[i][j + 2],board[i][j + 3])){
            x0 = 40 + (30 * (1 + i));
            y0 = 35 + (30 * (1 + j));
            x1 = 40 + (30 * (1 + i));
            y1 = 35 + (30 * (1 + (j+3)));
            draw_line(x0,y0,x1,y1,col);
            return 1;
          }
        }
      }
    }

    //Check Diagonal
    for (j = 0; j < BOARD_ROWS; j++) {
      for (i = 0; i < BOARD_COLOUMNS - 3; i++) {
        if (board[i][j] != 0){
          if (j <= 2 ) {
            if (check_win(board[i][j],board[i + 1][j + 1],board[i + 2][j + 2],board[i + 3][j + 3])){
              x0 = 40 + (30 * (1 + i));
              y0 = 35 + (30 * (1 + j));
              x1 = 40 + (30 * (1 + (i+3)));
              y1 = 35 + (30 * (1 + (j+3)));
              draw_line(x0,y0,x1,y1,col);
              return 1;
            }
          }
          if (j >= 3 ) {
            if (check_win(board[i][j],board[i + 1][j - 1],board[i + 2][j - 2],board[i + 3][j - 3])){
              x0 = 40 + (30 * (1 + i));
              y0 = 35 + (30 * (1 + j));
              x1 = 40 + (30 * (1 + (i+3)));
              y1 = 35 + (30 * (1 + (j-3)));
              draw_line(x0,y0,x1,y1,col);
              return 1;
            }
          }
        }
      }
    }
    
    //Check if board is full
    for (i = 0; i < BOARD_COLOUMNS; i++) {
      if (board[i][0] == 0) {
        return 0;
      }
    }

    return 2;
}

//Compares four tokens to see if they are from the same player
int check_win(int a, int b, int c, int d){
    if (a == b && b == c && c == d) {
        return 1;
    }
    return 0;
}

void perform_action() {
    cli();
    if (game_state == GAME_LOOP) {
      takeTurn(current_postition);
      if (check_board() == 1) {
        char val[2];
		  	sprintf(val, "%d", player);
        hide_information();
        display_string_xy("PLAYER  WINS!", (LCDHEIGHT/2) - 37, 10);
        display_string_xy(val, (LCDHEIGHT/2) + 1, 10);
        display_string_xy("RESTART?", (LCDHEIGHT/2) - 103, 27);
        display_string_xy("END GAME", (LCDHEIGHT) - 103, 27);
        current_postition = 0;
        rectangle rect = {
            (LCDHEIGHT/2) - 103,
            (LCDHEIGHT/2) - 57,
            37,
            37,
        };
        fill_rectangle(rect,TEXT_COLOUR);
        game_state = RESTART;
      }
      else if (check_board() == 2)
      {
        hide_information();
        display_string_xy("IT'S A DRAW!", (LCDHEIGHT/2) - 35, 10);
        display_string_xy("RESTART?", (LCDHEIGHT/2) - 103, 27);
        display_string_xy("END GAME", (LCDHEIGHT) - 103, 27);
        current_postition = 0;
        rectangle rect = {
            (LCDHEIGHT/2) - 103,
            (LCDHEIGHT/2) - 57,
            37,
            37,
        };
        fill_rectangle(rect,TEXT_COLOUR);
        game_state = RESTART;
      }
      
      if (player == PLAYER_1)
      {
        player = PLAYER_2;
      }
      else
      {
        player = PLAYER_1;
      }
      if (game_state == GAME_LOOP) {
        char val[2];
		    sprintf(val, "%d", player);
        display_string_xy(val, (LCDHEIGHT/2) -3, 10);
      }
    }
    else
    {
      //Set BOARD ELEMENTS BACK TO 0
      int8_t i,j;
      for (j = 0; j < BOARD_ROWS; j++) {
        for (i = 0; i < BOARD_COLOUMNS; i++) {
          if (board[i][j] != 0)
          {
            board[i][j] = 0;
          }
        }
      }
      clear_screen();
      print_board();
      current_postition = 0;
      player = PLAYER_1;
      game_state = GAME_LOOP;
      draw_information();
    }
    
    sei();
}