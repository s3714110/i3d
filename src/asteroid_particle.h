#ifndef _ASTEROID_PARTICLE_H
#define _ASTEROID_PARTICLE_H

#include "vec2d.h"

struct asteroid_particle {
	int number;
	float decay_time;
	float velocity;
	float r, g, b;
	struct vec2d dir;
	struct vec2d pos;
	bool in_use;
	float size;
};

#endif /*_ASTEROID_PARTICLE_H */