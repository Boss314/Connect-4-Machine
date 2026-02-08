#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define NUM_COLS 7
#define NUM_ROWS 6

#define MAX_DEPTH 6

typedef uint8_t Col_t;
typedef int16_t Score_t;

#define SCORE_MIN INT16_MIN
#define SCORE_MAX INT16_MAX
#define COL(x) ((uint8_t)((x)-1))

typedef struct {
    uint8_t p1[NUM_COLS];   // bot
    uint8_t p2[NUM_COLS];   // player
    uint8_t height[NUM_COLS];
} Board_t;

typedef struct {
    Col_t move;
    Score_t score;
} Result_t;

static const uint8_t DIRS[4] = {
    0x11,  // dx=1, dy=0: horizontal
    0x02,  // dx=0, dy=1: vertical  
    0x12,  // dx=1, dy=1: up diagonal
    0x10   // dx=1, dy=-1 down diagonal
};

#define GET_DX(dir) ((int8_t)((dir) >> 4))
#define GET_DY(dir) ((int8_t)(((dir) & 0x0F) - 1))

// random number gen
static uint16_t rand_seed = 12345;

static inline uint8_t rand_col(void)
{
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7FFF;
    return (uint8_t)(rand_seed % NUM_COLS);
}

static inline void set_rand_seed(uint16_t seed)
{
    rand_seed = seed;
}

/// Board 

static inline void board_init(Board_t *b)
{
    memset(b, 0, sizeof(Board_t));
}

static inline bool board_full(const Board_t *b)
{
    // Check if all columns are full
    uint8_t full = 0xFF;
    for (uint8_t i = 0; i < NUM_COLS; i++) {
        full &= (b->height[i] >= NUM_ROWS) ? 0xFF : 0;
    }
    return full;
}

static inline void drop_piece(Board_t *b, Col_t c, bool player)
{
    uint8_t r = b->height[c];
    uint8_t mask = (1 << r);
    if (player) 
        b->p1[c] |= mask;
    else 
        b->p2[c] |= mask;
    b->height[c]++;
}

static inline void undo_piece(Board_t *b, Col_t c, bool player)
{
    b->height[c]--;
    uint8_t r = b->height[c];
    uint8_t mask = ~(1 << r);
    if (player) 
        b->p1[c] &= mask;
    else 
        b->p2[c] &= mask;
}

/// Drawing 

void draw_board(Board_t *b)
{
    printf("\n");
    for (int r = NUM_ROWS - 1; r >= 0; r--) {
        printf("| ");
        for (int c = 0; c < NUM_COLS; c++) {
            uint8_t m = (1 << r);
            if (b->p1[c] & m) printf("X ");
            else if (b->p2[c] & m) printf("O ");
            else printf(". ");
        }
        printf("|\n");
    }
    printf("  1 2 3 4 5 6 7\n\n");
}

/// Window delta 

static inline Score_t eval_window(uint8_t p1, uint8_t p2, bool player)
{
    // mixed
    if (p1 & p2) return 0;

    // empty 
    if (!(p1 | p2)) {
        return player ? 1 : -1;
    }

    if(player) {
        if(p1) {
            return (p1 == 3) ? 95 : (1 << p1);
        }
        else {
             return (p2 == 3) ? 5 : (1 << (p2 - 1));
        }
    }

    else {
        if(p2) {
            return (p2 == 3) ? -95 : -(1 << p2);
        }
        else {
            return (p1 == 3) ? -5 : -(1 << (p1 - 1));
        }
    }
}


 /// Delta score caused only by the last placed coin at (col,row)

Score_t delta_score(Board_t *b, Col_t col, int row, bool player)
{
    Score_t total_delta = 0;
    Score_t delta;

    for (int8_t d = 0; d < 4; d++) {
        uint8_t dir = DIRS[d];
        int8_t dx = GET_DX(dir);
        int8_t dy = GET_DY(dir);

        int8_t p1_count = 0;
        int8_t p2_count = 0;
        int8_t cells = 0;

        // counted first 3 cells 
        for (int8_t i = 3; i > 0; i--) {
            int8_t c = col - i * dx;
            int8_t r = row - i * dy;
            
            if ((c >= 0) & (c < NUM_COLS) & (r >= 0) & (r < NUM_ROWS)) {
                uint8_t m = (1 << r);
                p1_count += (b->p1[c] & m) ? 1 : 0;
                p2_count += (b->p2[c] & m) ? 1 : 0;
                cells++;
            }
            else { continue; }
        }

        // check if we have window 
            if(cells >= 3) {
                delta = eval_window(p1_count, p2_count, player);
                if (delta == 95) return SCORE_MAX;
                if (delta == -95) return SCORE_MIN;
                total_delta += delta;
            }

        // counted last 3 cells
        for (int8_t i = 1; i <= 3; i++) {

            int8_t c = col + i * dx;
            int8_t r = row + i * dy;
            
            // check next cell is valid
            if ((c >= 0) & (c < NUM_COLS) & (r >= 0) & (r < NUM_ROWS)) {
                uint8_t m = (1 << r);
                p1_count += (b->p1[c] & m) ? 1 : 0;
                p2_count += (b->p2[c] & m) ? 1 : 0;
                cells++;

                // check if we have window 
                if(cells >= 3) {
                    // Remove leftmost cell if window has 4 cells
                    if(cells >= 4) {
                        int8_t c_del = c - (dx << 2); // multiply by 4 using shift
                        int8_t r_del = r - (dy << 2);
                        uint8_t m_del = (1 << r_del);
                        p1_count -= (b->p1[c_del] & m_del) ? 1 : 0;
                        p2_count -= (b->p2[c_del] & m_del) ? 1 : 0;
                    }

                    
                    delta = eval_window(p1_count, p2_count, player);
                    if (delta == 95) return SCORE_MAX;
                    if (delta == -95) return SCORE_MIN;
                    total_delta += delta;
                }
        
            }
            // if not no need to continue
            else { break; }
        }

    }
    return total_delta;
}

///  Minimax (incremental score) 

Result_t minimax(Board_t *b, int depth, Score_t score, 
                 bool maximizing, Score_t alpha, Score_t beta)
{

    if (depth == 0 || board_full(b))
        return (Result_t){0, score};

    Result_t best;
    best.move = 0;
    best.score = maximizing ? SCORE_MIN : SCORE_MAX;

    for (Col_t c = 0; c < NUM_COLS; c++) {

        if (b->height[c] >= NUM_ROWS) continue;

        int row = b->height[c];

        // calculate how score will change 
        Score_t delta = delta_score(b, c, row, maximizing);

        Result_t r;

        if((delta == SCORE_MAX) & maximizing) {
            r = (Result_t){0, SCORE_MAX};
        }

        else if((delta == SCORE_MIN) & !maximizing) {
            r = (Result_t){0, SCORE_MIN};
        }

        else {
            // change board
            drop_piece(b, c, maximizing);
            // new score
            Score_t newScore = score + delta;

            // recurse
            r = minimax(b, depth - 1, newScore,
                                !maximizing, alpha, beta);

            // revert board
            undo_piece(b, c, maximizing);
        }
        

        if (maximizing) {
            if (r.score > best.score) {
                best.score = r.score;
                best.move = c;
            }
            if (best.score > alpha) alpha = best.score;
        } else {
            if (r.score < best.score) {
                best.score = r.score;
                best.move = c;
            }
            if (best.score < beta) beta = best.score;
        }

        if (alpha >= beta) break;
    }
    return best;
}

/// Main 

int main(void)
{

    Board_t board;
    board_init(&board);

    uint8_t bot_turn = 0;  // 0 = bot starts, 1 = player starts
    uint8_t turn = 0;  // 0 = first player move, 1 = second player move
    uint8_t input;
    Score_t score = 0;
    Score_t delta;
    bool first_move = true;

    
    printf("CONNECT 4\n");
    printf("==========\n\n");

    // Choose who goes first
    set_rand_seed(__TIME__[7] * 100 + __TIME__[6] * 10 + __TIME__[4]);
    

    // Game loop
    while (1) {
        draw_board(&board);

        if (turn == bot_turn) {
            // Bot's turn
            printf("🤖 Bot thinking...\n");
            
            Col_t bot_move;
            
            // First move: play randomly in middle columns (3,4,5)
            if (first_move && bot_turn == 0) {
                // Random column from center 3 columns (2, 3, 4 in 0-indexed)
                uint8_t center_cols[3] = {2, 3, 4};
                bot_move = center_cols[rand_col() % 3];
                printf("Bot plays column %d (opening move)\n", bot_move + 1);
                first_move = false;
            }
            else {
                // Use minimax
                Result_t r = minimax(&board, MAX_DEPTH, score, true, SCORE_MIN, SCORE_MAX);
                bot_move = r.move;
                printf("Bot plays column %d\n", bot_move + 1);
            }
            
            int8_t row = board.height[bot_move];
            delta = delta_score(&board, bot_move, row, true);
            score += delta;
            drop_piece(&board, bot_move, true);
        } 
        else {
            // Player's turn
            printf("Your move (1-7): ");
            
            if (scanf("%hhu", &input) != 1) {
                printf("Invalid input!\n");
                while(getchar() != '\n');
                continue;
            }
            
            Col_t c = COL(input);
            if (c >= NUM_COLS || board.height[c] >= NUM_ROWS) {
                printf("Invalid move! Try again.\n");
                continue;
            }
            
            int8_t row = board.height[c];
            delta = delta_score(&board, c, row, false);
            score += delta;
            drop_piece(&board, c, false);
            
            first_move = false;  // Player moved, no longer first move
        }

        // Check win conditions
        if (delta >= 95) {
            draw_board(&board);
            if (turn == bot_turn) {
                printf("🤖 BOT WINS\n");
            } else {
                printf("🎉 YOU WIN\n");
            }
            break;
        }
        if (delta <= -95) {
            draw_board(&board);
            if (turn == bot_turn) {
                printf("🎉 YOU WIN\n");
            } else {
                printf("🤖 BOT WINS\n");
            }
            break;
        }
        if (board_full(&board)) {
            draw_board(&board);
            printf("🤝 DRAW\n");
            break;
        }

        turn ^= 1;  // Toggle turn
    }
    
    return 0;
}
