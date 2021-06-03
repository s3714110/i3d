#ifndef _INIT_H
#define _INIT_H

#include "config.h"
#include "vec2d.h"
#include "ship.h"
#include "asteroid.h"
#include "bullet.h"
#include "key.h"
#include "mouse.h"
#include "ship_particle.h"
#include "asteroid_particle.h"


void key_init(struct key* obj);
void mouse_init(struct mouse* obj);
void ship_init(struct ship* ship);
void asteroid_init(struct asteroid* ast, float velmin, float velmax, float colr, float colg, float colb, float radius_min, float radius_max, float rotate_min, float rotate_max);
void ship_particle_init(struct ship_particle* ship_particle, int number, float drop_time, float starting_size, float decay_time);
void bullet_init(struct bullet* bullet, float fire_rate, float speed, float size, float r, float g, float b);
void asteroids_particles_init(struct asteroid_particle* particles, float decay_time);

#endif /*_INIT_H */