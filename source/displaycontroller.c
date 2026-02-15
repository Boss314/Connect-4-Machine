#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "include/displaycontroller.h"
#include "include/types.h"
#include "include/sensorsdriver.h"
#include "include/board.h"
#include "include/connect4algorithm.h"

Graphics_Context g_sContext;

void Display_init(void){
    // Initializes display
    Crystalfontz128x128_Init();

    // Set default screen orientation
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initializes graphics context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_PURPLE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    Display_clear();
    Display_write();
}

void Display_clear(void){
    Graphics_clearDisplay(&g_sContext);
}

void Display_printCentered(char *text){
    Display_clear();
    Graphics_drawStringCentered(&g_sContext,
                                (int8_t *)text,
                                AUTO_STRING_LENGTH,
                                64,
                                60,
                                OPAQUE_TEXT);
    __delay_cycles(3000000);

}

void Display_write(){
    // TODO re initing every call
    char buffer[10] = "Red to: ";
    buffer[8] = '0' + (move_to_make + 1);
    buffer[9] = '\0';

    switch (current_state) {
        case STATE_INIT:
            Display_printCentered("Initializing");
            break;
        case STATE_WAITING_FOR_MOVE:
            Display_printCentered("Your Move: Yellow");
            break;
        case STATE_CALCULATING_MOVE:
            Display_printCentered("Calculating Move");
            break;
        case STATE_MAKING_MOVE:
            Display_printCentered(buffer);
            break;
        case STATE_PLAYER_VICTORY:
            Display_printCentered("You WON!!!");
            break;
        case STATE_COMPUTER_VICTORY:
            Display_printCentered("You Lost :(");
            break;
        case STATE_DRAW:
            Display_printCentered("Draw");
            break;
    }
}

//void fn_START(void){
//
//}


void fn_MAKING_MOVE(void){
    //write the move we want to make on the display
    Display_write();

    //wait for the sensor in that column to detect a move
    Sensors_request_piece(move_to_make);

    //apply the move we made to the board
    int8_t row = game_board.height[move_to_make];
    Score_t delta = delta_score(&game_board, move_to_make, row, true);
    Board_make_move(&game_board, move_to_make, true, delta);

    //check if the computer won and decide the next state
    GameState_t state = Game_winner(delta);

    switch (state) {
        case GAME_COMPUTER_WON:
            current_state = STATE_COMPUTER_VICTORY;
            break;
        case GAME_PLAYER_WON:
            current_state = STATE_PLAYER_VICTORY;
            break;
        case GAME_DRAW:
            current_state = STATE_DRAW;
            break;
        case GAME_ONGOING:
            current_state = STATE_WAITING_FOR_MOVE;
            break;
    }
}
