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
    int w;
    int h;
    Vector2 position;
    Color c;
} Player;

typedef struct {
    unsigned short int size;
    Vector2 direction;
    Vector2 position;
} Ball;

static Player *player = NULL;
static Player *enemy = NULL;
static Ball *ball = NULL;
bool isGamePaused = false;

void init_character(Player *p, float posX, Color color) {
    int halfScreen = GetScreenHeight() / 2 - P_HEIGHT/2;
    p->position.x = posX;
    p->position.y = (float)halfScreen;
    p->w = P_WIDTH;
    p->h = P_HEIGHT;
    p->c = color;
}

void draw(Player *p) {
    DrawRectangle(p->position.x, p->position.y, p->w, p->h, p->c);
}

void player_movement(float dt) {
    if (IsKeyDown(KEY_W) && player->position.y > 0) {
        player->position.y -= SPEED * dt;
    }
    if (IsKeyDown(KEY_S) && player->position.y < HEIGHT - player->h) {
        player->position.y += SPEED * dt;
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
    int halfScreenX = GetScreenWidth() / 2;
    int halfScreenY = GetScreenHeight() / 2;
    ball->direction.x = -1;
    ball->direction.y = 0.3f;
    ball->size = 8.0f;
    ball->position.x = (float)halfScreenX;
    ball->position.y = (float)halfScreenY;
}

void draw_ball() {
    DrawCircle(ball->position.x, ball->position.y, ball->size, YELLOW);
}

bool is_colliding(float dt) {
    bool collidingWithPlayer = 
        ball->position.x - (int)ball->size - (SPEED * dt) <= player->position.x + player->w &&
        ball->position.y - (int)ball->size >= player->position.y &&
        ball->position.y + (int)ball->size <= player->position.y + player->h;

    bool collidingWithEnemy = 
        ball->position.x + (int)ball->size + (SPEED * dt) >= enemy->position.x &&
        ball->position.y + (int)ball->size >= enemy->position.y &&
        ball->position.y + (int)ball->size <= enemy->position.y + enemy->h;

    if (collidingWithPlayer || collidingWithEnemy) return true;
    return false;
}

void update_ball(float dt) {
    ball->position.x += ball->direction.x * (SPEED * dt);

    bool isCollinding = is_colliding(dt);
    if (isCollinding) {
        int middle_player_point_y = (player->position.y + player->h) / 2;

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
        float dt = GetFrameTime();
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
