#ifndef _ASTEROID_H
#define _ASTEROID_H

#include "vec2d.h"

struct asteroid {
	struct vec2d pos;
	struct vec2d dir;
	struct vec2d launch_point;
	float velocity;
	float velmin;
	float velmax;
	float r, g, b;

	float radius;
	float radius_min;
	float radius_max;

	int circle_lines;
	float random_angles[32];

	float rc;
	int rotate_dir;
	float rotate_speed;
	float rotate_min;
	float rotate_max;

	bool alive;
	bool launched;
	int hitpoint;

	bool inside;
	bool split;
	bool exploded;

};

#endif /* _ASTEROID_H */