#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <memory.h>

//TODO: Multithreading
//TODO: Chunking the grid 
//TODO: Camera movement

#define GRID_WIDTH 32
#define GRID_HEIGHT 24
#define LIVE_CELL_COLOR BLACK
#define DEAD_CELL_COLOR WHITE
#define GRID_LINE_COLOR DARKGRAY
#define PAUSED_TEXT_COLOR BLACK
#define PAUSED_TEXT "Paused, press SPACE to resume"
#define PAUSED_TEXT_POSITION_X 10
#define PAUSED_TEXT_POSITION_Y 10
#define PAUSED_TEXT_SIZE 20

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Function to print the current state of the grid
void printGrid(int* grid){
    for (int row = 0; row < GRID_HEIGHT; row++){
        for(int col = 0; col < GRID_WIDTH; col++){
            printf(grid[row] & (1 << (GRID_WIDTH - col - 1)) ? "#" : "*");
        }
        printf("\n");
    }
    printf("\n\n");
}

// Function to draw a cell on the screen
void drawCell(int cellX, int cellY, Color color) {
    int cellWidth = ceil(SCREEN_WIDTH / GRID_WIDTH)   ;
    int cellHeight = ceil(SCREEN_HEIGHT / GRID_HEIGHT);
    DrawRectangle(cellX * cellWidth, cellY * cellHeight, cellWidth, cellHeight, color);
}

// Function to set the state of a cell based on the Game of Life rules
int setCellState(int* cellGrid, int gridX, int gridY){
    int neighbourCount = 0;
    int isAlive = cellGrid[gridY] & ( 1 << (GRID_WIDTH-gridX-1));
    int newState;

    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            int check = cellGrid[(gridY + i + GRID_HEIGHT) % GRID_HEIGHT] & (1 << (GRID_WIDTH - ((gridX + j + GRID_WIDTH) % GRID_WIDTH) - 1));
            if(check != 0){
                neighbourCount += 1;
            }
        }
    }

    newState = (isAlive && (neighbourCount == 2 || neighbourCount == 3)) || (!isAlive && neighbourCount == 3);

    return newState;
}

// Function to get the cell at a specific screen coordinate
void getCell(int mouseX, int mouseY, int *gridX, int *gridY) {
    *gridX = ceil((float)mouseX * GRID_WIDTH / GetScreenWidth());
    *gridY = floor((float)mouseY * GRID_HEIGHT / GetScreenHeight());
}

// Main function
int main(void) {
    int targetFPS = 10;
    int gridX;
    int gridY;
    int* cellGrid = calloc(GRID_HEIGHT, sizeof(int));
    int* newGridState = calloc(GRID_HEIGHT, sizeof(int));
    bool paused = true;

    if(!cellGrid || !newGridState){
        printf("Memory allocation failed\n");
        return 1;
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");
    SetTargetFPS(targetFPS);

    if (!IsWindowReady()) {
        printf("Window could not be created\n");
        return 1;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (IsMouseButtonPressed(0)) {
            getCell(GetMouseX(), GetMouseY(), &gridX, &gridY);
            cellGrid[gridY] ^= 1<<((GRID_WIDTH-gridX)%GRID_WIDTH);
            drawCell(gridX, gridY, cellGrid[gridY] & 1<<((GRID_WIDTH-gridX)%GRID_WIDTH) ? LIVE_CELL_COLOR : DEAD_CELL_COLOR);
        }
        
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;

        if (IsKeyDown(KEY_UP)) {
            targetFPS += 1;
            SetTargetFPS(targetFPS);
        }
        if (IsKeyDown(KEY_DOWN)) {
            targetFPS -= 1;
            if (targetFPS < 1) targetFPS = 1;
            SetTargetFPS(targetFPS);
        }

        if (IsKeyPressed(KEY_R)) {
            memset(cellGrid, 0, GRID_HEIGHT * sizeof(int));
            memset(newGridState, 0, GRID_HEIGHT * sizeof(int));
        }

        for (int i = 0; i < GRID_HEIGHT; i++){
            for (int j = 0; j < GRID_WIDTH; j++){
                drawCell(j, i, cellGrid[i] & 1 << (GRID_WIDTH - j - 1) ? LIVE_CELL_COLOR : DEAD_CELL_COLOR);

                if (!paused){
                    newGridState[i] |= setCellState(cellGrid, j, i) << (GRID_WIDTH - j - 1);
                }
            }
        }

        if(!paused) {
            DrawText(TextFormat("Target FPS: %d -- Real FPS: %d", targetFPS, GetFPS()), 10, 5, 20, RED);
            memcpy(cellGrid, newGridState, GRID_HEIGHT * sizeof(int));
            memset(newGridState, 0, GRID_HEIGHT * sizeof(int));
        }

        for (int i = 0; i < GetScreenWidth(); i++) {
            if (i % (GetScreenWidth() / GRID_WIDTH) == 0) {
                DrawLine(i, 0, i, SCREEN_HEIGHT, GRID_LINE_COLOR);
            }
        };

        for (int i = 0; i < GetScreenHeight(); i++) {
            if (i % (GetScreenHeight() / GRID_HEIGHT) == 0) {
                DrawLine(0, i, SCREEN_WIDTH, i, GRID_LINE_COLOR);
            }
        };

        if (paused) {
            DrawText(PAUSED_TEXT, PAUSED_TEXT_POSITION_X, PAUSED_TEXT_POSITION_Y, PAUSED_TEXT_SIZE, PAUSED_TEXT_COLOR);
        }

        EndDrawing();
    }

    free(newGridState);
    free(cellGrid);
    CloseWindow();
    return 0;
}
