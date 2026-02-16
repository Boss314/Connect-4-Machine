#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "include/displaycontroller.h"
#include "include/board.h"
#include "include/sensorsdriver.h"
#include "include/connect4algorithm.h"

// graphics context used for all drawing on the LCD
Graphics_Context g_sContext;

// function to initialise the LCD display and graphics context
// should be called once during program startup
void Display_init(void){
    // Initialize the ST7735 display
    Crystalfontz128x128_Init();

    // Set screen orientation (upright)
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize graphics context with foreground and background colors
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    
    // choose the font to use for text
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    // clear the display and show initial content
    Display_clear();
    Display_write();
}

// helper function to clear the entire display
void Display_clear(void){
    Graphics_clearDisplay(&g_sContext);
}

// function to draw a string centered on the screen
// char *text: text to display
void Display_printCentered(char *text){
    Display_clear();
    Graphics_drawStringCentered(&g_sContext,
                                (int8_t *)text,
                                AUTO_STRING_LENGTH,
                                64,
                                64,
                                OPAQUE_TEXT);
}

// function that updates the display based on the current state of the game
// shows messages like "Initializing", "Your move", or the move made by the computer
void Display_write(){
    switch (current_state) {
        case STATE_INIT:
            Display_printCentered("Initializing");
            break;
        case STATE_WAITING_FOR_MOVE:
            Display_printCentered("Your move: Yellow");
            break;
        case STATE_CALCULATING_MOVE:
            Display_printCentered("Calculating Move");
            break;
        case STATE_MAKING_MOVE:
        {
            char buffer[16] = "Red moves to: ";
            buffer[14] = '0' + (move_to_make + 1);
            buffer[15] = '\0';
            Display_printCentered(buffer);
            break;
        }
        case STATE_PLAYER_VICTORY:
            Display_printCentered("You WON! :D");
            break;
        case STATE_COMPUTER_VICTORY:
            Display_printCentered("You Lost :(");
            break;
        case STATE_DRAW:
            Display_printCentered("Draw");
            break;
    }
}

/* function that handles the STATE_MAKING_MOVE state
 * - updates the display to show the move
 * - waits for the physical sensor to detect a piece
 * - applies the move to the game board
 * - checks for victory and updates the current state
 */
void fn_MAKING_MOVE(void){
    Display_write();

    Sensors_request_piece(move_to_make);

    int8_t row = game_board.height[move_to_make];
    Score_t delta = delta_score(&game_board, move_to_make, row, true);
    Board_make_move(&game_board, move_to_make, true, delta);

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
