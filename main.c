#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>

#define WIDTH       800
#define HEIGHT      600
#define TITLE       "Minigame"
#define FPS         60
#define SPEED       20
#define P_WIDTH     10
#define P_HEIGHT    100

typedef struct {
    int x;
    int y;
    int w;
    int h;
    Color c;
} Player;

static Player *player = NULL;

void create_player() {
    player = malloc(sizeof(*player));
    assert(player != NULL);
    memset(player, 0, sizeof(*player));

    player->x = 0;
    player->y = GetScreenHeight() / 2;
    player->w = P_WIDTH;
    player->h = P_HEIGHT;
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
}

void draw_middle_divisor() {
    DrawRectangle(
            GetScreenWidth() / 2,
            GetScreenHeight() / 2,
            1,
            HEIGHT - 10,
            WHITE
    );
}

void update_player() {
    draw_player();
    draw_middle_divisor();
    player_movement();
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    create_player();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            update_player();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
