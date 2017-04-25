/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define EMPTY 0

// Condition Constants
#define UP iTile - 1
#define DOWN iTile + 1
#define RIGHT jTile + 1
#define LEFT jTile - 1
#define LAST_TILE i == d - 1 && j == d - 1

// blank space trackers
int blankRow, blankCol;

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    blankRow = d - 1;
    blankCol = d - 1;
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // Set a assignment variable
    int boardNum = d * d - 1;
    // Iterate through the board
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = boardNum--;
        }
    }
    
    // checks if board is even
    if (d % 2 == 0)
    {
        // Since exchanging 2 and 1 
        // Set value of that space as 3 - space
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // iterate through the array and print values
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if (board[i][j] == EMPTY)
            {
                printf("%2c ",'_');
                continue;
            }
            printf("%2d ",board[i][j]);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // find indices of tile by iterating
    int iTile = -1, jTile = -1; 
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                iTile = i;
                jTile = j;
                break;
            }
        }
    }
    // if Tile is not found return false
    if (iTile == -1 && jTile == -1)
    {
        return false;
    }
    
    // Check if empty space is in surrounding rows
    if (UP == blankRow || DOWN == blankRow)
    {
        // Checks if Empty space is same column as tile
        if (jTile == blankCol)
        {   
            board[blankRow][blankCol] = tile;
            blankRow = iTile;
            blankCol = jTile;
            board[iTile][jTile] = EMPTY;
            return true;
        }
    }
    // Check if empty space is in surrounding columns
    else if (RIGHT == blankCol || LEFT == blankCol)
    {
        // Checks if Empty space is same row as tile
        if (iTile == blankRow)
        {
            board[blankRow][blankCol] = tile;
            blankRow = iTile;
            blankCol = jTile;
            board[iTile][jTile] = EMPTY;
            return true;
        }
    }
    
    // returns false if control reaches here
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // Check other tiles only if
    // Empty space is in the corner
    int boardNum = 1;
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d ; i++)
        {
            if (LAST_TILE)
            {
                break;
            }
            if (board[i][j] != boardNum++)
            {
                return false;
            }
        }
    }
    // if control is here then board is in winning config
    return true;
    return false;
}
