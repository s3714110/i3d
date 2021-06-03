#ifndef _SHIP_H
#define _SHIP_H

#include "vec2d.h"

struct ship 
{
	struct vec2d pos;      // position

	float size;
	float fillr, fillg, fillb;   // colour
	float outr, outg, outb;

	float rc;          // current rotation amount   // ************

	float radius;

	float velocity;
	float rotate_angle;

	bool alive;

	float acceleration;
	float decelrataion;
	float last_velocity;

};



#endif /* _SHIP_H */