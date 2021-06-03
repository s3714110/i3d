#ifndef _GAME_H
#define _GAME_H

#include "gl.h"
#include "config.h"
#include "vec2d.h"
#include "ship.h"
#include "asteroid.h"
#include "bullet.h"
#include "key.h"
#include "mouse.h"
#include "ship_particle.h"
#include "asteroid_particle.h"

#include "init.h"


//input handling methods
void on_key_press(unsigned char key, int x, int y);
void on_key_release(unsigned char key, int x, int y);
void on_mouse_button(int button, int state, int x, int y);

//window reshape handle
void on_reshape(int w, int h);

//drawing methods
void draw_arena_line(float colr, float colg, float colb, float x_start, float y_start, float x_end, float y_end, bool warning);
void draw_arena(float colr, float colg, float colb, float width, float height);
void draw_ship(struct ship* ship);
void draw_ship_circle(struct ship* ship, int n);
void draw_ship_particles(struct ship* ship, struct ship_particle* particles);
void draw_asteroid_launch_circle(int n);
void draw_asteroid_launch_point(struct asteroid* ast);
void draw_asteroids(struct asteroid* ast);
void draw_bullets(struct bullet* bl, struct ship* ship);
void draw_asteroid_particles(struct asteroid_particle* particles);
void draw_player_stat();
void draw_welcome();
void draw_gameover();
void on_display();


//game states methods

//reset stats methods
void reset_asteroid(struct asteroid* ast);
void reset_wave_time();
void reset_bullet(struct bullet* bl, struct ship* ship);
void ship_reshape(struct ship* ship);
void reset_player_stat();
void reset_asteroids_particles(struct asteroid_particle* particles, float decay_time);
bool all_bullets_fired_dead(struct bullet* bl);
bool all_asteroids_launched_and_dead(struct asteroid* ast);
bool all_asteroid_particles_in_use(struct asteroid_particle* particles);

//update game entities, movement, pos, velocity etc,...
void update_ship_state(struct ship* ship, float dt);
void update_wave_state(struct asteroid* ast, struct ship* ship, float dt);
void update_asteroid_state(struct asteroid* ast, float dt);
void update_asteroid_launch_point(struct asteroid* ast, struct ship* ship);

void update_bullet_state(struct bullet* bl, struct ship* ship, float dt);
void update_asteroids_particles(struct asteroid_particle* particles, struct asteroid* ast, float dt);

//collisions detecion and bouncing methods
void check_wall(struct ship* ship, float warn_dist);
void check_ship_asteroid_collision(struct asteroid* ast, struct ship* ship);
void check_bullet_asteroid_collision(struct bullet* bl, struct asteroid* ast, struct ship* ship);
void check_bullet_arena_collision(struct bullet* bl);
void check_asteroid_inside(struct asteroid* ast);
void check_asteroid_arena_collision(struct asteroid* ast);
void check_asteroids_collision(struct asteroid* ast);
void check_asteroid_split(struct asteroid* ast, float dt);

void on_idle();

void init_app(int* argcp, char** argv);


#endif /*_GAME_H */