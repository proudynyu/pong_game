#include <assert.h>
#include <stdio.h>
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
bool isGamePaused = false;

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
    ball->direction.x = -1.0f;
    ball->direction.y = 0.0f;
    ball->size = 8;
    ball->x = GetScreenWidth() / 2;
    ball->y = GetScreenHeight() / 2;
}

void draw_ball() {
    DrawCircle(ball->x, ball->y, ball->size, YELLOW);
}

void update_ball() {
    // int isCollinding = check_collision();
    // if (isCollinding) {
    //  ball->direction.x *= -1;
    //  ball->direction.y *= -1;
    // }
    ball->x += ball->direction.x * SPEED / 2;
    ball->y += ball->direction.y * SPEED / 2;
}

void clear() {
    free(player);
    free(enemy);
    free(ball);
}

void keyboard_events() {
    if (IsKeyPressed(KEY_SPACE)) {
        isGamePaused = !isGamePaused;
    }
}

void init() {
    player = malloc(sizeof(*player));
    assert(player != NULL);
    memset(player, 0, sizeof(*player));

    enemy = malloc(sizeof(*enemy));
    assert(enemy != NULL);
    memset(enemy, 0, sizeof(*enemy));

    ball = malloc(sizeof(*ball));
    assert(ball != NULL);
    memset(ball, 0, sizeof(*ball));

    init_character(player, 5, RED);
    init_character(enemy, GetScreenWidth() - P_WIDTH - 5, GREEN);
    init_ball();
}

void draw_pause_text() {
    int fontSize = 32;
    DrawText("Paused!", GetScreenWidth() / 2 - (fontSize * 2), GetScreenHeight() / 2 - fontSize, fontSize, WHITE);
}

void update() {
    draw(player);
    draw(enemy);
    draw_ball();
    draw_middle_divisor();

    if(!isGamePaused) {
        update_ball();
        player_movement();
    } else {
        draw_pause_text();
    }
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    init();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        keyboard_events();
        score();
        update();
        EndDrawing();
    }
    clear();
    CloseWindow();
    return 0;
}
