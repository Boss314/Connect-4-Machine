/*
 * board.c
 *
 *  Created on: Dec 2, 2025
 *      Author: enrico
 */


#include <include/board.h>
#include <include/types.h>
#include "include/connect4algorithm.h"
#include "stdint.h"
#include "stdbool.h"






void Board_init(Board_t * board){
    int i;
    for(i=0;i<NUM_COLS;i++){
        board->column[i]=0;
        board->next_row[i]=ROW1; // since each column is empty the next piece will have to be placed in the first row
    }
    board->score=0; //since it's a blank board, the score is perfectly neutral
}



bool Board_make_move(Board_t* board,const Move_t move,const bool is_computer){
    if(move>=NUM_COLS) return false; //if the selected column is invalid the move is illegal
    if(board->next_row[move] > ROW6) return false;// if the selected column is full then the move is illegal

    // subtract the contribution to the score the spot we want to change was making
    Score_t contribution=score_contribution(board, move, board->next_row[move]);
    board->score-=contribution;

    if(is_computer){
        board->column[move] |= board->next_row[move]; //set the bit in the lowest empty row to 1
    }else{
        board->column[move] &= ~board->next_row[move]; //set the bit in the lowest empty row to 0
    }


    board->next_row[move] = board->next_row[move] << 1; //the next row is now the one above where we just placed our piece

    // recalculate the score contribution now that we changed the board and add it back to keep board.score accurate
    contribution=score_contribution(board, move, board->next_row[move] >> 1);
    board->score+=contribution;


    return true;
}



bool Board_unmake_move(Board_t* board,const Move_t move){
    if(move>=NUM_COLS) return false; //if the selected column is invalid the move is illegal
    if(board->next_row[move] == ROW1) return false;// if the selected column is empty there is no move to roll back

    // subtract the contribution to the score the spot we want to change was making
    Score_t contribution=score_contribution(board, move, board->next_row[move] >> 1);
    board->score-=contribution;

    board->next_row[move] = board->next_row[move] >> 1; // we took out a piece from the column, so the next row is now the one right below

    // recalculate the score contribution now that we changed the board and add it back to keep board.score accurate
    contribution=score_contribution(board, move, board->next_row[move]);
    board->score+=contribution;

    return true;
}




