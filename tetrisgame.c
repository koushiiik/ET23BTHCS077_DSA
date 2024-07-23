#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// Define the tetromino shapes
char *tetrominos[7];
int field[BOARD_WIDTH * BOARD_HEIGHT]; // The game board

int rotate(int px, int py, int r) {
    switch (r % 4) {
        case 0: return py * 4 + px;         // 0 degrees
        case 1: return 12 + py - (px * 4);  // 90 degrees
        case 2: return 15 - (py * 4) - px;  // 180 degrees
        case 3: return 3 - py + (px * 4);   // 270 degrees
    }
    return 0;
}

int doesPieceFit(int nTetromino, int rotation, int posX, int posY) {
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            // Get index into piece
            int pi = rotate(px, py, rotation);

            // Get index into field
            int fi = (posY + py) * BOARD_WIDTH + (posX + px);

            // Check range
            if (posX + px >= 0 && posX + px < BOARD_WIDTH) {
                if (posY + py >= 0 && posY + py < BOARD_HEIGHT) {
                    // Collision detection
                    if (tetrominos[nTetromino][pi] != '0' && field[fi] != 0)
                        return 0;
                }
            }
        }
    }
    return 1;
}

void initTetrominos() {
    tetrominos[0] = "0000111100000000";
    tetrominos[1] = "0100010001000100";
    tetrominos[2] = "0000111100000000";
    tetrominos[3] = "0100010001000100";
    tetrominos[4] = "0000111100000000";
    tetrominos[5] = "0100010001000100";
    tetrominos[6] = "0000111100000000";
}

int main() {
    initTetrominos();
    for (int x = 0; x < BOARD_WIDTH; x++) // Board Boundary
        for (int y = 0; y < BOARD_HEIGHT; y++)
            field[y * BOARD_WIDTH + x] = (x == 0 || x == BOARD_WIDTH - 1 || y == BOARD_HEIGHT - 1) ? 9 : 0;

    int currentPiece = 0;
    int currentRotation = 0;
    int currentX = BOARD_WIDTH / 2;
    int currentY = 0;

    bool gameOver = false;
    while (!gameOver) {
        // Game Loop
        // Draw Screen
        system("cls");
        for (int x = 0; x < BOARD_WIDTH; x++) {
            for (int y = 0; y < BOARD_HEIGHT; y++) {
                printf("%c", " ABCDEFG=#"[field[y * BOARD_WIDTH + x]]);
            }
            printf("\n");
        }

        // Input
        char keyPress;
        if (_kbhit()) {
            keyPress = _getch();
            switch (keyPress) {
                case 'a': // Move Left
                    if (doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY))
                        currentX--;
                    break;
                case 'd': // Move Right
                    if (doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY))
                        currentX++;
                    break;
                case 'w': // Rotate
                    if (doesPieceFit(currentPiece, currentRotation + 1, currentX, currentY))
                        currentRotation++;
                    break;
                case 's': // Speed up fall
                    if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1))
                        currentY++;
                    break;
            }
        }

        // Logic
        if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1))
            currentY++;
        else {
            // Lock current piece in the field
            for (int px = 0; px < 4; px++) {
                for (int py = 0; py < 4; py++) {
                    if (tetrominos[currentPiece][rotate(px, py, currentRotation)] != '0')
                        field[(currentY + py) * BOARD_WIDTH + (currentX + px)] = currentPiece + 1;
                }
            }

            currentPiece = rand() % 7;
            currentRotation = 0;
            currentX = BOARD_WIDTH / 2;
            currentY = 0;

            // If piece does not fit after reset, game over
            gameOver = !doesPieceFit(currentPiece, currentRotation, currentX, currentY);
        }
    }

    printf("Game Over!!\n");
    return 0;
}
