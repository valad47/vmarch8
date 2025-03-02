#include "raylib.h"
#include <stdio.h>

int main() {
    InitWindow(800, 400, "vmarch8");

    while(!WindowShouldClose()) {
        BeginDrawing();

        EndDrawing();
    }

    CloseWindow();
}
