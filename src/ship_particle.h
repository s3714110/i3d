#ifndef _SHIP_PARTICLE_H
#define _SHIP_PARTICLE_H

#include "vec2d.h"

struct ship_particle{
	struct vec2d pos;      // position

	int number;
	float drop_time;
	float starting_size;
	float decay_time;
	float r, g, b;
	bool moving;

};

#endif /* _SHIP_PARTICLE_H*/