#ifndef _BULLET_H
#define _BULLET_H

#include "vec2d.h"

struct bullet {
	struct vec2d pos;
	float fire_rate;			//time between shots in seconds
	float speed;
	float size;
	float r, g, b;
	bool alive;
	bool fired;
	float ship_rc;
};

#endif /* _BULLET_H */