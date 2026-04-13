#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "Hello Raylib");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello, Sway!", 190, 200, 40, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
