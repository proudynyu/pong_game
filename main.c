#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>

#define WIDTH   800
#define HEIGHT  600
#define TITLE   "Minigame"
#define FPS     60
#define SPEED   20

typedef struct {
    int x;
    int y;
    int w;
    int h;
    Color c;
} Rect;

static Rect *player = NULL;

void create_character() {
    player = malloc(sizeof(*player));
    assert(player != NULL);
    memset(player, 0, sizeof(*player));

    player->x = GetScreenWidth() / 2;
    player->y = GetScreenHeight() / 2;
    player->w = 10;
    player->h = 10;
    player->c = RED;
}

void draw_player() {
    DrawRectangle(player->x, player->y, player->w, player->h, player->c);
}

void player_movement() {
    if (IsKeyPressed(KEY_W) && player->y > 0) {
        player->y -= SPEED;
    }
    if (IsKeyPressed(KEY_S) && player->y < HEIGHT) {
        player->y += SPEED;
    }
    if (IsKeyPressed(KEY_D) && player->x < WIDTH) {
        player->x += SPEED;
    }
    if (IsKeyPressed(KEY_A) && player->x > 0) {
        player->x -= SPEED;
    }
}

void update() {
    draw_player();
    player_movement();
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    create_character();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            update();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
