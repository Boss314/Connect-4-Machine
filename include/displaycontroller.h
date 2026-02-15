#include "types.h"

#ifndef DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_
#define DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_

extern State_t current_state;
extern Move_t move_to_make;
extern Board_t game_board;


void fn_MAKING_MOVE(void);

//void fn_START(void);

void Display_write(void);

void Display_init(void);

void Display_clear(void);

void Display_printCentered(char *text);


#endif /* DISPLAYCONTROLLER_DISPLAYCONTROLLER_H_ */
