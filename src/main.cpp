#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

int br = 115200;
int dt = 80;

ArduinoLEDMatrix matrix;
const byte maxRows = 8;
const byte maxCols = 12;
int rDirection = 1;int cDirection = 1;

byte frame[maxRows][maxCols] = {};
byte currentRow = 3;
byte currentCol = 0;

void setup() {
    Serial.begin(br);
    matrix.begin();
    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            frame[i][j] = 0;
        }
    }

    frame[currentRow][currentCol] = 1;
    matrix.renderBitmap(frame, 8, 12);
    delay(dt);
}

void itHitBottomWall(byte currentRow, byte currentCol, int *rDirection, int *cDirection) {
    if (currentRow < (maxRows - 1) || currentCol >= maxCols) {
        return;
    }

    if ( *rDirection > 0 && *cDirection > 0 ) {
        *rDirection = -1;
        *cDirection = 1;
    }

    if ( *rDirection > 0 && *cDirection < 0 ) {
        *rDirection = -1;
        *cDirection = -1;
    }
}

void itHitTopWall(byte currentRow, byte currentCol, int *rDirection, int *cDirection) {
    if (currentRow > 0) {
        return;
    }

    if (currentCol < (maxCols -1) && *rDirection < 0 && *cDirection > 0 ) {
        *rDirection = 1;
        *cDirection = 1;
    }

    if (currentCol < (maxCols -1) && *rDirection < 0 && *cDirection < 0 ) {
        *rDirection = 1;
        *cDirection = -1;
    }
}

void itHitRightWall(byte currentRow, byte currentCol, int *rDirection, int *cDirection) {
    if (currentCol < (maxCols - 1)) {
        return;
    }

    if (currentRow < maxRows -1 && *rDirection > 0 && *cDirection > 0 ) {
        *rDirection = 1;
        *cDirection = -1;
    }

    if (currentRow < maxRows -1 && *rDirection < 0 && *cDirection > 0 ) {
        *rDirection = -1;
        *cDirection = -1;
    }
}

void itHitLeftWall(byte currentRow, byte currentCol, int *rDirection, int *cDirection) {
    if (currentCol > 0) {
        return;
    }

    if (*rDirection < 0 && *cDirection < 0 ) {
        *rDirection = -1;
        *cDirection = 1;
    }

    if (*rDirection > 0 && *cDirection < 0 ) {
        *rDirection = 1;
        *cDirection = 1;
    }
}

void itHitTheCorners(byte currentRow, byte currentCol, int *rDirection, int *cDirection) {
    if (currentRow == 0 && currentCol == 0) {
        *rDirection = 1;
        *cDirection = 1;
    }

    if (currentRow == 0 && currentCol == (maxCols - 1)) {
        *rDirection = 1;
        *cDirection = -1;
    }

    if (currentRow == (maxRows - 1) && currentCol == 0) {
        *rDirection = -1;
        *cDirection = 1;
    }

    if (currentRow == (maxRows - 1) && currentCol == (maxCols - 1)) {
        *rDirection = -1;
        *cDirection = -1;
    }

}

void loop() {
    int prevRow = currentRow;
    int prevCol = currentCol;

    itHitBottomWall(currentRow, currentCol, &rDirection, &cDirection);
    itHitTopWall(currentRow, currentCol, &rDirection, &cDirection);
    itHitRightWall(currentRow, currentCol, &rDirection, &cDirection);
    itHitLeftWall(currentRow, currentCol, &rDirection, &cDirection);
    itHitTheCorners(currentRow, currentCol, &rDirection, &cDirection);


    currentRow = currentRow + rDirection;
    currentCol = currentCol + cDirection;

    frame[prevRow][prevCol] = 0;
    frame[currentRow][currentCol] = 1;
    matrix.renderBitmap(frame, 8, 12);
    delay(dt);
}