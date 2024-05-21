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
#define SPEED       150
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

void player_movement(float dt) {
    if (IsKeyDown(KEY_W) && player->y > 0) {
        player->y -= SPEED * dt;
    }
    if (IsKeyDown(KEY_S) && player->y < HEIGHT - player->h) {
        player->y += SPEED * dt;
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
    ball->direction.x = -1;
    ball->direction.y = 0;
    ball->size = 8.0f;
    ball->x = GetScreenWidth() / 2;
    ball->y = GetScreenHeight() / 2;
}

void draw_ball() {
    DrawCircle(ball->x, ball->y, ball->size, YELLOW);
}

bool is_colliding(float dt) {
    bool collidingWithPlayer = 
        ball->x - (int)ball->size - (SPEED * dt) <= player->x + player->w &&
        ball->y - (int)ball->size >= player->y &&
        ball->y + (int)ball->size <= player->y + player->h;

    bool collidingWithEnemy = 
        ball->x + (int)ball->size + (SPEED * dt) >= enemy->x &&
        ball->y + (int)ball->size >= enemy->y &&
        ball->y + (int)ball->size <= enemy->y + enemy->h;

    if (collidingWithPlayer || collidingWithEnemy) return true;
    return false;
}

void update_ball(float dt) {
    ball->x += ball->direction.x * (SPEED * dt);

    bool isCollinding = is_colliding(dt);
    if (isCollinding) {
        ball->direction.x = (float)((int)ball->direction.x * -1);
    }
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

void update(float dt) {
    score();

    draw(player);
    draw(enemy);
    draw_ball();
    draw_middle_divisor();

    if(!isGamePaused) {
        update_ball(dt);
        player_movement(dt);
    } else {
        draw_pause_text();
    }
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();                                   // Get time in seconds for last frame drawn (delta time)
        BeginDrawing();
            ClearBackground(BLACK);
            keyboard_events();
            update(dt);
        EndDrawing();
    }
    clear();
    CloseWindow();
    return 0;
}
