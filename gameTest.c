/*
#include<stdio.h>

#define BOARD_COLOUMNS 7
#define BOARD_ROWS 6
void printBoard(int board [BOARD_COLOUMNS][BOARD_ROWS]);
int takeTurn(int board[BOARD_COLOUMNS][BOARD_ROWS], int col, int player);
int checkWin();
int checkBoard();

int main(){
    int board[BOARD_COLOUMNS][BOARD_ROWS] = {0};
    int check = 0;
    int player = 1;
    while (check == 0) {
        int col = 0;
        printf("Enter number coordinate: ");

         while(1){ 
            if(1 != scanf("%d", &col) || col < 0 || col > 6 ){
                while(getchar() != '\n');
                puts("Number out of bounds! Try again.");
            } else { 
                break;
            }
         }
        printf ("%d \n", takeTurn(board, col,player));
        printBoard(board);
        check = checkBoard(board);
        printf("%d ", check);
        if (player == 1)
        {
          player =2;
        }
        else
        {
          player =1;
        }
        
    }
    //printBoard(board);
    return 0;
}


void printBoard(int board[BOARD_COLOUMNS][BOARD_ROWS]) {
    int i, j; 
    for (i = 0; i < BOARD_ROWS; i++) {
      for (j = 0; j < BOARD_COLOUMNS; j++) {
        printf("%d ", board[j][i]); 
      }
      printf("\n");
    }
}

int takeTurn(int board[BOARD_COLOUMNS][BOARD_ROWS], int col, int player) {
    int i;
    for (i = BOARD_ROWS - 1; i >= 0; i--) {
      if (board[col][i] == 0) {
        board[col][i] = player;
        return 1;
      }
    }
    return 0;
}

int checkBoard(int board[BOARD_COLOUMNS][BOARD_ROWS]){
    int i, j; 
    //Check horizontal 
    for (i = 0; i < BOARD_ROWS; i++) {
      for (j = 0; j < BOARD_COLOUMNS - 3; j++) {
        if (board[j][i] != 0){
          if (checkWin(board[j][i],board[j+1][i],board[j+2][i],board[j+3][i]) == 1){
            return 1;
          }
        }
      }
    }
    
    //Check Vertical
    for (i = 0; i < BOARD_ROWS - 3; i++) {
      for (j = 0; j < BOARD_COLOUMNS; j++) {
        if (board[j][i] != 0){
          if (checkWin(board[j][i],board[j][i + 1],board[j][i + 2],board[j][i + 3]) == 1){
            return 1;
          }
        }
      }
    }
    //check diaginal
    for (j = 0; j < BOARD_ROWS; j++) {
      for (i = 0; i < BOARD_COLOUMNS - 3; i++) {
        if (board[i][j] != 0){
          if (j <= 3 ) {
            if (checkWin(board[i][j],board[i + 1][j + 1],board[i + 2][j + 2],board[i + 3][j + 3])){
              return 1;
            }
          }
          if (j >= 3 ) {
            if (checkWin(board[i][j],board[i + 1][j - 1],board[i + 2][j - 2],board[i + 3][j - 3])){
              return 1;
            }
          }
        }
      }
    }
    return 0;
}

int checkWin(int a, int b, int c, int d){
    if (a == b && b == c && c == d) {
        return 1;
    }
    return 0;
}
*/