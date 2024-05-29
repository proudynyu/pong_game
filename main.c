/* 
 * *******************************************
 * Made by: Igor Becker - github @proudynyu
 * Date: 29/05/24
 * ********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

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
    Vector2 position;
    Vector2 velocity;
} Ball;

typedef struct {
    int player;
    int enemy;
} Score;

typedef struct {
    Player player;
    Player enemy;
    Ball ball;
    bool running;
    float deltaTime;
    Score score;
} State;

static State *state = NULL;

void init_character(Player *p, float posX, Color color) {
    int halfScreen = GetScreenHeight() / 2 - P_HEIGHT/2;
    p->position.x = posX;
    p->position.y = (float)halfScreen;
    p->w = P_WIDTH;
    p->h = P_HEIGHT;
    p->c = color;
}

void init_ball() {
    float x = (float)GetScreenWidth() / 2;
    float y = (float)GetScreenHeight() / 2;
    state->ball.position.x = x;
    state->ball.position.y = y;
    state->ball.size = 8;
    state->ball.velocity.x = SPEED;
    state->ball.velocity.y = SPEED;
}

void draw_score() {
    DrawText(TextFormat("%d", state->score.player), 30, 10, 32, WHITE);
    DrawText(TextFormat("%d", state->score.enemy), WIDTH - 30, 10, 32, WHITE);
}

void draw_player(Player *p) {
    DrawRectangle(p->position.x, p->position.y, p->w, p->h, p->c);
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

void draw_ball() {
    DrawCircleV(state->ball.position, state->ball.size, WHITE);
}

void init_state() {
    init_character(&state->player, 5, RED);
    init_character(&state->enemy, GetScreenWidth() - P_WIDTH - 5, GREEN);
    init_ball();
}

void init() {
    state = malloc(sizeof(*state));
    assert(state != NULL);
    memset(state, 0, sizeof(*state));

    state->running = true;
    state->score.player = 0;
    state->score.enemy = 0;

    init_state();
}

void draw_pause_text() {
    int fontSize = 32;
    DrawText("Paused!", GetScreenWidth() / 2 - (fontSize * 2), GetScreenHeight() / 2 - fontSize, fontSize, WHITE);
}

void keyboard_events() {
    if (IsKeyPressed(KEY_SPACE)) {
        state->running = !state->running;
    }
    if (IsKeyPressed(KEY_R)) {
        init_state();
    }

    if (state->running) {
        // player movement
        if (IsKeyDown(KEY_W) && state->player.position.y > 0) {
            state->player.position.y -= SPEED * state->deltaTime;
        }
        if (IsKeyDown(KEY_S) && state->player.position.y < HEIGHT - state->player.h) {
            state->player.position.y += SPEED * state->deltaTime;
        }

        // enemy movement
        if (IsKeyDown(KEY_UP) && state->player.position.y > 0) {
            state->enemy.position.y -= SPEED * state->deltaTime;
        }
        if (IsKeyDown(KEY_DOWN) && state->enemy.position.y < HEIGHT - state->enemy.h) {
            state->enemy.position.y += SPEED * state->deltaTime;
        }
    }
}

bool ball_collision_wall() {
    return state->ball.position.y + state->ball.size >= HEIGHT || state->ball.position.y - state->ball.size <= 0;
}

bool ball_collision_player() {
    return state->ball.position.x - state->ball.size <= state->player.position.x + state->player.w &&
        state->ball.position.y >= state->player.position.y &&
        state->ball.position.y <= state->player.position.y + state->player.h;
}

bool ball_collision_enemy() {
    return state->ball.position.x + state->ball.size >= state->enemy.position.x &&
        state->ball.position.y >= state->enemy.position.y &&
        state->ball.position.y <= state->enemy.position.y + state->enemy.h;
}

void update_score() {
    if (state->ball.position.x - state->ball.size <= 0) {
        state->score.enemy++;
        init_ball();
    }
    if (state->ball.position.x + state->ball.size >= WIDTH) {
        state->score.player++;
        init_ball();
    }
}

void update_ball() {
    state->ball.position.x += (-state->ball.velocity.x) * state->deltaTime;
    state->ball.position.y += (state->ball.velocity.y) * state->deltaTime;

    if (ball_collision_wall()) {
        state->ball.velocity.y = -state->ball.velocity.y;
    }

    if (ball_collision_player() || ball_collision_enemy()) {
        state->ball.velocity.x = -state->ball.velocity.x;
    }
}

void update() {
    draw_player(&state->player);
    draw_player(&state->enemy);
    draw_ball();
    draw_middle_divisor();
    draw_score();

    if(!state->running) {
        draw_pause_text();
    } else {
        update_ball();
        update_score();
    }
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    init();

    while (!WindowShouldClose()) {
        state->deltaTime = GetFrameTime();
        BeginDrawing();
            ClearBackground(BLACK);
            keyboard_events();
            update();
        EndDrawing();
    }
    free(state);
    CloseWindow();
    return 0;
}
