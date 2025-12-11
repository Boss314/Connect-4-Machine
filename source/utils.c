/*
 * utils.c
 *
 *  Created on: Dec 2, 2025
 *      Author: enrico
 */


#include "include/utils.h"
#include "include/types.h"
#include "stdint.h"
#include "stdbool.h"


bool Board_find_diagonals(const Board_t * board,uint8_t c,Row_t r, const bool rising,bool * winner);


void Board_init(Board_t * board){
    int i;
    for(i=0;i<NUM_COLS;i++){
        board->column[i]=0;
        board->next_row[i]=ROW1; // since each column is empty the next piece will have to be placed in the first row
    }
}



bool Board_make_move(Board_t* board,const Move_t move,const bool is_computer){
    if(board->next_row[move] > ROW6){
        // if the selected column is full then the move is illegal
        return false;
    }else{

        if(is_computer){
            board->column[move] |= board->next_row[move]; //set the bit in the lowest empty row to 1
        }else{
            board->column[move] &= ~board->next_row[move]; //set the bit in the lowest empty row to 0
        }

        board->next_row[move] = board->next_row[move] << 1; //the next row is now the one above where we just placed our piece

        return true;
    }
}



bool Board_unmake_move(Board_t* board,const Move_t move){
    if(board->next_row[move] == ROW1){
        // if the selected column is empty there is no move to roll back
        return false;
    }else{

        board->next_row[move] = board->next_row[move] >> 1; // we took out a piece from the column, so the next row is now the one right below

        return true;
    }
}



bool Board_has_won(const Board_t * board, bool * winner){
    int c;
    int r;


    //check for vertical lines in each column
    for(c=0;c<NUM_COLS;c++){
        uint8_t count=0; //use thios variable to count how many connected identical pieces we have seen, when we reach 4 we know there is a vertical line
        *winner=true; //use this variable to temporarily store the player whose pieces we are currently counting

        for(r=ROW1;r < board->next_row[c];r=r<<1){
            if(board->column[c] & r){
                if(*winner){ //case where the current piece is from the computer and the ones below it are from the computer
                    count++;
                }else{ //case where the current piece is from the computer and the ones below it are from the player
                    *winner=true;
                    count=1;
                }
            }else{
                if(*winner){ //case where the current piece is from the player and the ones below it are from the computer
                    *winner=false;
                    count=1;
                }else{ //case where the current piece is from the player and the ones below it are from the player
                    count++;
                }
            }

            if(count>=4){
                return true;
            }
        }
    }


    //check for horizontal lines in each row
    for(r=ROW1;r<ROW7;r=r<<1){
        uint8_t count=0;
        *winner=true;

        for(c=0;c<NUM_COLS;c++){
            if(board->next_row[c] <= r){// case where the current column does not have a piece in that row
                count=0;
            }else{
                if(board->column[c] & r){
                    if(*winner){ //case where the current piece is from the computer and the ones to the left are from the computer
                        count++;
                    }else{ //case where the current piece is from the computer and the ones to the left are from the player
                        *winner=true;
                        count=1;
                    }
                }else{
                    if(*winner){ //case where the current piece is from the player and the ones to the left are from the computer
                        *winner=false;
                        count=1;
                    }else{ //case where the current piece is from the player and the ones to the left are from the player
                        count++;
                    }
                }
            }

            if(count>=4){
                return true;
            }
        }
    }


    /* when it comes to finding diagonals we can notice that on a connect 4 board there are only some diagonals that can be at least 4 peces long, for example:
     *  6 O O O O O O O
     *  5 O O O O O O O
     *  4 O O O O O O O
     *  3 X O O O O O O
     *  2 X O O O O O O
     *  1 X X X X O O O
     *    1 2 3 4 5 6 7
     *
     *  using the scanning and counting approach used earlier on the columns and rows starting from the places marked in X is
     *  guaranteed to find any 4-piece rising diagonal (increasing in row as it goes left to right), there are 6 other starting points for diagonals that go down
     */


    //check for diagonal rising lines
    if(Board_find_diagonals(board, 0, ROW3, true, winner)) return true;
    if(Board_find_diagonals(board, 0, ROW2, true, winner)) return true;
    if(Board_find_diagonals(board, 0, ROW1, true, winner)) return true;
    if(Board_find_diagonals(board, 1, ROW1, true, winner)) return true;
    if(Board_find_diagonals(board, 2, ROW1, true, winner)) return true;
    if(Board_find_diagonals(board, 3, ROW1, true, winner)) return true;

    //check for diagonal falling lines
    if(Board_find_diagonals(board, 0, ROW4, false, winner)) return true;
    if(Board_find_diagonals(board, 0, ROW5, false, winner)) return true;
    if(Board_find_diagonals(board, 0, ROW6, false, winner)) return true;
    if(Board_find_diagonals(board, 1, ROW6, false, winner)) return true;
    if(Board_find_diagonals(board, 2, ROW6, false, winner)) return true;
    if(Board_find_diagonals(board, 3, ROW6, false, winner)) return true;

    return false;
}


bool Board_find_diagonals(const Board_t * board,uint8_t c,Row_t r, const bool rising,bool * winner){
    uint8_t count=0;
    *winner=true;

    while(c<NUM_COLS && r>=ROW1 && r<=ROW6){
        if(board->next_row[c] <= r){// case where the current column does not have a piece in that row
            count=0;
        }else{
            if(board->column[c] & r){
                if(*winner){ //case where the current piece is from the computer and the ones before it are from the computer
                    count++;
                }else{ //case where the current piece is from the computer and the ones before it are from the player
                    *winner=true;
                    count=1;
                }
            }else{
                if(*winner){ //case where the current piece is from the player and the ones before it are from the computer
                    *winner=false;
                    count=1;
                }else{ //case where the current piece is from the player and the ones before it are from the player
                    count++;
                }
            }
        }

        if(count>=4){
            return true;
        }

        c++;
        r = rising ? r<<1 : r>>1;
    }

    return false;
}
