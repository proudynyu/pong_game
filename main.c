#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>

#define WIDTH       800
#define HEIGHT      600
#define TITLE       "Pong"
#define FPS         60
#define SPEED       10
#define P_WIDTH     10
#define P_HEIGHT    100

typedef struct {
    int x;
    int y;
    int w;
    int h;
    Color c;
} Player;

typedef struct {
    int x; 
    int y;
    unsigned short int size;
    Vector2 direction;
} Ball;

static Player *player = NULL;
static Player *enemy = NULL;
static Ball *ball = NULL;

void characters_init() {
    player = malloc(sizeof(*player));
    assert(player != NULL);
    memset(player, 0, sizeof(*player));

    enemy = malloc(sizeof(*enemy));
    assert(enemy != NULL);
    memset(enemy, 0, sizeof(*enemy));

    ball = malloc(sizeof(*ball));
    assert(ball != NULL);
    memset(ball, 0, sizeof(*ball));
}

void init_character(Player *p, int posX, Color color) {
    p->x = posX;
    p->y = GetScreenHeight() / 2 - P_HEIGHT/2;
    p->w = P_WIDTH;
    p->h = P_HEIGHT;
    p->c = color;
}

void draw(Player *p) {
    DrawRectangle(p->x, p->y, p->w, p->h, p->c);
}

void player_movement() {
    if (IsKeyDown(KEY_W) && player->y > 0) {
        player->y -= SPEED;
    }
    if (IsKeyDown(KEY_S) && player->y < HEIGHT - player->h) {
        player->y += SPEED;
    }
}

void draw_middle_divisor() {
    DrawRectangle(
            GetScreenWidth() / 2,
            10,
            1,
            GetScreenHeight() - 20,
            WHITE
            );
}

void score() {
    DrawText("Score: ", 40, 10, 16, WHITE);
}

void init_ball() {
    ball->size = 8;
    ball->x = GetScreenWidth() / 2;
    ball->y = GetScreenHeight() / 2;
}

void update_ball() {
    DrawCircle(ball->x, ball->y, ball->size, YELLOW);
}

void update() {
    draw(player);
    draw(enemy);
    draw_middle_divisor();
    player_movement();

    update_ball();
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    characters_init();

    init_character(player, 5, RED);
    init_character(enemy, GetScreenWidth() - P_WIDTH - 5, GREEN);
    init_ball();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        score();
        update();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
