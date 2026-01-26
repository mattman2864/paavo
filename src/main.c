#include "player.h"
#include <raylib.h>

int main(void) {
    InitWindow(800, 600, "paavo");
    SetTargetFPS(GetMonitorRefreshRate(0));

    Player p = *player_create();
    
    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        Vector2 movement = {
          IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
          IsKeyDown(KEY_S) - IsKeyDown(KEY_W)
        };
        player_move(&p, movement);
        body_update(&p.body, delta);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        player_draw(&p);
        
        DrawFPS(0, 0);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
