#ifndef _CONFIG_H
#define _CONFIG_H

#define KEY_ESC 27
#define MARGIN_ADJ 0.01

#define ARENA_WIDTH 1280
#define ARENA_HEIGHT 720
#define DEFAULT_ARENA_R 1
#define DEFAULT_ARENA_G 1
#define DEFAULT_ARENA_B 1
#define WARNING_DISTANCE 20

#define SHIP_CIRCLE_LINE 50
#define ASTEROID_LAUNCH_CIRCLE_LINE 100


#define SHIP_FILL_R 1
#define SHIP_FILL_G 0
#define SHIP_FILL_B 0
#define SHIP_OUTLINE_R 1
#define SHIP_OUTLINE_G 1
#define SHIP_OUTLINE_B 1
#define SHIP_ROTATE_SPEED 360
#define SHIP_MOVING_SPEED 300
#define ACCELERATION_RATE 150
#define DECELERATION_RATE 250

#define MAX_SHIP_PARTICLES 100
#define NUMBER_PARTICLES 20
#define STARTING_SIZE 10
#define DROP_TIME 1
#define DECAY_TIME 1


#define MOVE_FORWARD_KEY 119
#define ROTATE_LEFT_KEY 97
#define ROTATE_RIGHT_KEY 100

#define ASTEROID_R 0
#define ASTEROID_G 0
#define ASTEROID_B 1
#define MAX_ASTEROIDS 100
#define MIN_ASTEROID_SPEED 100
#define MAX_ASTEROID_SPEED 200
#define MIN_ASTEROID_RADIUS 0.5
#define MAX_ASTEROID_RADIUS 2
#define MIN_ASTEROID_ROTATE_SPEED 90
#define MAX_ASTEROID_ROTATE_SPEED 180
#define TIME_BETWEEN_WAVES 5
#define INCREMENT_PER_WAVE 1
#define HIT_POINT_MULTIPLIER 5

#define MAX_ASTEROIDS_PARTICLES MAX_ASTEROIDS * 2 * NUMBER_ASTEROID_PARTICLES
#define NUMBER_ASTEROID_PARTICLES 20
#define PARTICLES_VEL_MIN 50
#define PARTICLES_VEL_MAX 200
#define PARTICLES_SIZE 5
#define ASTEROIDS_PARTICLES_DECAY_TIME 5

#define MAX_BULLETS 100
#define BULLET_FIRE_RATE 0.1
#define BULLET_SPEED 500
#define BULLET_SIZE 5
#define BULLET_R 0
#define BULLET_G 1
#define BULLET_B 1


#endif /*_CONFIG_H*/