#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "init.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void key_init(struct key* obj)
{
	obj->pressed = false;
}

void mouse_init(struct mouse* obj)
{
	obj->pressed = false;
}

void ship_init(struct ship* ship)
{
	//because the ship triangle is drawn so that its circumcenter is at the local origin
	//therefore the radius of the bounding circle is simply the highest local y coordinates of the ship
	ship->radius = sqrtf(0.5f);

	ship->rc = 315;

	ship->fillr = SHIP_FILL_R;
	ship->fillg = SHIP_FILL_G;
	ship->fillb = SHIP_FILL_B;
	ship->outr = SHIP_OUTLINE_R;
	ship->outg = SHIP_OUTLINE_G;
	ship->outb = SHIP_OUTLINE_B;

	ship->velocity = SHIP_ROTATE_SPEED;
	ship->rotate_angle = SHIP_ROTATE_SPEED;

	ship->alive = true;
	ship->acceleration = ACCELERATION_RATE;
	ship->decelrataion = DECELERATION_RATE;
	ship->last_velocity = 0;

}

void asteroid_init(struct asteroid* ast, float velmin, float velmax, float colr, float colg, float colb, float radius_min, float radius_max, float rotate_min, float rotate_max)
{
	float velocity_random = rand() / (float)RAND_MAX;
	float circle_random = rand() / (float)RAND_MAX;
	float rotate_random = rand() / (float)RAND_MAX;
	float radius_random = rand() / (float)RAND_MAX;

	ast->velocity = velmin + velocity_random * (velmax - velmin);

	ast->circle_lines = 6 + circle_random * (32 - 6);
	ast->random_angles[ast->circle_lines];

	for (int i = 0; i < ast->circle_lines; i++)
	{
		float rand_scale = rand() / (float)RAND_MAX;
		float random_number = 0 + rand_scale * 1;
		ast->random_angles[i] = random_number;
	}

	ast->rotate_speed = rotate_min + rotate_random * (rotate_max - rotate_min);
	ast->rotate_min = rotate_min;
	ast->rotate_max = rotate_max;

	int dir_array[2] = { -1,1 };
	int random_dir = rand() % 2;
	ast->rotate_dir = dir_array[random_dir];

	ast->radius = radius_min + radius_random * (radius_max - radius_min);
	ast->radius_min = radius_min;
	ast->radius_max = radius_max;

	ast->velmin = velmin;
	ast->velmax = velmax;
	ast->r = colr;
	ast->g = colg;
	ast->b = colb;

	ast->rc = 0;

	float hitpoint_f = 1 + HIT_POINT_MULTIPLIER * ast->radius;
	ast->hitpoint = (int)round(hitpoint_f);


	ast->alive = false;
	ast->launched = false;
	ast->inside = false;
	ast->split = false;
	ast->exploded = false;


}

void ship_particle_init(struct ship_particle* ship_particle, int number, float drop_time, float starting_size, float decay_time)
{
	if (number > MAX_SHIP_PARTICLES)
	{
		number = MAX_SHIP_PARTICLES;
	}

	ship_particle->number = number;
	ship_particle->drop_time = drop_time;
	ship_particle->starting_size = starting_size;
	ship_particle->decay_time = decay_time;
	ship_particle->moving = false;
}

void bullet_init(struct bullet* bullet, float fire_rate, float speed, float size, float r, float g, float b)
{
	bullet->fire_rate = fire_rate;
	bullet->speed = speed;
	bullet->size = size;
	bullet->r = r;
	bullet->g = g;
	bullet->b = b;
	bullet->alive = false;
	bullet->fired = false;
}

void asteroids_particles_init(struct asteroid_particle* particles, float decay_time) 
{
	for (int i = 0; i <= MAX_ASTEROIDS_PARTICLES; i++) 
	{
		particles[i].decay_time = decay_time;
		particles[i].r = (float)rand() / RAND_MAX;
		particles[i].g = (float)rand() / RAND_MAX;
		particles[i].b = (float)rand() / RAND_MAX;

		float velocity_random = rand() / (float)RAND_MAX;
		float vel = PARTICLES_VEL_MIN + velocity_random * (PARTICLES_VEL_MAX - PARTICLES_VEL_MIN);
		particles[i].velocity = vel;

		float vector_random = rand() / (float)RAND_MAX;
		struct vec2d random_dir;
		int dir_array[2] = { -1,1 };
		int random_y = rand() % 2;
		random_dir.x = (-1 + vector_random * 2);
		random_dir.y =  sqrtf(1 - pow(random_dir.x, 2)) * dir_array[random_y];

		particles[i].dir.x = random_dir.x;
		particles[i].dir.y = random_dir.y;

		particles[i].in_use = false;
		particles[i].size = PARTICLES_SIZE;
	}
}