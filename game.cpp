//
// Created by Garrett on 6/24/2023.
//

#include <iostream>

#define is_down(b) input->buttons[b].is_down
#define pressed(b) input->buttons[b].is_down && input->buttons[b].changed
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_2_half_size_x = 2.5;
float player_half_size_y = 12, player_2_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 75, ball_dp_y, ball_half_size=1;

internal void
simulate_player(float *p, float *dp, float ddp, float dt) {
    // Friction
    ddp -= *dp * 10.f;
    // Position and Speed with Acceleration
    *p = *p + *dp * dt + ddp * dt * dt * .5f;
    *dp = *dp + ddp * dt;
    // Wall Collision Detection
    if (*p + player_half_size_y > arena_half_size_y) {
        *p = arena_half_size_y - player_half_size_y;
        *dp *=-0.5;
    } else if (*p - player_half_size_y < -arena_half_size_y) {
        *p = -arena_half_size_y + player_half_size_y;
        *dp *=-0.5;
    }
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
             float p2x, float p2y, float hs2x, float hs2y) {
    return (p1x + hs1x > p2x - hs2x && p1x - hs1x < p2x + hs2x &&
            p1y + hs1y > p2y - hs2y && p1y + hs1y < p2y + hs2y);
}



internal void simulate_game(Input* input, float dt) {
    clear_screen(0xff5500);
    draw_rect(0, 0, 85, 45, 0xffaa33);

    float player_1_ddp, player_2_ddp = 0.f;

    // Acceleration modifiers

    if (is_down(BUTTON_UP)) player_1_ddp += 2000;
    if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;
    if (is_down(BUTTON_W)) player_2_ddp += 2000;
    if (is_down(BUTTON_S)) player_2_ddp -= 2000;

    simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
    simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

    // ---------------- Simulate Ball ------------------ //
    {   // Ball Speed Updates
        ball_p_x += ball_dp_x * dt;
        ball_p_y += ball_dp_y * dt;
        // Ball Collision
        if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size,
                         ball_half_size, 80, player_1_p,
                         player_half_size_x, player_half_size_y)) {
            ball_p_x = 80 - player_half_size_x - ball_half_size;
            ball_dp_x *= -1;
        //ball_dp_y = player_s*.75f;
            ball_dp_y = 35.f * (1 - abs((player_half_size_y - (ball_p_y -
                    player_1_p))/ player_half_size_y));
        } else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size,
                                ball_half_size, -80, player_2_p,
                                player_2_half_size_x, player_2_half_size_y)) {
            ball_p_x = -80 + player_2_half_size_x + ball_half_size;
            ball_dp_x *= -1;
        //ball_dp_y = player_2_s*.75f;
            ball_dp_y = 35.f * (1 - abs((player_2_half_size_y - (ball_p_y -
                    player_2_p)) /player_2_half_size_y));
        }
        // Top and bottom arena ball collision
        if (ball_p_y + ball_half_size > arena_half_size_y) {
            ball_p_y = arena_half_size_y - ball_half_size;
            ball_dp_y *= -1;
        } else if (ball_p_y - ball_half_size < -arena_half_size_y) {
            ball_p_y = -arena_half_size_y + ball_half_size;
            ball_dp_y *= -1;
        }
        // Left and right arena ball collision + reset
        if (ball_p_x + ball_half_size > arena_half_size_x) {
            ball_p_x = arena_half_size_x - ball_half_size;
            ball_dp_x = 80;
            ball_dp_y = 0;
            ball_p_x = 0;
            ball_p_y = 0;
        } else if (ball_p_x - ball_half_size < -arena_half_size_x) {
            ball_p_x = -arena_half_size_x + ball_half_size;
            ball_dp_x = 80;
            ball_dp_y = 0;
            ball_p_x = 0;
            ball_p_y = 0;
        }
    }

    // Draw Objects
    draw_rect(ball_p_x, ball_p_y, ball_half_size,
              ball_half_size,0xffffff);
    draw_rect(-80, player_2_p, player_2_half_size_x,
              player_2_half_size_y,0xffffff);
    draw_rect(80, player_1_p, player_half_size_x,
              player_half_size_y,0xffffff);
}